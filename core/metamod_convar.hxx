#ifndef _INCLUDE_METAMOD_SOURCE_SUPPORT_H_
#define _INCLUDE_METAMOD_SOURCE_SUPPORT_H_

#include "metamod_convar.h"

template<> constexpr EMetaConVarType TranslateMetaConVarType<bool>() { return EMetaConVarType_Bool; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int16_t>() { return EMetaConVarType_Int16; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint16_t>() { return EMetaConVarType_UInt16; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int32_t>() { return EMetaConVarType_Int32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint32_t>() { return EMetaConVarType_UInt32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int64_t>() { return EMetaConVarType_Int64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint64_t>() { return EMetaConVarType_UInt64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<float>() { return EMetaConVarType_Float32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<double>() { return EMetaConVarType_Float64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<const char*>() { return EMetaConVarType_String; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Color>() { return EMetaConVarType_Color; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector2D>() { return EMetaConVarType_Vector2; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector>() { return EMetaConVarType_Vector3; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector4D>() { return EMetaConVarType_Vector4; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<QAngle>() { return EMetaConVarType_Qangle; }

template<typename T>
class MetamodSourceConVar {
private:
    // Private Member und Methoden hier

public:
    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, FnConVarChangeCallback_t cb = nullptr) :
        m_ConVar(nullptr) {
        MetamodConVarCreation_t create(name, flags, help, value, reinterpret_cast<void*>(cb));
        // Hier Initialisierung des ConVar
    }

    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, bool min, const T& minValue, bool max, const T& maxValue, FnConVarChangeCallback_t cb = nullptr) :
        m_ConVar(nullptr) {
        MetamodConVarCreation_t create(name, flags, help, value, min, minValue, max, maxValue, reinterpret_cast<void*>(cb));
        // Hier Initialisierung des ConVar
    }

#if defined META_IS_SOURCE2
    inline const T GetValue() const {
        return m_ConVar->GetValue();
    }

    inline const T GetDefaultValue() const {
        return m_ConVar->GetDefaultValue();
    }

    inline const T GetMinValue() const {
        return m_ConVar->GetMinValue();
    }

    inline const T GetMaxValue() const {
        return m_ConVar->GetMaxValue();
    }

    inline void SetDefaultValue(const T& value) {
        m_ConVar->SetDefaultValue(value);
    }

    inline void SetMinValue(const T& value) {
        m_ConVar->SetMinValue(value);
    }

    inline void SetMaxValue(const T& value) {
        m_ConVar->SetMaxValue(value);
    }

    inline void RemoveDefaultValue() {
        m_ConVar->RemoveDefaultValue();
    }

    inline void RemoveMinValue() {
        m_ConVar->RemoveMinValue();
    }

    inline void RemoveMaxValue() {
        m_ConVar->RemoveMaxValue();
    }

    inline void GetStringValue(char* dst, size_t len, int index) const {
        m_ConVar->GetStringValue(dst, len, index);
    }

    inline void GetStringDefaultValue(char* dst, size_t len) const {
        m_ConVar->GetStringDefaultValue(dst, len);
    }

    inline void GetStringMinValue(char* dst, size_t len) const {
        m_ConVar->GetStringMinValue(dst, len);
    }

    inline void GetStringMaxValue(char* dst, size_t len) const {
        m_ConVar->GetStringMaxValue(dst, len);
    }
#else
    // Fehlende Implementierungen für andere
#endif

};

#endif //_INCLUDE_METAMOD_SOURCE_SUPPORT_H_
