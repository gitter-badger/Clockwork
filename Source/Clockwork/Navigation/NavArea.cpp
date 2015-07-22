

#include "../Precompiled.h"

#include "../Scene/Component.h"
#include "../Core/Context.h"
#include "../Graphics/DebugRenderer.h"
#include "../IO/Log.h"
#include "../Navigation/NavArea.h"
#include "../Scene/Node.h"
#include "../Container/Str.h"

namespace Clockwork
{
    static const unsigned MAX_NAV_AREA_ID = 255;
    static const Vector3 DEFAULT_BOUNDING_BOX_MIN(-10.0f, -10.0f, -10.0f);
    static const Vector3 DEFAULT_BOUNDING_BOX_MAX(10.0f, 10.0f, 10.0f);
    static const unsigned DEFAULT_MASK_FLAG = 0;
    static const unsigned DEFAULT_AREA_ID = 0;

    extern const char* NAVIGATION_CATEGORY;

    NavArea::NavArea(Context* context) :
        Component(context),
        areaID_(DEFAULT_AREA_ID),
        boundingBox_(DEFAULT_BOUNDING_BOX_MIN, DEFAULT_BOUNDING_BOX_MAX)
    {
    }

    NavArea::~NavArea()
    {
    }
    
    void NavArea::RegisterObject(Context* context)
    {
        context->RegisterFactory<NavArea>(NAVIGATION_CATEGORY);

        COPY_BASE_ATTRIBUTES(Component);
        ATTRIBUTE("Bounding Box Min", Vector3, boundingBox_.min_, DEFAULT_BOUNDING_BOX_MIN, AM_DEFAULT);
        ATTRIBUTE("Bounding Box Max", Vector3, boundingBox_.max_, DEFAULT_BOUNDING_BOX_MAX, AM_DEFAULT);
        ACCESSOR_ATTRIBUTE("Area ID", GetAreaID, SetAreaID, unsigned, DEFAULT_AREA_ID, AM_DEFAULT);
    }

    void NavArea::SetAreaID(unsigned newID)
    {
        if (newID > MAX_NAV_AREA_ID)
            LOGERRORF("NavArea Area ID %u exceeds maximum value of %u", newID, MAX_NAV_AREA_ID);
        areaID_ = (unsigned char)newID;
        MarkNetworkUpdate();
    }

    BoundingBox NavArea::GetWorldBoundingBox() const
    {
        Matrix3x4 mat;
        mat.SetTranslation(node_->GetWorldPosition());
        return boundingBox_.Transformed(mat);
    }

    void NavArea::DrawDebugGeometry(DebugRenderer* debug, bool depthTest) 
    {
        if (debug && IsEnabledEffective())
        {
            Matrix3x4 mat;
            mat.SetTranslation(node_->GetWorldPosition());
            debug->AddBoundingBox(boundingBox_, mat, Color::GREEN, depthTest);
        }
    }
}
