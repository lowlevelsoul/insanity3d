//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __I3D_CVAR_H__
#define __I3D_CVAR_H__

#include "i3d/core/Types.h"

namespace i3d {

    class CVar {
        friend class CVarManagerLocal;
    public:
        static CVar * INIT_LIST_HEAD;
        static CVar * INIT_LIST_TAIL;

        enum TYPE {
            TYPE_INT = 0,
            TYPE_FLOAT,
            TYPE_STRING,
            TYPE_PROXY,
            TYPE_COUNT,
        };

        CVar( const char * name );
        CVar( const char * name, float defaultValue, const char * desc );
        CVar( const char * name, bool defaultValue, const char * desc );
        CVar( const char * name, int32_t defaultValue, const char * desc );
        CVar( const char * name, const char * defaultValue, const char * desc );

        ~CVar();

        void Set( const char * value );

        void Set( int32_t value );

        void Set( float value );

        void Set( bool value );

        bool GetBool() const;

        int32_t GetInt() const;

        float GetFloat() const;

        const char * GetString() const;

    protected:

        void AddToInit();

    protected:
        union DefaultValue {
            const char * m_string;
            float m_float;
            int32_t m_int;
        };

        union Value {
            char * m_string;
            float m_float;
            int32_t m_int;
            uint64_t m_blah;
        };

        const char *		m_name;				///< The cvar name
        const char *		m_desc;				///< Description / documentation for the cvar
        TYPE				m_type;				///< CVar type
        DefaultValue		m_defaultValue;		///< Default value for the cvar
        Value				m_value;			///< Current, actual value for the cvar
        CVar *				m_initNext;			///< Next cvar in the init list
        CVar *				m_original;			///< For cvars that are essentialy proxies - points to the original cvar


    };

    class CVarManager {
        friend class CVar;
    public:
        virtual ~CVarManager() {
        }

        virtual CVar * Find( const char * name ) = 0;

        virtual void PublishCVars( CVar * list ) = 0;

        void SetCVar( const char * name, bool value );

        void SetCVar( const char * name, int32_t value );

        void SetCVar( const char * name, float value );

        void SetCVar( const char * name, const char * value );

    protected:
        virtual void * Alloc( size_t size ) = 0;

        virtual void Free( void * mem ) = 0;
    };

    //==========================================================================================================================================
    inline void CVarManager::SetCVar( const char * name, bool value ) {
        CVar * var = Find( name );
        if ( var != nullptr ) {
            var->Set( value );
        }
    }

    //==========================================================================================================================================
    inline void CVarManager::SetCVar( const char * name, int32_t value ) {
        CVar * var = Find( name );
        if ( var != nullptr ) {
            var->Set( value );
        }
    }

    //==========================================================================================================================================
    inline void CVarManager::SetCVar( const char * name, float value ) {
        CVar * var = Find( name );
        if ( var != nullptr ) {
            var->Set( value );
        }
    }

    //==========================================================================================================================================
    inline void CVarManager::SetCVar( const char * name, const char * value ) {
        CVar * var = Find( name );
        if ( var != nullptr ) {
            var->Set( value );
        }
    }
}

extern i3d::CVarManager * cvars;

#endif
