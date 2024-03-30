#pragma once

template<> constexpr EMetaConVarType TranslateMetaConVarType<bool>( void )		{ return EMetaConVarType_Bool; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int16_t>( void )	{ return EMetaConVarType_Int16; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint16_t>( void )	{ return EMetaConVarType_UInt16; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int32_t>( void )	{ return EMetaConVarType_Int32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint32_t>( void )	{ return EMetaConVarType_UInt32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<int64_t>( void )	{ return EMetaConVarType_Int64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<uint64_t>( void )	{ return EMetaConVarType_UInt64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<float>( void )		{ return EMetaConVarType_Float32; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<double>( void )	{ return EMetaConVarType_Float64; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<const char*>( void ){ return EMetaConVarType_String; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Color>( void )		{ return EMetaConVarType_Color; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector2D>( void )	{ return EMetaConVarType_Vector2; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector>( void )	{ return EMetaConVarType_Vector3; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<Vector4D>( void )	{ return EMetaConVarType_Vector4; }
template<> constexpr EMetaConVarType TranslateMetaConVarType<QAngle>( void )	{ return EMetaConVarType_Qangle; }

template<typename T>
MetamodSourceConVar<T>::MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, FnConVarChangeCallback_t cb) :
    m_ConVar(nullptr)
{
    MetamodConVarCreation_t create(name, flags, help, value, reinterpret_cast<void*>(cb));
    m_ConVar = new ConVar<T>(create);
}

template<typename T>
MetamodSourceConVar<T>::MetamodSourceConVar(const char* name, int32_t flags, const char* help, const T& value, bool min, const T& minValue, bool max, const T& maxValue, FnConVarChangeCallback_t cb) :
    m_ConVar(nullptr)
{
    MetamodConVarCreation_t create(name, flags, help, value, min, minValue, max, maxValue, reinterpret_cast<void*>(cb));
    m_ConVar = new ConVar<T>(create);
}

#if defined META_IS_SOURCE2
// Implementierungen für Source-2-Plattform
template<>
class MetamodSourceConVar<const char *> {
private:
    using FnConVarChangeCallback_t = void(*)(IConVar* var, const char* pOldValue, float flOldValue);
    ConVar<const char *>* m_ConVar;

public:
    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const char* value, FnConVarChangeCallback_t cb = nullptr) :
        m_ConVar(nullptr) {
        MetamodConVarCreation_t create(name, flags, help, value, reinterpret_cast<void*>(cb));
        m_ConVar = new ConVar<const char *>(create);
    }

    MetamodSourceConVar(const char* name, int32_t flags, const char* help, const char* value, bool min, const char* minValue, bool max, const char* maxValue, FnConVarChangeCallback_t cb = nullptr) :
        m_ConVar(nullptr) {
        MetamodConVarCreation_t create(name, flags, help, value, min, minValue, max, maxValue, reinterpret_cast<void*>(cb));
        m_ConVar = new ConVar<const char *>(create);
    }

    ~MetamodSourceConVar() {
        delete m_ConVar;
    }

    inline const char* GetValue() const {
        return m_ConVar->GetValue();
    }

    inline const char* GetDefaultValue() const {
        return m_ConVar->GetDefaultValue();
    }

    inline const char* GetMinValue() const {
        return m_ConVar->GetMinValue();
    }

    inline const char* GetMaxValue() const {
        return m_ConVar->GetMaxValue();
    }

    inline void SetDefaultValue(const char* value) {
        m_ConVar->SetDefaultValue(value);
    }

    inline void SetMinValue(const char* value) {
        m_ConVar->SetMinValue(value);
    }

    inline void SetMaxValue(const char* value) {
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

    inline void GetStringValue(char* dst, size_t len, int index = 0) const {
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
};
#else
// Fehlende Implementierungen für andere Plattformen
#endif
