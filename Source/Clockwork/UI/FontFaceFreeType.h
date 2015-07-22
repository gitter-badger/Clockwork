

#pragma once

#include "../UI/FontFace.h"

namespace Clockwork
{

class FreeTypeLibrary;
class Texture2D;

/// Free type font face description.
class CLOCKWORK_API FontFaceFreeType : public FontFace
{
public:
    /// Construct.
    FontFaceFreeType(Font* font);
    /// Destruct.
    ~FontFaceFreeType();

    /// Load font face.
    virtual bool Load(const unsigned char* fontData, unsigned fontDataSize, int pointSize);
    /// Return pointer to the glyph structure corresponding to a character. Return null if glyph not found.
    virtual const FontGlyph* GetGlyph(unsigned c);

    /// Return if font face uses mutable glyphs.
    virtual bool HasMutableGlyphs() const { return hasMutableGlyph_; }

private:
    /// Check can load all glyph in one texture, return true and texture size if can load.
    bool CanLoadAllGlyphs(const PODVector<unsigned>& charCodes, int& textureWidth, int& textureHeight) const;
    /// Setup next texture.
    bool SetupNextTexture(int textureWidth, int textureHeight);
    /// Load char glyph.
    bool LoadCharGlyph(unsigned charCode, Image* image = 0);

    /// FreeType library.
    SharedPtr<FreeTypeLibrary> freeType_;
    /// FreeType face. Non-null after creation only in dynamic mode.
    void* face_;
    /// Load mode.
    int loadMode_;
    /// Ascender.
    int ascender_;
    /// Has mutable glyph.
    bool hasMutableGlyph_;
    /// Glyph area allocator.
    AreaAllocator allocator_;
};

}
