//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/rtti/RttiLoader.h"
#include "i3d/rtti/Rtti.h"
#include "i3d/res/Resource.h"

namespace rttiparse {
    bool ParseRttiFile( i3d::File * file, i3d::RttiLoader * loader );
}

namespace i3d {
    class Resource;
    
    CVar rtti_parseLogVerbose( "rtti_parseLogVerbose", false, "Enables verbose parsing and compilation of rtti scripts" );

    //======================================================================================================================
    RttiLoader::RttiLoader() {
        m_object = nullptr;
        m_systemPlatformId = PLATFORM_ID_NONE;
        m_currPlatformId = PLATFORM_ID_NONE;
        m_streamBuilder = nullptr;
    }

    //======================================================================================================================
    RttiLoader::~RttiLoader() {
        
    }

    //======================================================================================================================
    bool RttiLoader::Load( const char * path, i3d::PLATFORM_ID platformId ) {
        
        RttiStream stream;
        LoadAndCompileToStream( stream, path, platformId );

        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Loading from stream\n" );
        }
        
        stream.Construct();
        
        m_object = stream.m_objects.front();

        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Done.\n" );
        }

        return true;
    }
    
    //======================================================================================================================
    bool RttiLoader::Load( File * file, const char * path, i3d::PLATFORM_ID platformId ) {
        
        RttiStream stream;
        LoadAndCompileToStream( stream, file, path, platformId );

        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Loading from stream\n" );
        }
        
        stream.Construct();
        
        m_object = stream.m_objects.front();

        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Done.\n" );
        }

        return true;
    }
    
    //======================================================================================================================
    bool RttiLoader::LoadAndCompileToStream( RttiStream & stream, const char * path, i3d::PLATFORM_ID platformId ) {
        m_systemPlatformId = platformId;
        m_currPlatformId = m_systemPlatformId;
        
        RttiStreamBuilder streamBuilder;
        m_streamBuilder = &streamBuilder;

        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Loading object from %s\n", path );
        }

        File * file = fs->FileOpen( path, "rb" );
        XE_ERROR( file == nullptr, "Error opening %s\n", path );

        bool res = LoadAndCompileToStream( stream, file, path, platformId );
        return res;
    }
    
    //======================================================================================================================
    bool RttiLoader::LoadAndCompileToStream( RttiStream & stream, File * file, const char * path, i3d::PLATFORM_ID platformId ) {
        m_systemPlatformId = platformId;
        m_currPlatformId = m_systemPlatformId;
        
        RttiStreamBuilder streamBuilder;
        m_streamBuilder = &streamBuilder;


        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Parsing RTTI script\n" );
        }
        bool parseOk = rttiparse::ParseRttiFile( file, this );
        XE_ERROR( parseOk == false, "Error parsing file %s\n%s\n", path, GetLog() );
        
        if ( rtti_parseLogVerbose.GetBool() == true ) {
            XE_LOG( "   Building binary stream\n" );
        }
        
        streamBuilder.Build( stream );
        
        return true;
    }

    //======================================================================================================================
    void RttiLoader::Reset() {
        m_object = nullptr;
    }

    //======================================================================================================================
    uint32_t RttiLoader::BeginObject( const RttiParseItem & type ) {
        assert( type.IsLiteralString() == true );

        uint32_t index = m_streamBuilder->BeginObject( type.m_value.m_litString );
        m_streamBuilder->m_currContext.m_objectCreatedIndex = index;
        return index;
    }

    //======================================================================================================================
    void RttiLoader::EndObject() {

        m_streamBuilder->EndObbject();
    }

    //======================================================================================================================
    void RttiLoader::BeginProperty( const RttiParseItem & propName ) {
        
        // Filter out properties by platform type and other parameters.
        if (ShouldHandleProperty() == false ) {
            return;
        }

        // Get property. If it doesn't exist, we'll add a warning to the log.
        const RttiPropDef * pd = GetPropDef( propName.m_value.m_litString );
        if ( pd == nullptr ) {
            PrintWarning( "Property %s not found\n", propName.m_value.m_litString );
            return;
        }

        m_streamBuilder->m_currContext.m_currProp = pd;
        m_streamBuilder->m_currContext.m_arrayCounter = 0;

        uint64_t hash = fh64::CalcFromString( propName.m_value.m_litString );
        m_streamBuilder->WriteValue<uint64_t>( &hash );

        // Write the prop type
        m_streamBuilder->WritePropType( pd->m_type, pd->m_isVector );

        // If the property is an array, write a dummy array size value that we'll fill in after writing all of the
        // array values
        if ( pd->m_isVector != 0 ) {
            // Store the array size offset
            m_streamBuilder->m_currContext.m_arraySizeOffs = ( uint32_t ) m_streamBuilder->m_currContext.m_currObject->m_stream.size();

            // Write the dummy size
            uint32_t zero = 0;
            m_streamBuilder->WriteValue( &zero );
        }
    }

    //======================================================================================================================
    void RttiLoader::EndProperty() {
        
        // Filter out properties by platform type and other parameters.
        if (ShouldHandleProperty() == false ) {
            return;
        }

        if ( m_streamBuilder->m_currContext.m_currProp == nullptr ) {
            // It's possible that we don't have a valid property; it may have been deprecated! A
            // warning has already been added to the log, so just bail.
            return;
        }

        if ( m_streamBuilder->m_currContext.m_currProp->m_isVector != 0 ) {
            // This property is an array, so we need to fill in the array size
            uint32_t * arraySizePtr = ( uint32_t * ) ( &m_streamBuilder->m_currContext.m_currObject->m_stream[ 0 ] + m_streamBuilder->m_currContext.m_arraySizeOffs );
            *arraySizePtr = m_streamBuilder->m_currContext.m_arrayCounter;
        }

        m_streamBuilder->m_currContext.m_currProp = nullptr;
        m_streamBuilder->m_currContext.m_arrayCounter = 0;
    }

    //======================================================================================================================
    void RttiLoader::AssignProperty( const RttiParseItem & lhs, const RttiParseItem & rhs ) {

        assert( lhs.IsLiteralString() == true );

        BeginProperty( lhs );

        if ( m_streamBuilder->m_currContext.m_currProp == nullptr ) {
            // It's possible that we don't have a valid property; it may have been deprecated! A
            // warning has already been added to the log, so just bail.
            return;
        }

        // Make sure that the property is not an array
        if ( m_streamBuilder->m_currContext.m_currProp->m_isVector != 0 ) {
            PrintError( " Property %s is a vector type\n", lhs.m_value.m_litString );
            return;
        }

        SetProperty( rhs );

        EndProperty();
    }

    //======================================================================================================================
    const RttiPropDef * RttiLoader::GetPropDef( const char * propName ) const {
        RttiType * type = rtti->Find( m_streamBuilder->m_currContext.m_currObject->m_objectType );
        XE_ERROR( type == nullptr, "Type for property is invalid\n" );

        const RttiPropDef * pd = type->FindProp( propName );
        return pd;

    }

    //=============================x=========================================================================================
    void RttiLoader::SetProperty( const RttiParseItem & rhs ) {
        
        // Filter out properties by platform type and other parameters.
        if (ShouldHandleProperty() == false ) {
            return;
        }

        if ( m_streamBuilder->m_currContext.m_currProp == nullptr ) {
            // It's possible that we don't have a valid property; it may have been deprecated! A
            // warning has already been added to the log, so just bail.
            return;
        }
    
        if ( m_streamBuilder->m_currContext.m_currProp->m_isVector ) {
            ++m_streamBuilder->m_currContext.m_arrayCounter;
        }

        switch ( m_streamBuilder->m_currContext.m_currProp->m_type ) {
            case RTTI_PROP_TYPE_INT: {
                    if ( rhs.IsLiteralInt() == false && rhs.IsLiteralFloat() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    int32_t value = ( rhs.m_litType == RttiParseItem::LIT_INT ) ? rhs.m_value.m_litInt : ( int32_t ) rhs.m_value.m_litFloat;
                    m_streamBuilder->WriteValue<int32_t>( &value );
                }
                break;

            case RTTI_PROP_TYPE_FLOAT: {
                    if ( rhs.IsLiteralInt() == false && rhs.IsLiteralFloat() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    float value = ( rhs.m_litType == RttiParseItem::LIT_INT ) ? (float) rhs.m_value.m_litInt : rhs.m_value.m_litFloat;
                    m_streamBuilder->WriteValue(  &value );
                }
                break;

            case RTTI_PROP_TYPE_BOOL: {
                    if ( rhs.IsLiteralInt() == false && rhs.IsLiteralFloat() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    int32_t value = ( rhs.m_litType == RttiParseItem::LIT_INT ) ? rhs.m_value.m_litInt : ( int32_t ) rhs.m_value.m_litFloat;
                    bool boolValue = ( value == 0 ) ? false : true;
                    m_streamBuilder->WriteValue(  &boolValue );
                }
                break;

            case RTTI_PROP_TYPE_VEC2: {
                    if ( rhs.IsLiteralVector2() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    Vector2 value(rhs.m_value.m_litVector2[ 0 ], rhs.m_value.m_litVector2[ 1 ] );
                    m_streamBuilder->WriteValue(  &value );
                }
                break;

            case RTTI_PROP_TYPE_VEC3: {
                    if ( rhs.IsLiteralVector3() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    Vector3 value( rhs.m_value.m_litVector3[ 0 ], rhs.m_value.m_litVector3[ 1 ], rhs.m_value.m_litVector3[ 2 ] );
                    m_streamBuilder->WriteValue(  &value );
                }
                break;
            case RTTI_PROP_TYPE_VEC4: {
                    if ( rhs.IsLiteralVector4() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    Vector4 value( rhs.m_value.m_litVector4[ 0 ], rhs.m_value.m_litVector4[ 1 ], rhs.m_value.m_litVector4[ 2 ], rhs.m_value.m_litVector4[ 3 ] );
                    m_streamBuilder->WriteValue(  &value );
                }
                break;
            case RTTI_PROP_TYPE_QUAT: {
                    if ( rhs.IsLiteralVector4() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    Quaternion value( rhs.m_value.m_litVector4[ 0 ], rhs.m_value.m_litVector4[ 1 ], rhs.m_value.m_litVector4[ 2 ], rhs.m_value.m_litVector4[ 3 ] );
                    m_streamBuilder->WriteValue(  &value );
                }
                break;

            case RTTI_PROP_TYPE_OBJECT_PTR:
            case RTTI_PROP_TYPE_OBJECT_REFPTR: {
                    if ( rhs.IsLiteralInt() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    int32_t value = rhs.m_value.m_litInt;
                    m_streamBuilder->WriteValue(  &value );
                }
                break;

            case RTTI_PROP_TYPE_STRING: {
                    if ( rhs.IsLiteralString() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }

                    m_streamBuilder->WriteString( rhs.m_value.m_litString );
                }
                break;

            case RTTI_PROP_TYPE_RESOURCE: {
                    if ( rhs.IsLiteralString() == false ) {
                        PrintError( "Invalid literal value for property %s\n", m_streamBuilder->m_currContext.m_currProp->m_name );
                        return;
                    }
                    m_streamBuilder->WriteString( rhs.m_value.m_litString );
                }
                break;
                
            default:
                XE_ASSERT_MSG(false, "Unhandled prop type in RttiLoader::SetProperty" );
        }
    }

    //=============================x=========================================================================================
    void RttiLoader::SetPlatform( const RttiParseItem & platString ) {
        PLATFORM_ID thisId = i3d::GetPlatformIdFromString( platString.m_value.m_litString );
        if ( thisId == PLATFORM_ID_NONE ) {
            m_currPlatformId = m_systemPlatformId;
        }
        else {
            m_currPlatformId = thisId;
        }
    }

    //=============================x=========================================================================================
    void RttiLoader::ClearPlatform() {
        m_currPlatformId = m_systemPlatformId;
    }

    //=============================x=========================================================================================
    bool RttiLoader::ShouldHandleProperty() {
        return m_systemPlatformId == m_currPlatformId;
    }
}
