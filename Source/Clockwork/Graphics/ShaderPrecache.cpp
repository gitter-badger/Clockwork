#include "../IO/File.h"
#include "../IO/FileSystem.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GraphicsImpl.h"
#include "../IO/Log.h"
#include "../Graphics/ShaderPrecache.h"
#include "../Graphics/ShaderVariation.h"

#include "../DebugNew.h"

namespace Clockwork
{

ShaderPrecache::ShaderPrecache(Context* context, const String& fileName) :
    Object(context),
    fileName_(fileName),
    xmlFile_(context)
{
    if (GetSubsystem<FileSystem>()->FileExists(fileName))
    {
        // If file exists, read the already listed combinations
        File source(context_, fileName);
        xmlFile_.Load(source);

        XMLElement shader = xmlFile_.GetRoot().GetChild("shader");
        while (shader)
        {
            String oldCombination = shader.GetAttribute("vs") + " " + shader.GetAttribute("vsdefines") + " " +
                shader.GetAttribute("ps") + " " + shader.GetAttribute("psdefines");
            usedCombinations_.Insert(oldCombination);

            shader = shader.GetNext("shader");
        }
    }

    // If no file yet or loading failed, create the root element now
    if (!xmlFile_.GetRoot())
        xmlFile_.CreateRoot("shaders");

    LOGINFO("Begin dumping shaders to " + fileName_);
}

ShaderPrecache::~ShaderPrecache()
{
    LOGINFO("End dumping shaders");

    if (usedCombinations_.Empty())
        return;

    File dest(context_, fileName_, FILE_WRITE);
    xmlFile_.Save(dest);
}

void ShaderPrecache::StoreShaders(ShaderVariation* vs, ShaderVariation* ps)
{
    if (!vs || !ps)
        return;

    // Check for duplicate using pointers first (fast)
    Pair<ShaderVariation*, ShaderVariation*> shaderPair = MakePair(vs, ps);
    if (usedPtrCombinations_.Contains(shaderPair))
        return;
    usedPtrCombinations_.Insert(shaderPair);

    String vsName = vs->GetName();
    String psName = ps->GetName();
    const String& vsDefines = vs->GetDefines();
    const String& psDefines = ps->GetDefines();

    // Check for duplicate using strings (needed for combinations loaded from existing file)
    String newCombination = vsName + " " + vsDefines + " " + psName + " " + psDefines;
    if (usedCombinations_.Contains(newCombination))
        return;
    usedCombinations_.Insert(newCombination);

    XMLElement shaderElem = xmlFile_.GetRoot().CreateChild("shader");
    shaderElem.SetAttribute("vs", vsName);
    shaderElem.SetAttribute("vsdefines", vsDefines);
    shaderElem.SetAttribute("ps", psName);
    shaderElem.SetAttribute("psdefines", psDefines);
}

void ShaderPrecache::LoadShaders(Graphics* graphics, Deserializer& source)
{
    LOGDEBUG("Begin precaching shaders");

    XMLFile xmlFile(graphics->GetContext());
    xmlFile.Load(source);

    XMLElement shader = xmlFile.GetRoot().GetChild("shader");
    while (shader)
    {
        String vsDefines = shader.GetAttribute("vsdefines");
        String psDefines = shader.GetAttribute("psdefines");

        // Check for illegal variations on OpenGL ES and skip them
        #ifdef GL_ES_VERSION_2_0
        if (vsDefines.Contains("INSTANCED") || (psDefines.Contains("POINTLIGHT") && psDefines.Contains("SHADOW")))
        {
            shader = shader.GetNext("shader");
            continue;
        }
        #endif

        ShaderVariation* vs = graphics->GetShader(VS, shader.GetAttribute("vs"), vsDefines);
        ShaderVariation* ps = graphics->GetShader(PS, shader.GetAttribute("ps"), psDefines);
        // Set the shaders active to actually compile them
        graphics->SetShaders(vs, ps);

        shader = shader.GetNext("shader");
    }

    LOGDEBUG("End precaching shaders");
}

}
