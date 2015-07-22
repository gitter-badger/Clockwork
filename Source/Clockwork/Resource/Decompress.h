

#pragma once

#include "../Resource/Image.h"

namespace Clockwork
{

/// Decompress a DXT compressed image to RGBA.
CLOCKWORK_API void
    DecompressImageDXT(unsigned char* dest, const void* blocks, int width, int height, int depth, CompressedFormat format);
/// Decompress an ETC1 compressed image to RGBA.
CLOCKWORK_API void DecompressImageETC(unsigned char* dest, const void* blocks, int width, int height);
/// Decompress a PVRTC compressed image to RGBA.
CLOCKWORK_API void DecompressImagePVRTC(unsigned char* dest, const void* blocks, int width, int height, CompressedFormat format);
/// Flip a compressed block vertically.
CLOCKWORK_API void FlipBlockVertical(unsigned char* dest, unsigned char* src, CompressedFormat format);
/// Flip a compressed block horizontally.
CLOCKWORK_API void FlipBlockHorizontal(unsigned char* dest, unsigned char* src, CompressedFormat format);

}
