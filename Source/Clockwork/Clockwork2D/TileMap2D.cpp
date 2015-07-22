

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Graphics/DebugRenderer.h"
#include "../Resource/ResourceCache.h"
#include "../Scene/Node.h"
#include "../Scene/Scene.h"
#include "../Clockwork2D/TileMap2D.h"
#include "../Clockwork2D/TileMapLayer2D.h"
#include "../Clockwork2D/TmxFile2D.h"

#include "../DebugNew.h"

namespace Clockwork
{

extern const float PIXEL_SIZE;
extern const char* CLOCKWORK2D_CATEGORY;

TileMap2D::TileMap2D(Context* context) :
    Component(context)
{
}

TileMap2D::~TileMap2D()
{
}

void TileMap2D::RegisterObject(Context* context)
{
    context->RegisterFactory<TileMap2D>(CLOCKWORK2D_CATEGORY);

    ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    MIXED_ACCESSOR_ATTRIBUTE("Tmx File", GetTmxFileAttr, SetTmxFileAttr, ResourceRef, ResourceRef(TmxFile2D::GetTypeStatic()),
        AM_DEFAULT);
}

void TileMap2D::DrawDebugGeometry(DebugRenderer* debug, bool depthTest)
{
    const Color& color = Color::RED;
    float mapW = info_.GetMapWidth();
    float mapH = info_.GetMapHeight();

    switch (info_.orientation_)
    {
    case O_ORTHOGONAL:
        debug->AddLine(Vector2(0.0f, 0.0f), Vector2(mapW, 0.0f), color);
        debug->AddLine(Vector2(mapW, 0.0f), Vector2(mapW, mapH), color);
        debug->AddLine(Vector2(mapW, mapH), Vector2(0.0f, mapH), color);
        debug->AddLine(Vector2(0.0f, mapH), Vector2(0.0f, 0.0f), color);
        break;

    case O_ISOMETRIC:
        debug->AddLine(Vector2(0.0f, mapH * 0.5f), Vector2(mapW * 0.5f, 0.0f), color);
        debug->AddLine(Vector2(mapW * 0.5f, 0.0f), Vector2(mapW, mapH * 0.5f), color);
        debug->AddLine(Vector2(mapW, mapH * 0.5f), Vector2(mapW * 0.5f, mapH), color);
        debug->AddLine(Vector2(mapW * 0.5f, mapH), Vector2(0.0f, mapH * 0.5f), color);
        break;

    case O_STAGGERED:
        debug->AddLine(Vector2(0.0f, 0.0f), Vector2(mapW, 0.0f), color);
        debug->AddLine(Vector2(mapW, 0.0f), Vector2(mapW, mapH), color);
        debug->AddLine(Vector2(mapW, mapH), Vector2(0.0f, mapH), color);
        debug->AddLine(Vector2(0.0f, mapH), Vector2(0.0f, 0.0f), color);
        break;
    }

    for (unsigned i = 0; i < layers_.Size(); ++i)
        layers_[i]->DrawDebugGeometry(debug, depthTest);
}

void TileMap2D::DrawDebugGeometry()
{
    Scene* scene = GetScene();
    if (!scene)
        return;

    DebugRenderer* debug = scene->GetComponent<DebugRenderer>();
    if (!debug)
        return;

    DrawDebugGeometry(debug, false);
}

void TileMap2D::SetTmxFile(TmxFile2D* tmxFile)
{
    if (tmxFile == tmxFile_)
        return;

    if (rootNode_)
        rootNode_->RemoveAllChildren();

    layers_.Clear();

    tmxFile_ = tmxFile;
    if (!tmxFile_)
        return;

    info_ = tmxFile_->GetInfo();

    if (!rootNode_)
    {
        rootNode_ = GetNode()->CreateChild("_root_", LOCAL);
        rootNode_->SetTemporary(true);
    }

    unsigned numLayers = tmxFile_->GetNumLayers();
    layers_.Resize(numLayers);

    for (unsigned i = 0; i < numLayers; ++i)
    {
        const TmxLayer2D* tmxLayer = tmxFile_->GetLayer(i);

        Node* layerNode(rootNode_->CreateChild(tmxLayer->GetName(), LOCAL));
        layerNode->SetTemporary(true);

        TileMapLayer2D* layer = layerNode->CreateComponent<TileMapLayer2D>();
        layer->Initialize(this, tmxLayer);
        layer->SetDrawOrder(i * 10);

        layers_[i] = layer;
    }
}

TmxFile2D* TileMap2D::GetTmxFile() const
{
    return tmxFile_;
}

TileMapLayer2D* TileMap2D::GetLayer(unsigned index) const
{
    if (index >= layers_.Size())
        return 0;

    return layers_[index];
}

Vector2 TileMap2D::TileIndexToPosition(int x, int y) const
{
    return info_.TileIndexToPosition(x, y);
}

bool TileMap2D::PositionToTileIndex(int& x, int& y, const Vector2& position) const
{
    return info_.PositionToTileIndex(x, y, position);
}

void TileMap2D::SetTmxFileAttr(const ResourceRef& value)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    SetTmxFile(cache->GetResource<TmxFile2D>(value.name_));
}

ResourceRef TileMap2D::GetTmxFileAttr() const
{
    return GetResourceRef(tmxFile_, TmxFile2D::GetTypeStatic());
}

}
