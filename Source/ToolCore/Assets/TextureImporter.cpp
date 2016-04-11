//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
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

#include <Clockwork/Resource/ResourceCache.h>
#include <Clockwork/Resource/Image.h>
#include <Clockwork/Clockwork2D/Sprite2D.h>
#include <Clockwork/Clockwork2D/StaticSprite2D.h>

#include "Asset.h"
#include "AssetDatabase.h"
#include "TextureImporter.h"

namespace ToolCore
{

TextureImporter::TextureImporter(Context* context, Asset *asset) : AssetImporter(context, asset)
{

}

TextureImporter::~TextureImporter()
{

}

void TextureImporter::SetDefaults()
{
    AssetImporter::SetDefaults();
}

bool TextureImporter::Import()
{
    AssetDatabase* db = GetSubsystem<AssetDatabase>();

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    SharedPtr<Image> image = cache->GetTempResource<Image>(asset_->GetPath());

    if (image.Null())
        return false;

    // todo, proper proportions
    image->Resize(64, 64);

    String cachePath = db->GetCachePath();

    // not sure entirely what we want to do here, though if the cache file doesn't exist
    // will reimport
    image->SavePNG(cachePath + asset_->GetGUID());

    // save thumbnail
    image->SavePNG(cachePath + asset_->GetGUID() + "_thumbnail.png");

    return true;
}

bool TextureImporter::LoadSettingsInternal(JSONValue& jsonRoot)
{
    if (!AssetImporter::LoadSettingsInternal(jsonRoot))
        return false;

    JSONValue import = jsonRoot.Get("TextureImporter");

    return true;
}

bool TextureImporter::SaveSettingsInternal(JSONValue& jsonRoot)
{
    if (!AssetImporter::SaveSettingsInternal(jsonRoot))
        return false;

    JSONValue import(JSONValue::emptyObject);
    jsonRoot.Set("TextureImporter", import);

    return true;
}

Resource* TextureImporter::GetResource(const String& typeName)
{
    if (!typeName.Length())
        return 0;

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    return cache->GetResource(typeName, asset_->GetPath());

}

Node* TextureImporter::InstantiateNode(Node* parent, const String& name)
{

    Node* node = parent->CreateChild(name);

    Sprite2D* spriteGraphic = GetSubsystem<ResourceCache>()->GetResource<Sprite2D>(asset_->GetPath());

    StaticSprite2D* sprite = node->CreateComponent<StaticSprite2D>();

    sprite->SetSprite(spriteGraphic);

    return node;

}



}
