

#include "../Precompiled.h"

#include "../Resource/Localization.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/JSONFile.h"
#include "../Resource/ResourceEvents.h"
#include "../IO/Log.h"

#include "../DebugNew.h"

namespace Clockwork
{

Localization::Localization(Context* context) :
    Object(context),
    languageIndex_(-1)
{
}

Localization::~Localization()
{
}

int Localization::GetLanguageIndex(const String &language)
{
    if (language.Empty())
    {
        LOGWARNING("Localization::GetLanguageIndex(language): language name is empty");
        return -1;
    }
    if (GetNumLanguages() == 0)
    {
        LOGWARNING("Localization::GetLanguageIndex(language): no loaded languages");
        return -1;
    }
    for (int i = 0; i < GetNumLanguages(); i++)
    {
        if (languages_[i] == language)
            return i;
    }
    return -1;
}

String Localization::GetLanguage()
{
    if (languageIndex_ == -1)
    {
        LOGWARNING("Localization::GetLanguage(): no loaded languages");
        return String::EMPTY;
    }
    return languages_[languageIndex_];
}

String Localization::GetLanguage(int index)
{
    if (GetNumLanguages() == 0)
    {
        LOGWARNING("Localization::GetLanguage(index): no loaded languages");
        return String::EMPTY;
    }
    if (index < 0 || index >= GetNumLanguages())
    {
        LOGWARNING("Localization::GetLanguage(index): index out of range");
        return String::EMPTY;
    }
    return languages_[index];
}

void Localization::SetLanguage(int index)
{
    if (GetNumLanguages() == 0)
    {
        LOGWARNING("Localization::SetLanguage(index): no loaded languages");
        return;
    }
    if (index < 0 || index >= GetNumLanguages())
    {
        LOGWARNING("Localization::SetLanguage(index): index out of range");
        return;
    }
    if (index != languageIndex_)
    {
        languageIndex_ = index;
        VariantMap& eventData = GetEventDataMap();
        SendEvent(E_CHANGELANGUAGE, eventData);
    }
}

void Localization::SetLanguage(const String &language)
{
    if (language.Empty())
    {
        LOGWARNING("Localization::SetLanguage(language): language name is empty");
        return;
    }
    if (GetNumLanguages() == 0)
    {
        LOGWARNING("Localization::SetLanguage(language): no loaded languages");
        return;
    }
    int index = GetLanguageIndex(language);
    if (index == -1)
    {
        LOGWARNING("Localization::SetLanguage(language): language not found");
        return;
    }
    SetLanguage(index);
}

String Localization::Get(const String &id)
{
    if (id.Empty())
        return String::EMPTY;
    if (GetNumLanguages() == 0)
    {
        LOGWARNING("Localization::Get(id): no loaded languages");
        return id;
    }
    String result = strings_[StringHash(GetLanguage())][StringHash(id)];
    if (result.Empty())
    {
        LOGWARNING("Localization::Get(\"" + id + "\") not found translation, language=\"" + GetLanguage() + "\"");
        return id;
    }
    return result;
}

void Localization::Reset()
{
    languages_.Clear();
    languageIndex_ = -1;
    strings_.Clear();
}

void Localization::LoadJSON(const JSONValue &source)
{
    Vector<String> ids = source.GetChildNames();
    for (unsigned i = 0; i < ids.Size(); i++)
    {
        String id = ids[i];
        if (id.Empty())
        {
            LOGWARNING("Localization::LoadJSON(source): string ID is empty");
            continue;
        }
        JSONValue value = source.GetChild(id);
        Vector<String> langs = value.GetValueNames();
        for (unsigned j = 0; j < langs.Size(); j++)
        {
            String lang = langs[j];
            if (lang.Empty())
            {
                LOGWARNING("Localization::LoadJSON(source): language name is empty, string ID=\"" + id + "\"");
                continue;
            }
            String string = value.GetString(lang);
            if (string.Empty())
            {
                LOGWARNING("Localization::LoadJSON(source): translation is empty, string ID=\"" + id +
                           "\", language=\"" + lang + "\"");
                continue;
            }
            if (strings_[StringHash(lang)][StringHash(id)] != String::EMPTY)
            {
                LOGWARNING("Localization::LoadJSON(source): override translation, string ID=\"" + id +
                           "\", language=\"" + lang + "\"");
            }
            strings_[StringHash(lang)][StringHash(id)] = string;
            if (!languages_.Contains(lang))
                languages_.Push(lang);
            if (languageIndex_ == -1)
                languageIndex_ = 0;
        }
    }
}

void Localization::LoadJSONFile(const String &name)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    JSONFile* jsonFile = cache->GetResource<JSONFile>(name);
    if (jsonFile)
        LoadJSON(jsonFile->GetRoot());
}

}
