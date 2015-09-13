//
// Copyright (c) 2008-2015 the Clockwork project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Core/CoreEvents.h"
#include "../Core/Profiler.h"
#include "../IO/Log.h"
#include "../IO/MemoryBuffer.h"
#include "../Physics/CollisionShape.h"
#include "../Physics/Constraint.h"
#include "../Physics/PhysicsUtils.h"
#include "../Physics/PhysicsWorld.h"
#include "../Physics/SoftBody.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/ResourceEvents.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneEvents.h"
#include "../Scene/SmoothedTransform.h"
#include "../Graphics/StaticModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Geometry.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Drawable.h"
#include "../Graphics/Batch.h"
#include "../Math/BoundingBox.h"

#include <Bullet/BulletSoftBody/btSoftBody.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include <Bullet/BulletCollision/CollisionShapes/btCompoundShape.h>

namespace Clockwork
{
	static const float DEFAULT_SOFT_BODY_MASS = 1.0f;
	static const float DEFAULT_FRICTION = 0.5f;
	static const float DEFAULT_RESTITUTION = 0.0f;
	static const float DEFAULT_ROLLING_FRICTION = 0.0f;
	static const unsigned DEFAULT_COLLISION_LAYER = 0x1;
	static const unsigned DEFAULT_COLLISION_MASK = M_MAX_UNSIGNED;

	extern const char* PHYSICS_CATEGORY;

	SoftBody::SoftBody(Context* context) :
		Component(context),
		body_(0),
		gravityOverride_(Vector3::ZERO),
		centerOfMass_(Vector3::ZERO),
		mass_(DEFAULT_SOFT_BODY_MASS),
		collisionLayer_(DEFAULT_COLLISION_LAYER),
		collisionMask_(DEFAULT_COLLISION_MASK),
		lastPosition_(Vector3::ZERO),
		lastRotation_(Quaternion::IDENTITY),
		useGravity_(true),
		readdBody_(false),
		inWorld_(false),
		enableMassUpdate_(true),
		vb_(0),
		mDupVertices(0),
		mNewIndexes(0)
	{
		// constructor
	}
	
	SoftBody::~SoftBody()
	{
		ReleaseBody();
		
		if (physicsWorld_)
			physicsWorld_->RemoveSoftBody(this);
		
		if (mDupVertices) delete[] mDupVertices;
		if (mNewIndexes) delete[] mNewIndexes;
	}
	
	void SoftBody::RegisterObject(Context* context)
	{
		context->RegisterFactory<SoftBody>(PHYSICS_CATEGORY);
		
		ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
		MIXED_ACCESSOR_ATTRIBUTE("Physics Rotation", GetRotation, SetRotation, Quaternion, Quaternion::IDENTITY, AM_FILE | AM_NOEDIT);
		MIXED_ACCESSOR_ATTRIBUTE("Physics Position", GetPosition, SetPosition, Vector3, Vector3::ZERO, AM_FILE | AM_NOEDIT);
		ATTRIBUTE("Mass", float, mass_, DEFAULT_SOFT_BODY_MASS, AM_DEFAULT);
		ATTRIBUTE("Collision Layer", int, collisionLayer_, DEFAULT_COLLISION_LAYER, AM_DEFAULT);
		ATTRIBUTE("Collision Mask", int, collisionMask_, DEFAULT_COLLISION_MASK, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE("Use Gravity", GetUseGravity, SetUseGravity, bool, true, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE("Gravity Override", GetGravityOverride, SetGravityOverride, Vector3, Vector3::ZERO, AM_DEFAULT);
	}
	
	void SoftBody::OnSetAttribute(const AttributeInfo& attr, const Variant& src)
	{
		Serializable::OnSetAttribute(attr, src);
		
		// Change of any non-accessor attribute requires the rigid body to be re-added to the physics world
		if (!attr.accessor_)
			readdBody_ = true;
		}
	
	void SoftBody::ApplyAttributes()
	{
		if (readdBody_)
			AddBodyToWorld();
	}
	
	void SoftBody::OnSetEnabled()
	{
		bool enabled = IsEnabledEffective();
	
		if (enabled && !inWorld_)
			AddBodyToWorld();
		else if (!enabled && inWorld_)
			RemoveBodyFromWorld();
	}
	
	void SoftBody::getWorldTransform(btTransform& worldTrans) const
	{
		// We may be in a pathological state where a SoftBody exists without a scene node when this callback is fired,
		// so check to be sure
		if (node_)
		{
			lastPosition_ = node_->GetWorldPosition();
			lastRotation_ = node_->GetWorldRotation();
			worldTrans.setOrigin(ToBtVector3(lastPosition_ + lastRotation_ * centerOfMass_));
			worldTrans.setRotation(ToBtQuaternion(lastRotation_));
		}
	}
	
	void SoftBody::setWorldTransform(const btTransform& worldTrans)
	{
		Quaternion newWorldRotation = ToQuaternion(worldTrans.getRotation());
		Vector3 newWorldPosition = ToVector3(worldTrans.getOrigin()) - newWorldRotation * centerOfMass_;
		SoftBody* parentSoftBody = 0;
		
		// It is possible that the SoftBody component has been kept alive via a shared pointer,
		// while its scene node has already been destroyed
		if (node_)
		{
			// If the rigid body is parented to another rigid body, can not set the transform immediately.
			// In that case store it to PhysicsWorld for delayed assignment
			Node* parent = node_->GetParent();
			if (parent != GetScene() && parent)
				parentSoftBody = parent->GetComponent<SoftBody>();
			
			if (!parentSoftBody)
				ApplyWorldTransform(newWorldPosition, newWorldRotation);
			else
			{
				DelayedWorldTransformSoftBody delayed;
				delayed.softBody_ = this;
				delayed.parentSoftBody_ = parentSoftBody;
				delayed.worldPosition_ = newWorldPosition;
				delayed.worldRotation_ = newWorldRotation;
				physicsWorld_->AddDelayedWorldTransformSoftBody(delayed);
			}
			
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::DrawDebugGeometry(DebugRenderer* debug, bool depthTest)
	{
		if (debug && physicsWorld_ && body_ && IsEnabledEffective())
		{
			physicsWorld_->SetDebugRenderer(debug);
			physicsWorld_->SetDebugDepthTest(depthTest);
			
			btDynamicsWorld* world = physicsWorld_->GetWorld();
			//world->debugDrawObject(body_->getWorldTransform(), shiftedCompoundShape_, IsActive() ? btVector3(1.0f, 1.0f, 1.0f) : btVector3(0.0f, 1.0f, 0.0f));
			
			physicsWorld_->SetDebugRenderer(0);
		}
	}
	
	void SoftBody::SetMass(float mass)
	{
		mass = Max(mass, 0.0f);
		
		if (mass != mass_)
		{
			mass_ = mass;
			AddBodyToWorld();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetPosition(const Vector3& position)
	{
		if (body_)
		{
			btTransform& worldTrans = body_->getWorldTransform();
			worldTrans.setOrigin(ToBtVector3(position + ToQuaternion(worldTrans.getRotation()) * centerOfMass_));
			
			// When forcing the physics position, set also interpolated position so that there is no jitter
			btTransform interpTrans = body_->getInterpolationWorldTransform();
			interpTrans.setOrigin(worldTrans.getOrigin());
			body_->setInterpolationWorldTransform(interpTrans);
			
			Activate();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetRotation(const Quaternion& rotation)
	{
		if (body_)
		{
			Vector3 oldPosition = GetPosition();
			btTransform& worldTrans = body_->getWorldTransform();
			worldTrans.setRotation(ToBtQuaternion(rotation));
			if (!centerOfMass_.Equals(Vector3::ZERO))
				worldTrans.setOrigin(ToBtVector3(oldPosition + rotation * centerOfMass_));
			
			btTransform interpTrans = body_->getInterpolationWorldTransform();
			interpTrans.setRotation(worldTrans.getRotation());
			if (!centerOfMass_.Equals(Vector3::ZERO))
				interpTrans.setOrigin(worldTrans.getOrigin());
			
			body_->setInterpolationWorldTransform(interpTrans);
			//body_->updateInertiaTensor();
			
			Activate();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetTransform(const Vector3& position, const Quaternion& rotation)
	{
		if (body_)
		{
			btTransform& worldTrans = body_->getWorldTransform();
			worldTrans.setRotation(ToBtQuaternion(rotation));
			worldTrans.setOrigin(ToBtVector3(position + rotation * centerOfMass_));
			
			btTransform interpTrans = body_->getInterpolationWorldTransform();
			interpTrans.setOrigin(worldTrans.getOrigin());
			interpTrans.setRotation(worldTrans.getRotation());
			body_->setInterpolationWorldTransform(interpTrans);
			//body_->updateInertiaTensor();
			
			Activate();
			MarkNetworkUpdate();
		}
	}
	
	
	void SoftBody::SetUseGravity(bool enable)
	{
		if (enable != useGravity_)
		{
			useGravity_ = enable;
			UpdateGravity();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetGravityOverride(const Vector3& gravity)
	{
		if (gravity != gravityOverride_)
		{
			gravityOverride_ = gravity;
			UpdateGravity();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetCollisionLayer(unsigned layer)
	{
		if (layer != collisionLayer_)
		{
			collisionLayer_ = layer;
			AddBodyToWorld();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetCollisionMask(unsigned mask)
	{
		if (mask != collisionMask_)
		{
			collisionMask_ = mask;
			AddBodyToWorld();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::SetCollisionLayerAndMask(unsigned layer, unsigned mask)
	{
		if (layer != collisionLayer_ || mask != collisionMask_)
		{
			collisionLayer_ = layer;
			collisionMask_ = mask;
			AddBodyToWorld();
			MarkNetworkUpdate();
		}
	}
	
	void SoftBody::Activate()
	{
		if (body_ && mass_ > 0.0f)
			body_->activate(true);
	}
	
	void SoftBody::ReAddBodyToWorld()
	{
		if (body_ && inWorld_)
			AddBodyToWorld();
	}
	
	void SoftBody::DisableMassUpdate()
	{
		enableMassUpdate_ = false;
	}
	
	void SoftBody::EnableMassUpdate()
	{
		if (!enableMassUpdate_)
		{
			enableMassUpdate_ = true;
			UpdateMass();
		}
	}
	
	Vector3 SoftBody::GetPosition() const
	{
		if (body_)
		{
			const btTransform& transform = body_->getWorldTransform();
			return ToVector3(transform.getOrigin()) - ToQuaternion(transform.getRotation()) * centerOfMass_;
		}
	else
		return Vector3::ZERO;
	}
	
	Quaternion SoftBody::GetRotation() const
	{
		return body_ ? ToQuaternion(body_->getWorldTransform().getRotation()) : Quaternion::IDENTITY;
	}
	
	void SoftBody::ApplyWorldTransform(const Vector3& newWorldPosition, const Quaternion& newWorldRotation)
	{
		// In case of holding an extra reference to the SoftBody, this could be called in a situation
		// where node is already null
		if (!node_ || !physicsWorld_)
			return;

		physicsWorld_->SetApplyingTransforms(true);

		// Apply transform to the SmoothedTransform component instead of node transform if available
		if (smoothedTransform_)
		{
			smoothedTransform_->SetTargetWorldPosition(newWorldPosition);
			smoothedTransform_->SetTargetWorldRotation(newWorldRotation);
			lastPosition_ = newWorldPosition;
			lastRotation_ = newWorldRotation;
		}
		else
		{
			node_->SetWorldPosition(newWorldPosition);
			node_->SetWorldRotation(newWorldRotation);
			lastPosition_ = node_->GetWorldPosition();
			lastRotation_ = node_->GetWorldRotation();
		}

		physicsWorld_->SetApplyingTransforms(false);
	}
	
	void SoftBody::UpdateMass()
	{
		if (!body_ || !enableMassUpdate_)
			return;
		
		if (mass_ > 0.0f)
			body_->setTotalMass(mass_, true);
		
	}
	
	void SoftBody::UpdateGravity()
	{
		if (physicsWorld_ && body_)
		{
			btSoftRigidDynamicsWorld* world_ = (btSoftRigidDynamicsWorld*)physicsWorld_->GetWorld();
			btSoftBodyWorldInfo* softBodyWorldInfo_ = physicsWorld_->GetSoftBodyWorld();
			
			if (useGravity_)
			{
				// If override vector is zero, use world's gravity
				if (gravityOverride_ == Vector3::ZERO)
					softBodyWorldInfo_->m_gravity = world_->getGravity();
				else
					softBodyWorldInfo_->m_gravity = ToBtVector3(gravityOverride_);
			}
			else
				softBodyWorldInfo_->m_gravity = btVector3(0.0f, 0.0f, 0.0f);
		}
	}
	
	void SoftBody::ReleaseBody()
	{
		if (body_)
		{
			RemoveBodyFromWorld();
		
			delete body_;
			body_ = 0;
		}
	}
	
	void SoftBody::OnMarkedDirty(Node* node)
	{
		// If node transform changes, apply it back to the physics transform. However, do not do this when a SmoothedTransform
		// is in use, because in that case the node transform will be constantly updated into smoothed, possibly non-physical
		// states; rather follow the SmoothedTransform target transform directly
		// Also, for kinematic objects Bullet asks the position from us, so we do not need to apply ourselves
		if ((!physicsWorld_ || !physicsWorld_->IsApplyingTransforms()) && !smoothedTransform_)
		{
			// Physics operations are not safe from worker threads
			Scene* scene = GetScene();
			if (scene && scene->IsThreadedUpdate())
			{
				scene->DelayedMarkedDirty(this);
				return;
			}
			
			// Check if transform has changed from the last one set in ApplyWorldTransform()
			Vector3 newPosition = node_->GetWorldPosition();
			Quaternion newRotation = node_->GetWorldRotation();
			
			if (!newRotation.Equals(lastRotation_))
			{
				lastRotation_ = newRotation;
				SetRotation(newRotation);
			}
			if (!newPosition.Equals(lastPosition_))
			{
				lastPosition_ = newPosition;
				SetPosition(newPosition);
			}
		}
	}
	
	void SoftBody::OnNodeSet(Node* node)
	{
		if (node)
			node->AddListener(this);
	}
	
	void SoftBody::OnSceneSet(Scene* scene)
	{
		if (scene)
		{
			if (scene == node_)
				LOGWARNING(GetTypeName() + " should not be created to the root scene node");
			
			physicsWorld_ = scene->GetOrCreateComponent<PhysicsWorld>();
			physicsWorld_->AddSoftBody(this);
			
			AddBodyToWorld();
		}	
		else
		{
			ReleaseBody();
			
			if (physicsWorld_)
			physicsWorld_->RemoveSoftBody(this);
		}
	}
	
	void SoftBody::AddBodyToWorld()
	{
		if (!physicsWorld_)
			return;
		
		PROFILE(AddBodyToWorld);
		
		if (mass_ < 0.0f)
			mass_ = 0.0f;
		
		/*if (body_)
			RemoveBodyFromWorld();
		else
		{
			btSoftBodyWorldInfo* softBodyWorldInfo_ = GetPhysicsWorld()->GetSoftBodyWorld();
			body_ = new btSoftBody(softBodyWorldInfo_);
			body_->setUserPointer(this);
		}*/
		
		if (!body_) return;
		UpdateMass();
		UpdateGravity();
		
		int flags = body_->getCollisionFlags();
		/*body_->setCollisionFlags(flags);
		body_->forceActivationState(mass_ > 0.0f ? DISABLE_DEACTIVATION : ISLAND_SLEEPING);*/
		
		if (!IsEnabledEffective())
			return;
		
		btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)physicsWorld_->GetWorld();
		world->addSoftBody(body_, (short)collisionLayer_, (short)collisionMask_);
		inWorld_ = true;
		readdBody_ = false;
		
		//if (mass_ > 0.0f)
		//	Activate();
		
		SubscribeToEvent(E_POSTUPDATE, HANDLER(SoftBody, HandlePostUpdate));
		
	}
	
	void SoftBody::RemoveBodyFromWorld()
	{
		if (physicsWorld_ && body_ && inWorld_)
		{
			btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)physicsWorld_->GetWorld();
			world->removeSoftBody(body_);
			inWorld_ = false;
			UnsubscribeFromEvent(this, E_POSTUPDATE);
		}
	}
	
	bool SoftBody::IsActive() const
	{
		return body_ ? body_->isActive() : false;
	}
	
	void SoftBody::UpdateSoftBodyFromStaticModel()
	{
		// Get model
		StaticModel* model = node_->GetComponent<StaticModel>();
		
		if (model)
		{
			Model* originalModel = model->GetModel();
			if (originalModel)
			{
				bool sucess = false;
				
				// Clone model
				SharedPtr<Model> cloneModel = originalModel->Clone();
				model->SetModel(cloneModel);
				
				if (body_)
				{
					RemoveBodyFromWorld();
				}
				
				// get only first geometry
				if (cloneModel->GetNumGeometries() > 0)
				{
					Geometry* geometry = cloneModel->GetGeometry(0, 0);
					
					
					VertexBuffer* vb = geometry->GetVertexBuffer(0);
					vb_ = vb; // save pointer
					bbmodel = cloneModel;
					IndexBuffer* ib = geometry->GetIndexBuffer();
					sucess = create(vb, ib, true);
				}
				
				// add it
				if (sucess)
				{
					btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)physicsWorld_->GetWorld();
					world->addSoftBody(body_, (short)collisionLayer_, (short)collisionMask_);
					inWorld_ = true;
					readdBody_ = false;
					UpdateMass();
					UpdateGravity();
					//body_->forceActivationState(DISABLE_DEACTIVATION);
					//body_->activate(true);
					body_->setMass(0, 0);
				}
			}
		}
	}

	bool SoftBody::CreateSoftBodyFromTriMesh(VertexBuffer* vb, IndexBuffer* ib, bool randomizeConstraints)
	{
		bool bConstructed = false;

		if (vb && ib)
		{
			btAlignedObjectArray<bool>      chks;
			btAlignedObjectArray<btVector3>   vtx;

			// save vertexbuffer ptr
			VertexBuffer* m_pVertexBuffer = vb;

			// copy vertex buffer
			const unsigned char *pVertexData = (const unsigned char*)m_pVertexBuffer->Lock(0, m_pVertexBuffer->GetVertexCount());

			if (pVertexData)
			{
				unsigned numVertices = m_pVertexBuffer->GetVertexCount();
				unsigned vertexSize = m_pVertexBuffer->GetVertexSize();

				vtx.resize(numVertices);

				// copy the original verts
				for (unsigned i = 0; i < numVertices; ++i)
				{
					const Vector3& src = *reinterpret_cast<const Vector3*>(pVertexData + i * vertexSize);
					vtx[i] = ToBtVector3(src);
				}
				m_pVertexBuffer->Unlock();
			}

			// Create softbody
			btSoftBodyWorldInfo* softBodyWorldInfo_ = GetPhysicsWorld()->GetSoftBodyWorld();
			body_ = new btSoftBody(softBodyWorldInfo_, vtx.size(), &vtx[0], 0);

			// copy indexbuffer
			const unsigned *pIndexData = (const unsigned *)ib->Lock(0, ib->GetIndexCount());
			const unsigned short *pUShortData = (const unsigned short *)pIndexData;
			if (pIndexData)
			{
				unsigned numIndeces = ib->GetIndexCount();
				unsigned indexSize = ib->GetIndexSize();

				int ntriangles = (int)numIndeces / 3;
				int maxidx = 0;
				int i;

				if (indexSize == sizeof(unsigned short))
				{
					for (i = 0; i < (int)numIndeces; ++i)
					{
						unsigned uidx = pUShortData[i];
						maxidx = Max(uidx, maxidx);
					}
				}
				else if (indexSize == sizeof(unsigned))
				{
					for (i = 0; i < (int)numIndeces; ++i)
					{
						unsigned uidx = pIndexData[i];
						maxidx = Max(uidx, maxidx);
					}
				}
				++maxidx;
				chks.resize(maxidx*maxidx, false);

				for (i = 0; i < (int)numIndeces; i += 3)
				{
					int idx[3];
					if (indexSize == sizeof(unsigned short))
					{
						idx[0] = (int)pUShortData[i];
						idx[1] = (int)pUShortData[i + 1];
						idx[2] = (int)pUShortData[i + 2];
					}
					else
					{
						idx[0] = (int)pIndexData[i];
						idx[1] = (int)pIndexData[i + 1];
						idx[2] = (int)pIndexData[i + 2];
					}

					#define IDX(_x_, _y_) ((_y_)*maxidx + (_x_))
					for (int j = 2, k = 0; k < 3; j = k++)
					{
						if (!chks[IDX(idx[j], idx[k])])
						{
							chks[IDX(idx[j], idx[k])] = true;
							chks[IDX(idx[k], idx[j])] = true;
							body_->appendLink(idx[j], idx[k]);
						}
					}
					#undef IDX
					body_->appendFace(idx[0], idx[1], idx[2]);
				}

				ib->Unlock();
			}

			if (randomizeConstraints)
			{
				//body_->randomizeConstraints();
			}

			// straight out of bullet's softbody demo for trimesh
			body_->m_materials[0]->m_kLST = 0.25f;
			body_->m_cfg.kMT = 0.05f;

			body_->m_cfg.piterations = 10;
			body_->m_cfg.viterations = 10;
			/*body_->m_cfg.citerations = 10;
			body_->m_cfg.diterations = 10;
			*/
			//body_->m_cfg.kSRHR_CL = 1;
			//body_->m_cfg.kSR_SPLT_CL = 1;

			//body_->m_cfg.collisions = btSoftBody::fCollision::VF_SS;UpdateMass();

			body_->generateBendingConstraints(2);
			body_->randomizeConstraints();


			btMatrix3x3   m;
			m.setEulerZYX(0, 0, 0);

			// create methods for these
			if (node_)
			{
				SetPosition(node_->GetWorldPosition());

				//body_->transform(btTransform(m, ToBtVector3(node_->GetWorldPosition())));
				//body_->scale(ToBtVector3(node_->GetWorldScale()));
			}
			//UpdateMass();

			body_->setTotalMass(10.0f, true);
			body_->setPose(true, true); // i don't know what this does

			bConstructed = true;
			SubscribeToEvent(E_POSTUPDATE, HANDLER(SoftBody, HandlePostUpdate));if (body_  && physicsWorld_);
		}
	
		return bConstructed;
	}

	bool SoftBody::create(VertexBuffer* vb, IndexBuffer* ib, bool randomizeConstraints)
		{
			bool bConstructed = false;
		
			if (vb && ib)
			{
			const unsigned numVertices = vb->GetVertexCount();
			const unsigned vertexSize = vb->GetVertexSize();
			
			const unsigned numIndexes = ib->GetIndexCount();
			const unsigned indexSize = ib->GetIndexSize();
			
			mDupVertices = new int[numVertices];
			mNewIndexes = new int[numIndexes];
			mDupVerticesCount = 0;
			
			int i;
			int j;
			
			PODVector<Vector3> vertexBuffer;
			vertexBuffer.Resize(numVertices);
			
			// Collect vertexes into buffer
			const unsigned char *pVertexData = (const unsigned char*)vb->Lock(0, vb->GetVertexCount());
			if (pVertexData)
			{
				for (unsigned i = 0; i < numVertices; ++i)
				{
					const Vector3& src = *reinterpret_cast<const Vector3*>(pVertexData + i * vertexSize);
					vertexBuffer[i] = src;
				}
				vb->Unlock();
			}
			
			
			PODVector<int> indexBuffer;
			indexBuffer.Resize(numIndexes);
			// Collect indexes into buffer
			const unsigned char *pIndexData = (const unsigned char*)ib->Lock(0, ib->GetIndexCount());
			if (pIndexData)
			{
				for (unsigned i = 0; i < numIndexes; ++i)
				 {
					int localIndex = -1;
					
					if (indexSize == 4)
					{
						const int& src = *reinterpret_cast<const int*>(pIndexData + i * indexSize);
						localIndex = src;
					}
					else
					{
						const short unsigned& src = *reinterpret_cast<const short unsigned*>(pIndexData + i * indexSize);
						localIndex = src;
					}
					
					indexBuffer[i] = localIndex;
				}
				ib->Unlock();
			}
			
			// Find duplicates		
			for (i = 0; i < numVertices; i++)
			{
				Vector3 v1 = vertexBuffer[i];
				mDupVertices[i] = -1;
				mNewIndexes[i] = i - mDupVerticesCount;
				for (j = 0; j < i; j++)
				{
					Vector3 v2 = vertexBuffer[j];
					if (v1 == v2)
					{
						mDupVertices[i] = j;
						mDupVerticesCount++;
					}
				}
			}
			
			// Build array vertexes w/o dup
			const int newVertexCount = numVertices - mDupVerticesCount;
			//PODVector<btVector3> vertices;
			vertices.Resize(newVertexCount);
			for (i = 0, j = 0; i < numVertices; i++)
			{
				if (mDupVertices[i] == -1) 
				{
					Vector3 v = vertexBuffer[i];
					vertices[j++] = ToBtVector3(v);
					
				}
			}
			
			//PODVector<int> indexes(numIndexes);
			indexes.Resize(numIndexes);
			int idx;
			for (i = 0; i < numIndexes; i++)
			{
				idx = indexBuffer[i];
				int idxDup = mDupVertices[idx];
				indexes[i] = mNewIndexes[idxDup == -1 ? idx : idxDup];
			}
			int ntriangles = numIndexes / 3;
			
			btSoftBodyWorldInfo* softBodyWorldInfo_ = GetPhysicsWorld()->GetSoftBodyWorld();
			body_ = btSoftBodyHelpers::CreateFromTriMesh(*softBodyWorldInfo_, vertices[0], &indexes[0], ntriangles);
			
			btTransform	transforms;
			transforms.setIdentity();
			transforms.setOrigin(ToBtVector3(node_->GetWorldPosition()));
			transforms.setRotation(ToBtQuaternion(node_->GetWorldRotation()));
			body_->transform(transforms);
			body_->setUserPointer(this);
			
			body_->generateBendingConstraints(2);
			body_->randomizeConstraints();
			
			/*
			body_->m_materials[0]->m_kLST = 0.25f;
			body_->m_cfg.kMT = 0.05f;
			body_->m_cfg.piterations = 10;
			body_->m_cfg.viterations = 10;
			*/
			/*
			body_->m_cfg.citerations = 10;
			//body_->m_cfg.diterations = 10;
			*/
			
			//body_->m_cfg.kSRHR_CL = 1;
			//body_->m_cfg.kSR_SPLT_CL = 1;
			
			
			//body_->m_cfg.collisions = btSoftBody::fCollision::VF_SS;
			
			/*
			btMatrix3x3   m;
			m.setEulerZYX(0, 0, 0);
			
			// create methods for these
			if (node_)
			{
				SetPosition(node_->GetWorldPosition());
				
				//body_->transform(btTransform(m, ToBtVector3(node_->GetWorldPosition())));
				//body_->scale(ToBtVector3(node_->GetWorldScale()));
			}
			//UpdateMass();
			*/
			
			body_->setTotalMass(1.0f, true);
			body_->setPose(true, true); // i don't know what this does
			
			bConstructed = true;
			SubscribeToEvent(E_POSTUPDATE, HANDLER(SoftBody, HandlePostUpdate));
		}
		
		return bConstructed;
	}

	void SoftBody::UpdateStaticModelFromSoftBody()
	{
		if (body_  && physicsWorld_)
			//PROFILE(UpdateStaticModelFromSoftBody);

			if (vb_)
			{
				// Update vertex buffer
				unsigned char* pVertexData = (unsigned char*)vb_->Lock(0, vb_->GetVertexCount());
				bb.Clear();
				bb.Merge(node_->GetWorldPosition());
			
				// Copy soft body vertices back into the model vertex buffer
				if (pVertexData)
				{
					unsigned numVertices = vb_->GetVertexCount();
					unsigned vertexSize = vb_->GetVertexSize();
				
					// Copy processed with softbody vertex positions
					for (int i = 0; i < body_->m_nodes.size(); ++i)
					{
						btSoftBody::Node& n = body_->m_nodes[i];
						Vector3& src = *reinterpret_cast<Vector3*>(pVertexData + i * vertexSize);
						src = ToVector3(n.m_x);
						bb.Merge(ToVector3(n.m_x)); // create new bb from processed vertexes
					}
					vb_->Unlock();

					bbmodel->SetBoundingBox(bb);
					StaticModel* sm = node_->GetComponent<StaticModel>();
					if (sm)
					{
						sm->GetModel()->SetBoundingBox(bb);
					}
				}
			}
	}
	
	void SoftBody::HandlePostUpdate(StringHash eventType, VariantMap& eventData)
	{
		UpdateStaticModelFromSoftBody();
	}
}