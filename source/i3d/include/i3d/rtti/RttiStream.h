//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTISTREAM_H__
#define __I3D_RTTISTREAM_H__

#include "i3d/core/Types.h"
#include "i3d/core/Hash.h"

namespace i3d {

    class RttiObject;
    class RttiType;
    struct RttiPropDef;

    class RttiStream {
    public:
        enum PROP_TYPE {
            PROP_TYPE_INT,
            PROP_TYPE_UINT,
            PROP_TYPE_BOOL,
            PROP_TYPE_FLOAT,
            PROP_TYPE_STRING,
            PROP_TYPE_VECTOR2,
            PROP_TYPE_VECTOR3,
            PROP_TYPE_VECTOR4,
        };

        static const uint32_t PROP_ARRAY = 0x80000000;
        
        struct Header {
            static const uint64_t MAGIC = XE_MAKE_ID(X, e, n, g, i, n, e, _, R, t, t, i);
            static const uint64_t VERSION = 1;
            
            uint64_t        m_magic;
            uint64_t        m_version;
            uint64_t        m_objectCount;
            uint64_t        m_propDataSize;
            uint64_t        m_offsObjects;
            uint64_t        m_offsProps;
        };

        struct Object {
            uint64_t        m_typeId;
            uint32_t        m_propsStart;
            uint32_t        m_propsSize;

            typedef stl::Vector<Object> array_t;
        };

        RttiStream() {

        }

        ~RttiStream() {

        }
        
        template<typename __type__>
        void ReadPropData( __type__ * value ) {
            //LOG_ERROR( m_propDataPos + sizeof( __type__ ) <= m_propData.size(), "Prop data read out of bounds");

            const __type__ * src = ( const __type__ * ) &m_propData[ m_propDataPos ];
            m_propDataPos += sizeof( __type__ );
            *value = *src;
        }

        template<typename __type__>
        void ReadPropData( __type__ * value, size_t count ) {
            size_t totalSize = sizeof( __type__ ) * count;
            //LOG_ERROR( m_propDataPos + totalSize <= m_propData.size(), "Prop data read out of bounds" );
            const __type__ * src = ( const __type__ * ) &m_propData[ m_propDataPos ];
            memcpy( value, src, totalSize );
            m_propDataPos += totalSize;
        }

        void ReadPropDataString( stl::String::type & value ) {
            uint32_t length = 0;
            ReadPropData<uint32_t>( &length );
            value.resize( length );
            ReadPropData<char>( &value[ 0 ], length );
        }

        void SeekPropData( uint32_t pos ) {
            m_propDataPos = pos;
        }

        uint32_t TellPropData() const {
            return (uint32_t) m_propDataPos;
        }
        
        void ReadPropScalar( const RttiPropDef * pd, void * propData );
        
        void ReadPropArray( const RttiPropDef * pd, void * propData );
        
        void WriteToFile( File * file );
        
        void ReadFromFile( File * file );
        
        void Construct();

    public:
        stl::Vector<Object>::type               m_objectInfo;
        stl::Vector<uint8_t>::type              m_propData;
        stl::Vector<RttiObject *>::type         m_objects;
        uintptr_t                               m_propDataPos;
    };
    
    template<typename __type__> inline RttiStream::PROP_TYPE RttiStream_GetPropType() {}
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<int32_t>() { return RttiStream::PROP_TYPE_INT; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<unsigned char>() { return RttiStream::PROP_TYPE_BOOL; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<uint32_t>() { return RttiStream::PROP_TYPE_UINT; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<float>() { return RttiStream::PROP_TYPE_FLOAT; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<const char *>() { return RttiStream::PROP_TYPE_STRING; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<Vector2>() { return RttiStream::PROP_TYPE_VECTOR2; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<Vector3>() { return RttiStream::PROP_TYPE_VECTOR3; }
    template<> inline RttiStream::PROP_TYPE RttiStream_GetPropType<Vector4>() { return RttiStream::PROP_TYPE_VECTOR4; }

    class RttiStreamBuilder {
    public:

        static constexpr size_t STACK_MAX = 128;

        class ObjectStream {
        public:
            ObjectStream()
            { }

            ~ObjectStream() {

            }

            uint32_t WriteString( const char * str ) {
                uint32_t pos = (uint32_t) m_stream.size();
                uint32_t len = (uint32_t) strlen( str );
                WriteValue<uint32_t>( &len );
                while ( *str != 0 ) {
                    WriteValue<char>( str );
                    ++str;
                }

                return pos;
            }

            template<typename __type__>
            uint32_t WriteValue( const __type__ * value ) {
                uint32_t pos = (uint32_t) m_stream.size();
                const uint8_t * bytes = ( const uint8_t * ) value;
                size_t size = sizeof( __type__ );
                while ( size > 0 ) {
                    m_stream.push_back( *bytes );
                    ++bytes;
                    --size;
                }

                return pos;
            }

            template<typename __type__>
            uint32_t WriteProp( const char * propName, const __type__ * value ) {
                uint64_t nameHash = fh64::CalcFromString( propName );
                WriteValue<uint64_t>( &nameHash );
                WriteValue<__type__>( *value );
            }

            uint64_t                m_objectType;
            std::vector<uint8_t>    m_stream;
        };

        struct Context {
            Context() {
                m_currObject = nullptr;
                m_objectIndex = 0;
                m_currProp = nullptr;
                m_arrayCounter = 0;
                m_arraySizeOffs = 0;
                m_objectCreatedIndex = 0;
            }

            Context( const Context & rhs ) {
                m_currObject = rhs.m_currObject;
                m_objectIndex = rhs.m_objectIndex;
                m_currProp = rhs.m_currProp;
                m_arrayCounter = rhs.m_arrayCounter;
                m_arraySizeOffs = rhs.m_arraySizeOffs;
                m_objectCreatedIndex = rhs.m_objectCreatedIndex;
            }

            void Clear() {
                m_currObject = nullptr;
                m_objectIndex = 0;
                m_currProp = nullptr;
                m_arrayCounter = 0;
                m_arraySizeOffs = 0;
                m_objectCreatedIndex = 0;
            }

            ObjectStream *          m_currObject;
            uint32_t                m_objectIndex;
            const RttiPropDef *     m_currProp;
            uint32_t                m_arrayCounter;
            uint32_t                m_arraySizeOffs;
            uint32_t                m_objectCreatedIndex;
        };

        template<typename __type__>
        void WriteProp( const char * propName, const __type__ * value ) {
            m_currContext.m_currObject->WriteProp<__type__>( propName, value );
        }

        template<typename __type__>
        void WriteValue( const __type__ * value ) {
            m_currContext.m_currObject->WriteValue<__type__>( value );
        }

        void WriteString( const char * str ) {
            m_currContext.m_currObject->WriteString( str );
        }

        template<typename __type__>
        void WritePropType( bool isArray = false ) {
            uint32_t type = RttiStream_GetPropType<__type__>();
            if ( isArray == true ) {
                type |= RttiStream::PROP_ARRAY;
            }

            WriteValue<uint32_t>( &type );
        }

        void WritePropType( RTTI_PROP_TYPE propType, bool isArray );
        
        RttiStreamBuilder();

        ~RttiStreamBuilder();

        uint32_t BeginObject( const char * typeName );

        void EndObbject();

        void Build( RttiStream & str );

    public:
        Context     m_currContext;
        Context     m_stack[ STACK_MAX ];
        uint32_t    m_tos;
        std::vector<ObjectStream*> m_objects;
     };
}

#endif
