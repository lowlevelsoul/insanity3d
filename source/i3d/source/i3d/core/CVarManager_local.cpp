//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "i3d/core/CVarManager_local.h"

i3d::CVarManager * cvars = nullptr;

namespace i3d {

    //==========================================================================================================================================
    void CVarCreate() {
        if ( cvars != nullptr ) {
            return;
        }

        CVarManagerLocal * cvarsLocal = new CVarManagerLocal;
        cvars = cvarsLocal;

        cvarsLocal->Initialise();
    }

    //==========================================================================================================================================
    void CVarDestroy() {
        if ( cvars == nullptr ) {
            return;
        }

        SAFE_DELETE( cvars );
    }

    //==========================================================================================================================================
    CVarManagerLocal::CVarManagerLocal() {
    }

    //==========================================================================================================================================
    CVarManagerLocal::~CVarManagerLocal() {

    }

    //==========================================================================================================================================
    void CVarManagerLocal::Initialise() {
        
        PublishCVars( CVar::INIT_LIST_HEAD );
    }

    //==========================================================================================================================================
    CVar * CVarManagerLocal::Find( const char * name ) {
        uint64_t hash = fh64::CalcFromString( name );
        cvar_map_t::iterator findIt = m_cvars.find( hash );
        if ( findIt == m_cvars.end() ) {
            return nullptr;
        }
        
        return findIt->second;
    }

    //==========================================================================================================================================
    void CVarManagerLocal::PublishCVars( CVar * list ) {

        for ( CVar * curr = list; curr != nullptr; curr = curr->m_initNext ) {

            if ( curr->m_type == CVar::TYPE_PROXY ) {
                CVar * original = Find( curr->m_name );
                curr->m_original = original;
            }
            else {
                CVar * original = Find( curr->m_name );
                if ( original != nullptr ) {
                    curr->m_type = CVar::TYPE_PROXY;
                    curr->m_original = original;
                }
                else {
                    InitCVar( curr );
                }
            }
        }
    }

    //==========================================================================================================================================
    void CVarManagerLocal::InitCVar( CVar * var ) {

        // Set the default value
        switch ( var->m_type ) {
            case CVar::TYPE_INT:
                var->Set( var->m_defaultValue.m_int );
                break;

            case CVar::TYPE_FLOAT:
                var->Set( var->m_defaultValue.m_float );
                break;

            case CVar::TYPE_STRING:
                var->Set( var->m_defaultValue.m_string );
                break;
            default:
                XE_ASSERT(false);
        }

        uint64_t hash = fh64::CalcFromString( var->m_name );
        m_cvars[ hash ] = var;
    }

    //==========================================================================================================================================
    void * CVarManagerLocal::Alloc( size_t size ) {
        return malloc( size );
    }

    //==========================================================================================================================================
    void CVarManagerLocal::Free( void * mem ) {
        free( mem );
    }
}
