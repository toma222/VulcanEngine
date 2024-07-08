
#pragma once

#include "Core.hpp"
// #include "kon/events/Event.hpp"
#include "kon/events/ApplicationEvent.hpp"
#include "kon/types/String.hpp"
#include <kon/core/RefCount.hpp>

namespace kon
{
    struct knAPI TypeInfo
    {
        // TypeInfo(const String name, const i32 hash, TypeIngo)
        const String m_name;
        const u32 m_hash;
        const TypeInfo *m_parent;
    };

    class knAPI Object : public RefCount
    {
    public:
        virtual ~Object() = default;

        static const TypeInfo *GetStaticTypeInfo() { return nullptr; }
        virtual const TypeInfo *GetTypeInfo() const = 0;
        virtual const TypeInfo *GetParentTypeInfo() const = 0;

        bool IsInstanceOf(u32 hash);
        bool IsInstanceOf(const TypeInfo *info);

        template<typename T>
        bool IsInstanceOf() { return IsInstanceOf(T::GetStaticTypeInfo()); }

        template<typename T>
        T *Cast() { return (IsInstanceOf(T::GetStaticTypeInfo()) ? static_cast<T*>(this) : nullptr); }

        template<typename T>
        const T *Cast() const { return (IsInstanceOf(T::GetStaticTypeInfo()) ? static_cast<const T*>(this) : nullptr); }
    
        virtual void OnEvent(Event &event) {};
    };

    #define KN_OBJECT(name, parent) public: \
                      using className = parent; \
                      static const TypeInfo *GetStaticTypeInfo() { static const TypeInfo info {String(#name), String::Hash(#name), parent::GetStaticTypeInfo()}; return &info; } \
                      virtual const TypeInfo *GetTypeInfo() const override { return GetStaticTypeInfo(); } \
                      virtual const TypeInfo *GetParentTypeInfo() const override { return parent::GetStaticTypeInfo(); } 
}
