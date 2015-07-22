

#pragma once

#include "../Container/ArrayPtr.h"
#include "../Container/List.h"
#include "../Math/AreaAllocator.h"

namespace Clockwork
{

class Font;
class Image;
class Texture2D;

/// %Font glyph description.
struct FontGlyph
{
    /// Construct.
    FontGlyph();

    /// X position in texture.
    short x_;
    /// Y position in texture.
    short y_;
    /// Width.
    short width_;
    /// Height.
    short height_;
    /// Glyph X offset from origin.
    short offsetX_;
    /// Glyph Y offset from origin.
    short offsetY_;
    /// Horizontal advance.
    short advanceX_;
    /// Texture page. M_MAX_UNSIGNED if not yet resident on any texture.
    unsigned page_;
    /// Used flag.
    bool used_;
};

/// %Font face description.
class CLOCKWORK_API FontFace : public RefCounted
{
    friend class Font;

public:
    /// Construct.
    FontFace(Font* font);
    /// Destruct.
    ~FontFace();

    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize, int pointSize) = 0;
    /// Return pointer to the glyph structure corresponding to a character. Return null if glyph not found.
    virtual const FontGlyph* GetGlyph(unsigned c);

    /// Return if font face uses mutable glyphs.
    virtual bool HasMutableGlyphs() const { return false; }

    /// Return the kerning for a character and the next character.
    short GetKerning(unsigned c, unsigned d) const;
    /// Return true when one of the texture has a data loss.
    bool IsDataLost() const;

    /// Return point size.
    int GetPointSize() const { return pointSize_; }

    /// Return row height.
    int GetRowHeight() const { return rowHeight_; }

    /// Return textures.
    const Vector<SharedPtr<Texture2D> >& GetTextures() const { return textures_; }

protected:
    friend class FontFaceBitmap;
    /// Create a texture for font rendering.
    SharedPtr<Texture2D> CreateFaceTexture();
    /// Load font face texture from image resource.
    SharedPtr<Texture2D> LoadFaceTexture(SharedPtr<Image> image);

    /// Parent font.
    Font* font_;
    /// Glyph mapping.
    HashMap<unsigned, FontGlyph> glyphMapping_;
    /// Kerning mapping.
    HashMap<unsigned, short> kerningMapping_;
    /// Glyph texture pages.
    Vector<SharedPtr<Texture2D> > textures_;
    /// Point size.
    int pointSize_;
    /// Row height.
    int rowHeight_;
};

}
