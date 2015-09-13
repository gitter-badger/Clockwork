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

#pragma once

#include "../IO/VectorBuffer.h"
#include "../Scene/Component.h"
#include "../Core/Context.h"

#include <Bullet/LinearMath/btMotionState.h>

class btCompoundShape;
class btSoftBody;

namespace Clockwork
{

	class CollisionShape;
	class Constraint;
	class PhysicsWorld;
	class SmoothedTransform;
	class Model;
	class VertexBuffer;
	class IndexBuffer;
	
	/// Physics rigid body component.
	class CLOCKWORK_API SoftBody : public Component, public btMotionState
	{
		OBJECT(SoftBody);
		
	public:
		/// Construct.
		SoftBody(Context* context);
		/// Destruct. Free the rigid body and geometries.
		virtual ~SoftBody();
		/// Register object factory.
		static void RegisterObject(Context* context);
		
		/// Handle attribute write access.
		virtual void OnSetAttribute(const AttributeInfo& attr, const Variant& src);
		/// Apply attribute changes that can not be applied immediately. Called after scene load or a network update.
		virtual void ApplyAttributes();
		/// Handle enabled/disabled state change.
		virtual void OnSetEnabled();
		/// Return initial world transform to Bullet.
		virtual void getWorldTransform(btTransform& worldTrans) const;
		/// Update world transform from Bullet.
		virtual void setWorldTransform(const btTransform& worldTrans);
		/// Visualize the component as debug geometry.
		virtual void DrawDebugGeometry(DebugRenderer* debug, bool depthTest);
		
		/// Set mass. Zero mass makes the body static.
		void SetMass(float mass);
		/// Set rigid body position in world space.
		void SetPosition(const Vector3& position);
		/// Set rigid body rotation in world space.
		void SetRotation(const Quaternion& rotation);
		/// Set rigid body position and rotation in world space as an atomic operation.
		void SetTransform(const Vector3& position, const Quaternion& rotation);
		
		void SetUseGravity(bool enable);
		/// Set gravity override. If zero, uses physics world's gravity.
		void SetGravityOverride(const Vector3& gravity);
		/// Set rigid body kinematic mode. In kinematic mode forces are not applied to the rigid body.
		void SetCollisionLayer(unsigned layer);
		/// Set collision mask.
		void SetCollisionMask(unsigned mask);
		/// Set collision group and mask.
		void SetCollisionLayerAndMask(unsigned layer, unsigned mask);
		/// Set collision event signaling mode. Default is to signal when rigid bodies are active.
		//void SetCollisionEventMode(CollisionEventMode mode);
		
		/// Activate rigid body if it was resting.
		void Activate();
		/// Readd rigid body to the physics world to clean up internal state like stale contacts.
		void ReAddBodyToWorld();
		/// Disable mass update. Call this to optimize performance when adding or editing multiple collision shapes in the same node.
		void DisableMassUpdate();
		/// Re-enable mass update and recalculate the mass/inertia by calling UpdateMass(). Call when collision shape changes are finished.
		void EnableMassUpdate();
		
		/// Return physics world.
		PhysicsWorld* GetPhysicsWorld() const { return physicsWorld_; }
		
		/// Return Bullet soft body.
		btSoftBody* GetBody() const { return body_; }
		
		/// Return mass.
		float GetMass() const { return mass_; }
		
		/// Return rigid body position in world space.
		Vector3 GetPosition() const;
		/// Return rigid body rotation in world space.
		Quaternion GetRotation() const;
		
		/// Return whether rigid body uses gravity.
		bool GetUseGravity() const { return useGravity_; }
		
		/// Return gravity override. If zero (default), uses the physics world's gravity.
		const Vector3& GetGravityOverride() const { return gravityOverride_; }
		
		/// Return center of mass offset.
		const Vector3& GetCenterOfMass() const { return centerOfMass_; }
		
		/// Return whether rigid body is active (not sleeping.)
		bool IsActive() const;
		
		/// Return collision layer.
		unsigned GetCollisionLayer() const { return collisionLayer_; }
		
		/// Return collision mask.
		unsigned GetCollisionMask() const { return collisionMask_; }
		
		/// Apply new world transform after a simulation step. Called internally.
		void ApplyWorldTransform(const Vector3& newWorldPosition, const Quaternion& newWorldRotation);
		/// Update mass and inertia to the Bullet rigid body.
		void UpdateMass();
		/// Update gravity parameters to the Bullet rigid body.
		void UpdateGravity();
		
		void ReleaseBody();
		
		void UpdateSoftBodyFromStaticModel();
		
	protected:
		/// Handle node being assigned.
		virtual void OnNodeSet(Node* node);
		/// Handle scene being assigned.
		virtual void OnSceneSet(Scene* scene);
		/// Handle node transform being dirtied.
		virtual void OnMarkedDirty(Node* node);
		
	private:
		/// Create the soft body, or re-add to the physics world with changed flags. Calls UpdateMass().
		void AddBodyToWorld();
		/// Remove the soft body from the physics world.
		void RemoveBodyFromWorld();
		
		/// Bullet soft body.
		btSoftBody* body_;
		/// Physics world.
		WeakPtr<PhysicsWorld> physicsWorld_;
		/// Smoothed transform, if has one.
		WeakPtr<SmoothedTransform> smoothedTransform_;
		/// Gravity override vector.
		Vector3 gravityOverride_;
		/// Center of mass offset.
		Vector3 centerOfMass_;
		/// Mass.
		float mass_;
		/// Attribute buffer for network replication.
		mutable VectorBuffer attrBuffer_;
		/// Collision layer.
		unsigned collisionLayer_;
		/// Collision mask.
		unsigned collisionMask_;
		/// Last interpolated position from the simulation.
		mutable Vector3 lastPosition_;
		/// Last interpolated rotation from the simulation.
		mutable Quaternion lastRotation_;
		/// Use gravity flag.
		bool useGravity_;
		/// Readd body to world flag.
		bool readdBody_;
		/// Body exists in world flag.
		bool inWorld_;
		/// Mass update enable flag.
		bool enableMassUpdate_;
		
		bool CreateSoftBodyFromTriMesh(VertexBuffer* vb, IndexBuffer* ib, bool randomizeConstraints);
		bool create(VertexBuffer* vb, IndexBuffer* ib, bool randomizeConstraints);
		void UpdateStaticModelFromSoftBody();
		void HandlePostUpdate(StringHash eventType, VariantMap& eventData);
		//void CreateTriMesh();
		
		VertexBuffer* vb_;
		BoundingBox bb;
		WeakPtr<Model> bbmodel;

		int *mDupVertices;
		int *mNewIndexes;
		int mDupVerticesCount;
		PODVector<btVector3> vertices;
		PODVector<int> indexes;
	};
	
}