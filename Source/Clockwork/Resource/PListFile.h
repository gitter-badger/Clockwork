#pragma once

#include "../Resource/Resource.h"

namespace Clockwork
{

class XMLElement;

/// PList value types.
enum PListValueType
{
    PLVT_NONE = 0,
    PLVT_INT,
    PLVT_BOOL,
    PLVT_FLOAT,
    PLVT_STRING,
    PLVT_VALUEMAP,
    PLVT_VALUEVECTOR,
};

class PListValue;

/// PList value map.
class PListValueMap : public HashMap<String, PListValue>
{
public:
    PListValue& operator [](const String& key);
    const PListValue& operator [](const String& key) const;
};

typedef Vector<PListValue> PListValueVector;

/// PList value.
class CLOCKWORK_API PListValue
{
public:
    // Construct.
    PListValue();
    // Construct from int.
    PListValue(int value);
    // Construct from boolean.
    PListValue(bool value);
    // Construct from float.
    PListValue(float value);
    // Construct from string.
    PListValue(const String& value);
    // Construct from value map.
    PListValue(PListValueMap& valueMap);
    // Construct from value vector.
    PListValue(PListValueVector& valueVector);
    // Construct from another value.
    PListValue(const PListValue& value);
    /// Destruct.
    ~PListValue();

    /// Assign operator.
    PListValue& operator = (const PListValue& rhs);
    /// Return true if is valid.
    operator bool () const { return type_ != PLVT_NONE; }

    /// Set int.
    void SetInt(int value);
    /// Set boolean.
    void SetBool(bool value);
    /// Set float.
    void SetFloat(float value);
    /// Set string.
    void SetString(const String& value);
    /// Set value map.
    void SetValueMap(const PListValueMap& valueMap);
    /// Set value vector.
    void SetValueVector(const PListValueVector& valueVector);

    /// Return type.
    PListValueType GetType() const { return type_; }
    /// Return int.
    int GetInt() const;
    /// Return boolean.
    bool GetBool() const;
    /// Return float.
    float GetFloat() const;
    /// Return string.
    const String& GetString() const;
    /// Return IntRect, for string type.
    IntRect GetIntRect() const;
    /// Return IntVector2, for string type.
    IntVector2 GetIntVector2() const;
    /// Return value map.
    const PListValueMap& GetValueMap() const;
    /// Return value vector.
    const PListValueVector& GetValueVector() const;

    /// Convert to value map (internal use only).
    PListValueMap& ConvertToValueMap();
    /// Convert to value vector (internal use only).
    PListValueVector& ConvertToValueVector();

private:
    /// Reset.
    void Reset();

    /// Type.
    PListValueType type_;
    /// Values.
    union
    {
        int int_;
        bool bool_;
        float float_;
        String* string_;
        PListValueMap* valueMap_;
        PListValueVector* valueVector_;
    };
};

/// Property list (plist).
class CLOCKWORK_API PListFile : public Resource
{
    OBJECT(PListFile);

public:
    /// Construct.
    PListFile(Context* context);
    /// Destruct.
    virtual ~PListFile();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);

    /// Return root.
    const PListValueMap& GetRoot() const { return root_; }

private:
    /// Load dictionary.
    bool LoadDict(PListValueMap& dict, const XMLElement& dictElem);
    /// Load array.
    bool LoadArray(PListValueVector& array, const XMLElement& arrayElem);
    /// Load value.
    bool LoadValue(PListValue& value, XMLElement valueElem);

    /// Root dictionary.
    PListValueMap root_;
};

}
