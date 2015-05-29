#include <Clockwork/Clockwork.h>

#include <Clockwork/Container/ArrayPtr.h>
#include <Clockwork/Math/MathDefs.h>
#include <Clockwork/Core/ProcessUtils.h>
#include <Clockwork/Core/StringUtils.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

#include <Clockwork/DebugNew.h>

using namespace Clockwork;

int main(int argc, char** argv);
void Run(const Vector<String>& arguments);

int main(int argc, char** argv)
{
    Vector<String> arguments;

    #ifdef WIN32
    arguments = ParseArguments(GetCommandLineW());
    #else
    arguments = ParseArguments(argc, argv);
    #endif

    Run(arguments);
    return 0;
}

void Run(const Vector<String>& arguments)
{
    if (arguments.Size() < 3)
        ErrorExit("Usage: RampGenerator <output png file> <width> <power> [dimensions]\n");

    int width = ToInt(arguments[1]);
    float power = ToFloat(arguments[2]);

    int dimensions = 1;
    if (arguments.Size() > 3)
        dimensions = ToInt(arguments[3]);

    if (width < 2)
        ErrorExit("Width must be at least 2");

    if (dimensions < 1 || dimensions > 2)
        ErrorExit("Dimensions must be 1 or 2");

    if (dimensions == 1)
    {
        SharedArrayPtr<unsigned char> data(new unsigned char[width]);

        for (int i = 0; i < width; ++i)
        {
            float x = ((float)i) / ((float)(width - 1));

            data[i] = (unsigned char)((1.0f - pow(x, power)) * 255.0f);
        }

        // Ensure start is full bright & end is completely black
        data[0] = 255;
        data[width - 1] = 0;

        stbi_write_png(arguments[0].CString(), width, 1, 1, data.Get(), 0);
    }

    if (dimensions == 2)
    {
        SharedArrayPtr<unsigned char> data(new unsigned char[width * width]);

        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                unsigned i = y * width + x;

                float halfWidth = width * 0.5f;
                float xf = (x - halfWidth + 0.5f) / (halfWidth - 0.5f);
                float yf = (y - halfWidth + 0.5f) / (halfWidth - 0.5f);
                float dist = sqrtf(xf * xf + yf * yf);
                if (dist > 1.0f)
                    dist = 1.0f;

                data[i] = (unsigned char)((1.0f - pow(dist, power)) * 255.0f);
            }
        }

        // Ensure the border is completely black
        for (int x = 0; x < width; ++x)
        {
            data[x] = 0;
            data[(width - 1) * width + x] = 0;
            data[x * width] = 0;
            data[x * width + (width - 1)] = 0;
        }

        stbi_write_png(arguments[0].CString(), width, width, 1, data.Get(), 0);
    }
}
