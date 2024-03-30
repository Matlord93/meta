#ifndef _INCLUDE_METAMOD_SOURCE_SUPPORT_H_
#define _INCLUDE_METAMOD_SOURCE_SUPPORT_H_

#include <cstdint>
#include <string>

// Forward declaration
template<typename T>
class MetamodSourceConVar;

// Enumeration for ConVar types
enum EMetaConVarType : int16_t
{
    EMetaConVarType_Invalid = -1,
    EMetaConVarType_Bool,
    EMetaConVarType_Int16,
    EMetaConVarType_UInt16,
    EMetaConVarType_Int32,
    EMetaConVarType_UInt32,
    EMetaConVarType_Int64,
    EMetaConVarType_UInt64,
    EMetaConVarType_Float32,
    EMetaConVarType_Float64,
    EMetaConVarType_String,
    EMetaConVarType_Color,
    EMetaConVarType_Vector2,
    EMetaConVarType_Vector3,
    EMetaConVarType_Vector4,
    EMetaConVarType_Qangle,
    EMetaConVarType_MAX
};

// Template function to translate ConVar types
template<typename T>
constexpr EMetaConVarType TranslateMetaConVarType();

// Structure for ConVar creation information
struct MetamodConVarCreation_t
{
    std::string m_name;
    std::string m_help;
    int32_t m_flags;
    EMetaConVarType m_type;
    uint8_t m_defaultValue[sizeof(CVValue_t)];
    uint8_t m_minValue[sizeof(CVValue_t)];
    uint8_t m_maxValue[sizeof(CVValue_t)];
    bool m_hasDefault;
    bool m_hasMin;
    bool m_hasMax;
    void* m_changeCallback;
    void** m_conVar;

    template<typename T>
    MetamodConVarCreation_t(const char* name, int32_t flags, const char* help, const T& value, void* cb = nullptr) :
        m_name(name),
        m_help(help),
        m_flags(flags),
        m_type(TranslateMetaConVarType<T>()),
        m_hasDefault(true),
        m_changeCallback(cb)
    {
        DefaultValue<T>() = value;
    }

    template<typename T>
    MetamodConVarCreation_t(const char* name, int32_t flags, const char* help, const T& value, bool min, const T& minValue, bool max, const T& maxValue, void* cb = nullptr) :
        m_name(name),
        m_help(help),
        m_flags(flags),
        m_type(TranslateMetaConVarType<T>()),
        m_hasDefault(true),
        m_hasMin(min),
        m_hasMax(max),
        m_changeCallback(cb)
    {
        DefaultValue<T>() = value;
        MinValue<T>() = minValue;
        MaxValue<T>() = maxValue;
    }
};

// Forward declaration for callback function type
#if defined META_IS_SOURCE2
template<typename T>
using FnConVarChangeCallback_t = void(*)(ConVar<T>* ref, const CSplitScreenSlot nSlot, const T* pNewValue, const T* pOldValue);
#else
using FnConVarChangeCallback_t = void(*)(IConVar* var, const char* pOldValue, float flOldValue);
#endif

// Class for managing ConVars
template<typename T>
class MetamodSourceConVar {
private:
    // Member variables and methods
public:
    // Constructors
    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, FnConVarChangeCallback_t<T> cb = nullptr);
    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, bool min, const T& minValue, bool max, const T& maxValue, FnConVarChangeCallback_t<T> cb = nullptr);

    // Methods for getting and setting values
    const T GetValue() const;
    const T GetDefaultValue() const;
    const T GetMinValue() const;
    const T GetMaxValue() const;
    void SetDefaultValue(const T& value);
    void SetMinValue(const T& value);
    void SetMaxValue(const T& value);
    void RemoveDefaultValue();
    void RemoveMinValue();
    void RemoveMaxValue();
    void GetStringValue(char* dst, size_t len, int index = 0) const;
    void GetStringDefaultValue(char* dst, size_t len) const;
    void GetStringMinValue(char* dst, size_t len) const;
    void GetStringMaxValue(char* dst, size_t len) const;
};

#endif //_INCLUDE_METAMOD_SOURCE_SUPPORT_H_
