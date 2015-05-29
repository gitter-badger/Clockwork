#pragma once

#include "../UI/FontFace.h"

namespace Clockwork
{

class Image;
class Serializer;

/// Bitmap font face description.
class CLOCKWORK_API FontFaceBitmap : public FontFace
{
public:
    /// Construct.
    FontFaceBitmap(Font* font);
    /// Destruct.
    ~FontFaceBitmap();

    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize, int pointSize);
    /// Load from existed font face, pack used glyphs into smallest texture size and smallest number of texture.
    bool Load(FontFace* fontFace, bool usedGlyphs);
    /// Save as a new bitmap font type in XML format. Return true if successful.
    bool Save(Serializer& dest, int pointSize, const String& indentation = "\t");

private:
    /// Convert graphics format to number of components.
    unsigned ConvertFormatToNumComponents(unsigned format);
    /// Save font face texture as image resource.
    SharedPtr<Image> SaveFaceTexture(Texture2D* texture);
    /// Save font face texture as image file.
    bool SaveFaceTexture(Texture2D* texture, const String& fileName);
    /// Blit.
    void Blit(Image* dest, int x, int y, int width, int height, Image* source, int sourceX, int sourceY, int components);
};

}
