//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/core/Hash.h"
#include "i3d/core/CVar.h"

namespace i3d {

    CVar * CVar::INIT_LIST_HEAD = nullptr;
    CVar * CVar::INIT_LIST_TAIL = nullptr;

    //=======================================================================================================================
    CVar::CVar( const char * name ) {
        m_type = TYPE_PROXY;
        m_name = name;
        m_original = nullptr;
    }

    //=======================================================================================================================
    CVar::CVar( const char * name, float defaultValue, const char * desc ) {
        m_type = TYPE_FLOAT;
        m_name = name;
        m_desc = desc;
        m_defaultValue.m_float = defaultValue;
        m_value.m_blah = 0;
        m_original = nullptr;

        AddToInit();
    }

    //=======================================================================================================================
    CVar::CVar( const char * name, bool defaultValue, const char * desc ) {
        m_type = TYPE_INT;
        m_name = name;
        m_desc = desc;
        m_defaultValue.m_int = (defaultValue == 0) ? false : true;
        m_value.m_blah = 0;
        m_original = nullptr;

        AddToInit();
    }

    //=======================================================================================================================
    CVar::CVar( const char * name, int32_t defaultValue, const char * desc ) {
        m_type = TYPE_INT;
        m_name = name;
        m_desc = desc;
        m_defaultValue.m_int = defaultValue;
        m_value.m_blah = 0;
        m_original = nullptr;

        AddToInit();
    }

    //=======================================================================================================================
    CVar::CVar( const char * name, const char * defaultValue, const char * desc ) {
        m_type = TYPE_STRING;
        m_name = name;
        m_desc = desc;
        m_defaultValue.m_string = defaultValue;
        m_value.m_blah = 0;
        m_original = nullptr;

        AddToInit();
    }

    //=======================================================================================================================
    CVar::~CVar() {
        
    }

    //=======================================================================================================================
    void CVar::AddToInit() {
        if ( INIT_LIST_HEAD == nullptr ) {
            INIT_LIST_HEAD = this;
        }
        else {
            INIT_LIST_TAIL->m_initNext = this;
        }
        INIT_LIST_TAIL = this;
        m_initNext = nullptr;
    }

    //=======================================================================================================================
    void CVar::Set( const char * value ) {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return;
        }

        if ( m_original != nullptr ) {
            m_original->Set( value );
            return;
        }

        switch ( m_type ) {
            case TYPE_INT:
            {
                m_value.m_int = atoi( value );
            }
            break;

            case TYPE_FLOAT:
            {
                m_value.m_float = (float) atof( value );
            }
            break;
            
            case TYPE_STRING:
            {
                if ( m_value.m_string != nullptr ) {
                    cvars->Free( m_value.m_string );
                    m_value.m_string = nullptr;
                }

                if ( value != nullptr ) {
                    size_t len = strlen( value );
                    m_value.m_string = (char *) cvars->Alloc( len + 1 );
                    strcpy( m_value.m_string, value );
                }
            }
            break;
                
            default:
            {
            }
            break;
        }
    }

    //=======================================================================================================================
    void CVar::Set( int32_t value ) {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return;
        }

        if ( m_original != nullptr ) {
            m_original->Set( value );
            return;
        }

        switch ( m_type ) {
            case TYPE_INT:
                {
                    m_value.m_int = value;
                }
                break;

            case TYPE_FLOAT:
                {
                    m_value.m_float = (float) value;
                }
                break;
            
            case TYPE_STRING:
                {
                    /*
                    if ( m_value.m_string != nullptr ) {
                        cvars->Free( m_value.m_string );
                        m_value.m_string = nullptr;
                    }
                    
                    size_t len = sprintf( m_value.m_string, "%i", value );
                    m_value.m_string = (char *) cvars->Alloc( len + 1 ); */
                }
                break;
                
            default:
            {
            }
            break;
            
        }
    }

    //=======================================================================================================================
    void CVar::Set( float value ) {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return;
        }

        if ( m_original != nullptr ) {
            m_original->Set( value );
            return;
        }

        switch ( m_type ) {
            case TYPE_INT:
            {
                m_value.m_int = (int32_t) value;
            }
            break;

            case TYPE_FLOAT:
            {
                m_value.m_float = value;
            }
            break;
            
            case TYPE_STRING:
            {
                /*
                if ( m_value.m_string != nullptr ) {
                    cvars->Free( m_value.m_string );
                    m_value.m_string = nullptr;
                }
                
                size_t len = sprintf( m_value.m_string, "%i", value );
                m_value.m_string = (char *) cvars->Alloc( len + 1 ); */
            }
            break;
                
            default:
            {
            }
            break;
        }
    }

    //=======================================================================================================================
    void CVar::Set( bool value ) {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return;
        }

        if ( m_original != nullptr ) {
            m_original->Set( value );
            return;
        }

        switch ( m_type ) {
            case TYPE_INT:
            {
                m_value.m_int = (value == true) ? 1 : 0;
            }
            break;

            case TYPE_FLOAT:
            {
                m_value.m_float = (value == true) ? 1.0f : 0.0f;
            }
            break;
            
            case TYPE_STRING:
            {
                /*
                if ( m_value.m_string != nullptr ) {
                    cvars->Free( m_value.m_string );
                    m_value.m_string = nullptr;
                }
                
                size_t len = sprintf( m_value.m_string, "%i", value );
                m_value.m_string = (char *) cvars->Alloc( len + 1 ); */
            }
            break;
                
            default:
            {
            }
            break;
        }
    }

    //=======================================================================================================================
    bool CVar::GetBool() const {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return false;
        }

        if ( m_original != nullptr ) {
            return GetBool();
        }

        switch ( m_type ) {
            case TYPE_INT:
                return ( m_value.m_int != 0 );

            case TYPE_FLOAT:
                return ( m_value.m_float != 0 );
            
            case TYPE_STRING:
                return ( atoi( m_value.m_string ) != 0 );
                
            default:
            {
            }
            break;
        }

        return false;
    }

    //=======================================================================================================================
    int32_t CVar::GetInt() const {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return 0;
        }

        if ( m_original != nullptr ) {
            return GetInt();
        }

        switch ( m_type ) {
            case TYPE_INT:
                return m_value.m_int;

            case TYPE_FLOAT:
                return (int32_t) m_value.m_float;
            
            case TYPE_STRING:
                return atoi( m_value.m_string );
                
            default:
            {
            }
            break;
        }

        return 0;
    }

    //=======================================================================================================================
    float CVar::GetFloat() const {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return 0;
        }

        if ( m_original != nullptr ) {
            return GetFloat();
        }

        switch ( m_type ) {
            case TYPE_INT:
                return (float) m_value.m_int;

            case TYPE_FLOAT:
                return m_value.m_float;
            
            case TYPE_STRING:
                return (float) atof( m_value.m_string );
                
            default:
            {
            }
            break;
        }

        return 0;
    }

    //=======================================================================================================================
    const char * CVar::GetString() const {

        if ( m_type == TYPE_PROXY && m_original == nullptr ) {
            return nullptr;
        }

        if ( m_original != nullptr ) {
            return GetString();
        }

        switch ( m_type ) {
            case TYPE_INT:
                return nullptr;

            case TYPE_FLOAT:
                return nullptr;
            
            case TYPE_STRING:
                return m_value.m_string;
                
            default:
            {
            }
            break;
        }

        return nullptr;
    }
}
