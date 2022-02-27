//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTIOBJECT_H__
#define __I3D_RTTIOBJECT_H__

#include "i3d/rtti/RttiType.h"

namespace i3d {

    class RttiUnknown {
    public:
        virtual ~RttiUnknown() {
        }
        
        static RttiType* GetClassType() { return nullptr; }
        
        virtual RttiType* GetType() { return nullptr; }
    };

    template<class __object__>
    class RttiRefPointer {
    public:
        RttiRefPointer() {
            m_object = nullptr;
        }

        RttiRefPointer( const RttiRefPointer& rhs ) {
            m_object = nullptr;
            SetObject( rhs.m_object );
        }

        RttiRefPointer( __object__* rhs ) {
            m_object = nullptr;
            SetObject( rhs );
        }

        ~RttiRefPointer() {
            SetObject( nullptr );
        }

        void SetObject(__object__* rhs) {
            if (m_object != nullptr) {
                m_object->DecRef();
                m_object = nullptr;
            }

            if (rhs != nullptr) {
                m_object = rhs;
                m_object->IncRef();
            }
        }

        RttiRefPointer& operator=(const RttiRefPointer& rhs) {
            SetObject( rhs.m_object );
            return *this;
        }

        RttiRefPointer& operator=( __object__* rhs ) {
            SetObject( rhs );
            return *this;
        }

        __object__* Get() { return m_object;  }
        const __object__* Get() const { return m_object; }

        operator __object__*() { return m_object;  }
        operator const __object__*() const { return m_object; }
        __object__* operator->() { return m_object;  }
        const __object__* operator->() const { return m_object; }

        bool operator!() const { return m_object == nullptr; }
        bool operator==( const __object__* rhs ) const { return m_object == rhs; }
        bool operator!=(const __object__* rhs) const { return m_object != rhs; }
        bool operator<=( const __object__* rhs ) const { return m_object <= rhs; }
        bool operator>=( const __object__* rhs ) const { return m_object >= rhs; }
        bool operator<( const __object__* rhs ) const { return m_object < rhs; }
        bool operator>( const __object__* rhs ) const { return m_object > rhs; }

    protected:
        __object__*		m_object;
    };


    class RttiObject : public RttiUnknown {
        RTTI_CLASS_DECLARE(RttiObject, RttiUnknown)
    public:
        RttiObject();
        
        virtual ~RttiObject();
        
        template<typename __type__>
        __type__ * GetPropValue(const char* name, int32_t element = 0) {
            const RttiPropDef* propDef = GetType()->FindProp(name);
            return GetPropValue<__type__>(propDef, element);
        }
        
        template<typename __type__>
        __type__ * GetPropValue(const RttiPropDef* propDef, int32_t element = 0) {
            uintptr_t baseAddr = ((uintptr_t) this) + propDef->m_offset;
            return &((__type__*)baseAddr)[element];
        }
        
        template<typename __type__>
        bool IsOfType() {
            return GetType()->IsOfType(__type__::GetClassType());
        }
        
        template<typename __type__>
        __type__* SafeCast() {
            assert(IsOfType<__type__>() == true);
            return (__type__*) this;
        }
        
        virtual bool ReadComplete( const char * path );

        void IncRef() {
            ++m_refCount;
        }

        void DecRef() {
            --m_refCount;
            if ( m_refCount == 0 ) {
                delete this;
            }
        }
        
        bool WerePropsRead() const { return m_propsWereRead == 1; }
        
        void SetPropsWereRead( bool _value ) { m_propsWereRead = (_value == true) ? 1 : 0; }

    protected:
        uint64_t        m_refCount;
        uint64_t        m_propsWereRead : 1;
    };
}


#endif
