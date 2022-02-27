//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/rtti/Rtti.h"
#include "i3d/rtti/RttiStream.h"
#include "i3d/res/Resource.h"

namespace i3d {

    //=======================================================================================================================
    void RttiStream::WriteToFile( File * file ) {
        
        // Fill out the header with the info we have right now
        Header header;
        memset(&header, 0, sizeof(header));
        header.m_magic          = Header::MAGIC;
        header.m_version        = Header::VERSION;
        header.m_objectCount    = m_objectInfo.size();
        header.m_propDataSize   = m_propData.size();
        
        uint64_t offsHeader = file->Pos();
        file->Write( &header, sizeof(header), 1);
        
        // Write out object info
        file->WriteAlignPadding( 64 );
        header.m_offsObjects = file->Pos() - offsHeader;
        file->Write( &m_objectInfo[0], sizeof(Object), m_objectInfo.size());
        
        // Write out prop data stream
        file->WriteAlignPadding( 64 );
        header.m_offsProps = file->Pos() - offsHeader;
        file->Write( &m_propData[0], sizeof(uint8_t), m_propData.size() );
        
        // Re-write the header now that it's been completely filled out
        uint64_t endPos = file->Pos();
        file->Seek( offsHeader );
        file->Write( &header, sizeof(header), 1 );
        file->Seek( endPos );
    }
    
    //=======================================================================================================================
    void RttiStream::ReadFromFile( File * file ) {
        Header header;
        file->Read( &header, sizeof(header), 1 );
        
        XE_ASSERT( header.m_magic == Header::MAGIC );
        XE_ASSERT( header.m_version == Header::VERSION );
        
        // Read Object info
        m_objectInfo.resize( header.m_objectCount );
        file->Seek( header.m_offsObjects );
        file->Read( &m_objectInfo[0], sizeof(Object), header.m_objectCount );
        
        // Read in prop data stream
        m_propData.resize( header.m_propDataSize );
        file->Seek( header.m_offsProps );
        file->Read( &m_propData[0], sizeof(uint8_t), header.m_propDataSize );
    }
    
    //=======================================================================================================================
    void RttiStream::Construct() {
        // Create the objects
        for ( auto & oi : m_objectInfo ) {
            RttiObject * object = rtti->Create( oi.m_typeId );
            m_objects.push_back( object );
        }

        uint32_t objectIndex = 0;
        for ( auto & oi : m_objectInfo ) {

            RttiObject * object = m_objects[ objectIndex ];
            ++objectIndex;

            if ( oi.m_propsSize > 0 ) {
                
                // Flag that props were read in for this object
                object->SetPropsWereRead( true );

                uint32_t endPos = oi.m_propsStart + oi.m_propsSize;
                SeekPropData( oi.m_propsStart );

                while ( TellPropData() < endPos ) {

                    uint64_t propName;
                    uint32_t propType;

                    ReadPropData<uint64_t>( &propName );
                    ReadPropData<uint32_t>( &propType );

                    bool isArray = propType & 0x80000000;
                    propType &= 0x7fffffff;

                    const RttiPropDef * pd = object->GetType()->FindProp( propName );
                    XE_ERROR( pd == nullptr, "uNKNOWN PROPERTY NAME\n" );
                    uint8_t * propData = object->GetPropValue<uint8_t>( pd );

                    if ( isArray ) {
                        ReadPropArray( pd, propData );
                    } else {
                        ReadPropScalar( pd, propData );
                    }
                }
            }
        }
        
        for ( auto o : m_objects ) {
            o->ReadComplete( nullptr );
        }
    }

    
    //======================================================================================================================
    void RttiStream::ReadPropArray( const RttiPropDef * pd, void * propData ) {

        uint32_t arraySize = 0;
        ReadPropData<uint32_t>( &arraySize );

        // Nothing to load in this array, so just bail.
        if ( arraySize == 0 ) {
            return;
        }
        
        switch ( pd->m_type ) {
            case RTTI_PROP_TYPE_INT: {
                    stl::Vector<uint32_t>::type * propArray = ( stl::Vector<uint32_t>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<uint32_t>( &(*propArray)[0], arraySize );
                }
                break;

            case RTTI_PROP_TYPE_FLOAT : {
                    stl::Vector<float>::type * propArray = ( stl::Vector<float>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<float>( &(*propArray)[0], arraySize );
                }
                break;

            case RTTI_PROP_TYPE_BOOL: {
                    stl::Vector<bool>::type * propArray = ( stl::Vector<bool>::type * ) propData;
                    propArray->resize( arraySize );

                    for ( uint32_t i = 0; i < arraySize; ++i ) {
                        uint8_t val;
                        ReadPropData<uint8_t>( &val );
                        ( *propArray )[ i ] = ( val != 0 );
                    }
                }
                break;
        
            case RTTI_PROP_TYPE_VEC2: {
                    stl::Vector<Vector2>::type * propArray = ( stl::Vector<Vector2>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<Vector2>( &(*propArray)[0], arraySize );
                }
                break;

            case RTTI_PROP_TYPE_VEC3: {
                    stl::Vector<Vector3>::type * propArray = ( stl::Vector<Vector3>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<Vector3>( &(*propArray)[0], arraySize );
                }
                break;

            case RTTI_PROP_TYPE_VEC4: {
                    stl::Vector<Vector4>::type * propArray = ( stl::Vector<Vector4>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<Vector4>( &(*propArray)[0], arraySize );
                }
                break;

            case RTTI_PROP_TYPE_QUAT: {
                    stl::Vector<Quaternion>::type * propArray = ( stl::Vector<Quaternion>::type * ) propData;
                    propArray->resize( arraySize );
                    ReadPropData<Quaternion>( &(*propArray)[0], arraySize );
                }
                break;
           
            case RTTI_PROP_TYPE_OBJECT_PTR: {
                    stl::Vector<RttiObject*>::type * propArray = ( stl::Vector<RttiObject*>::type * ) propData;
                    propArray->resize( arraySize );

                    for ( uint32_t i = 0; i < arraySize; ++i ) {
                        uint32_t index;
                        ReadPropData<uint32_t>( &index );
                        (*propArray)[i] = m_objects[ index ];
                    }
                }
                break;

            case RTTI_PROP_TYPE_OBJECT_REFPTR: {
                    stl::Vector<RttiObject::ref_ptr_t>::type * propArray = ( stl::Vector<RttiObject::ref_ptr_t>::type * ) propData;
                    propArray->resize( arraySize );

                    for ( uint32_t i = 0; i < arraySize; ++i ) {
                        uint32_t index;
                        ReadPropData<uint32_t>( &index );
                        (*propArray)[i] = m_objects[ index ];
                    }
                }
                break;

            case RTTI_PROP_TYPE_STRING: {
                    stl::Vector<stl::String::type>::type * propArray = ( stl::Vector<stl::String::type>::type * ) propData;
                    propArray->resize( arraySize );

                    for ( uint32_t i = 0; i < arraySize; ++i ) {
                        ReadPropDataString( (*propArray)[i] );
                    }
                }
                break;

            case RTTI_PROP_TYPE_RESOURCE: {
                    stl::Vector<Resource*>::type * propArray = ( stl::Vector<Resource*>::type * ) propData;
                    propArray->resize( arraySize );

                    for ( uint32_t i = 0; i < arraySize; ++i ) {
                        stl::String::type path;
                        ReadPropDataString( path );
                        (*propArray)[i] = res->Load( path.c_str() );
                    }
                }
                break;
            default:
                XE_ASSERT_MSG(false, "Unhandled prop type in RttiLoader::ReadPropArray" );
        }
    }

    //======================================================================================================================
    void RttiStream::ReadPropScalar( const RttiPropDef * pd, void * propData ) {
        switch ( pd->m_type ) {
            case RTTI_PROP_TYPE_INT:
                ReadPropData<int32_t>( (int32_t*) propData );
                break;

            case RTTI_PROP_TYPE_FLOAT:
                ReadPropData<float>( (float*) propData );
                break;

            case RTTI_PROP_TYPE_BOOL: {
                    uint8_t val = 0;
                    ReadPropData<uint8_t>( &val );
                    *( ( bool * ) propData ) = ( val == 0 ) ? false : true;
                }
                break;
        
            case RTTI_PROP_TYPE_VEC2:
                ReadPropData<Vector2>( (Vector2*) propData );
                break;

            case RTTI_PROP_TYPE_VEC3:
                ReadPropData<Vector3>( (Vector3*) propData );
                break;

            case RTTI_PROP_TYPE_VEC4:
                ReadPropData<Vector4>( (Vector4*) propData );
                break;

            case RTTI_PROP_TYPE_QUAT:
                ReadPropData<Quaternion>( (Quaternion*) propData );
                break;
           
            case RTTI_PROP_TYPE_OBJECT_PTR: {
                    uint32_t index;
                    ReadPropData<uint32_t>( &index );
                    *( ( RttiObject** ) propData ) = m_objects[ index ];
                }
                break;

            case RTTI_PROP_TYPE_OBJECT_REFPTR: {
                    uint32_t index;
                    ReadPropData<uint32_t>( &index );
                    *( ( RttiObject::ref_ptr_t* ) propData ) = m_objects[ index ];
                }
                break;

            case RTTI_PROP_TYPE_STRING:
                ReadPropDataString( *( ( stl::String::type * ) propData ) );
                break;

            case RTTI_PROP_TYPE_RESOURCE: {
                    stl::String::type path;
                    ReadPropDataString( path );
                    *( ( Resource ** ) propData ) = res->Load( path.c_str() );
                }
                break;
                
            default:
                XE_ASSERT_MSG(false, "Unhandled prop type in RttiLoader::ReadPropScalar" );
        }
    }

    //=======================================================================================================================
    RttiStreamBuilder::RttiStreamBuilder() {
        m_tos = 0;
    }

    //=======================================================================================================================
    RttiStreamBuilder::~RttiStreamBuilder() {

    }

    //=======================================================================================================================
    uint32_t RttiStreamBuilder::BeginObject(const char * typeName) {
        assert( m_tos < STACK_MAX );

        if ( m_currContext.m_currObject != nullptr ) {
            m_stack[ m_tos ] = m_currContext;
            ++m_tos;
            m_currContext.Clear();
        }

        uint32_t objectIndex = (uint32_t) m_objects.size();
        m_currContext.m_currObject = new ObjectStream;
        m_currContext.m_objectIndex = objectIndex;
        m_currContext.m_currObject->m_objectType = fh64::CalcFromString( typeName );

        m_objects.push_back( m_currContext.m_currObject );

        return objectIndex;
    }

    //=======================================================================================================================
    void RttiStreamBuilder::EndObbject() {
        if ( m_tos > 0 ) {
            m_currContext = m_stack[ m_tos - 1 ];
            --m_tos;
        }
    }

    //=======================================================================================================================
    void RttiStreamBuilder::WritePropType( RTTI_PROP_TYPE propType, bool isVector ) {
        switch ( propType ) {
            case RTTI_PROP_TYPE_INT:
                WritePropType<uint32_t>( isVector );
                break;

            case RTTI_PROP_TYPE_FLOAT:
                WritePropType<float>( isVector );
                break;

            case RTTI_PROP_TYPE_BOOL:
                WritePropType<uint8_t>( isVector );
                break;

            case RTTI_PROP_TYPE_VEC2:
                WritePropType<Vector2>( isVector );
                break;

            case RTTI_PROP_TYPE_VEC3:
                WritePropType<Vector3>( isVector );
                break;
            case RTTI_PROP_TYPE_VEC4:
                WritePropType<Vector4>( isVector );
                break;

            case RTTI_PROP_TYPE_QUAT:
                WritePropType<Vector4>( isVector );
                break;

            case RTTI_PROP_TYPE_OBJECT_PTR:					// Fall-through
            case RTTI_PROP_TYPE_OBJECT_REFPTR:
                WritePropType<uint32_t>( isVector );
                break;

            case RTTI_PROP_TYPE_STRING:						// Fall-through
            case RTTI_PROP_TYPE_RESOURCE:
                WritePropType<const char *>( isVector );
                break;
            default:
                XE_ASSERT_MSG(false, "Unhandled prop type in RttiStreamBuilder::WritePropType");
        }
    }

    //=======================================================================================================================
    void RttiStreamBuilder::Build( RttiStream & str ) {
        size_t dataSize = 0;

        // Fill in the object info and calculate the total size of the prop
        // data stream
        for ( auto * o : m_objects ) {
            RttiStream::Object oi;
            oi.m_typeId         = o->m_objectType;
            oi.m_propsStart     = (uint32_t) dataSize;
            oi.m_propsSize      = (uint32_t) o->m_stream.size();
            str.m_objectInfo.push_back( oi );

            dataSize += oi.m_propsSize;
        }

        // Merge the prop data streams for all of the objects
        str.m_propData.resize( dataSize );
        uint32_t objectIndex = 0;
        for ( auto * o : m_objects ) {

            if ( o->m_stream.size() > 0 ) {
                RttiStream::Object * oi = &str.m_objectInfo[ objectIndex ];
                memcpy( &str.m_propData[ oi->m_propsStart ], o->m_stream.data(), oi->m_propsSize );
            }

            ++objectIndex;
        }
    }
    
}
