#pragma once

#include <Clockwork/Core/Object.h>
#include "Clockwork/Container/Vector.h"
#include <Clockwork/IO/File.h>

#include "BuildTypes.h"

using namespace Clockwork;

namespace ClockworkEditor
{

class BuildBase;

class ResourcePackager : public Object
{
    OBJECT(ResourcePackager);

public:

    ResourcePackager(Context* context, BuildBase* buildBase);
    virtual ~ResourcePackager();

    void AddResourceEntry(BuildResourceEntry* entry);

    void GeneratePackage(const String& destFilePath);

private:

    void WriteHeader(File* dest);
    bool WritePackageFile(const String& destFilePath);

    PODVector<BuildResourceEntry*> resourceEntries_;

    WeakPtr<BuildBase> buildBase_;

    unsigned checksum_;

};

}
