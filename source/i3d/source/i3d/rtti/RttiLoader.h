//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTILOADER_H__
#define __I3D_RTTILOADER_H__

#include "i3d/rtti/RttiObject.h"
#include "i3d/rtti/RttiPropType.h"
#include "i3d/util/LoaderBase.h"
#include "i3d/rtti/RttiStream.h"

namespace i3d {

    class EntityParams;
    struct RttiPropDef;

    class RttiParseItem {
    public:
        enum TYPE {
            TYPE_NONE,
            TYPE_LITERAL,
            TYPE_OBJECTPTR,
        };

        enum LIT {
            LIT_INT,
            LIT_STRING,
            LIT_FLOAT,
            LIT_VECTOR2,
            LIT_VECTOR3,
            LIT_VECTOR4,
        };

        RttiParseItem() {
            m_type = TYPE_NONE;
        }

        ~RttiParseItem() {

        }

        void Clear() {
            if ( m_type == TYPE_LITERAL && m_litType == LIT_STRING ) {
                SAFE_DELETEARRAY( m_value.m_litString )
            }

            m_type = TYPE_NONE;
        }

        void SetLiteral( LIT litType, const char * text ) {

            m_type = TYPE_LITERAL;
            m_litType = litType;

            switch ( litType ) {
                case LIT_INT:
                    m_value.m_litInt = atoi( text );
                    break;
                case LIT_STRING:
                    {
                        size_t len = strlen( text );
                        if ( *text == '\"' || *text == '\'' ) {
                            len -= 2;
                            ++text;
                        }
                        m_value.m_litString = new char[ len + 1 ];
                        strncpy( m_value.m_litString, text, len );
                         m_value.m_litString[ len ] = 0;
                    }
                    break;
                case LIT_FLOAT:
                    m_value.m_litFloat = (float) atof( text );
                    break;
                case LIT_VECTOR2:
                case LIT_VECTOR3:
                case LIT_VECTOR4:
                    break;
            }
        }

        void SetLiteralString( const char * text ) {
            SetLiteral( LIT_STRING, text );
        }

        void SetLiteralInt( const char * text ) {
            SetLiteral( LIT_INT, text );
        }

        void SetLiteralFloat( const char * text ) {
            SetLiteral( LIT_FLOAT, text );
        }

        void SetLiteralInt( int32_t value ) {
            m_type = TYPE_LITERAL;
            m_litType = LIT_INT;
            
           m_value.m_litInt = value;
        }

        void SetLiteralVec2( const RttiParseItem & px, const RttiParseItem & py) {
            assert( px.m_litType == LIT_FLOAT );
            assert( py.m_litType == LIT_FLOAT );
        
            m_type = TYPE_LITERAL;
            m_litType = LIT_VECTOR2;
            m_value.m_litVector2[ 0 ] = px.m_value.m_litFloat;
            m_value.m_litVector2[ 1 ] = py.m_value.m_litFloat;
        }

        void SetLiteralVec3( const RttiParseItem & px, const RttiParseItem & py, const RttiParseItem & pz ) {
            assert( px.m_litType == LIT_FLOAT );
            assert( py.m_litType == LIT_FLOAT );
            assert( pz.m_litType == LIT_FLOAT );

            m_type = TYPE_LITERAL;
            m_litType = LIT_VECTOR3;
            m_value.m_litVector3[ 0 ] = px.m_value.m_litFloat;
            m_value.m_litVector3[ 1 ] = py.m_value.m_litFloat;
            m_value.m_litVector3[ 2 ] = pz.m_value.m_litFloat;
        }

        void SetLiteralVec4( const RttiParseItem & px, const RttiParseItem & py, const RttiParseItem & pz, const RttiParseItem & pw) {
            assert( px.m_litType == LIT_FLOAT );
            assert( py.m_litType == LIT_FLOAT );
            assert( pz.m_litType == LIT_FLOAT );
            assert( pw.m_litType == LIT_FLOAT );

            m_type = TYPE_LITERAL;
            m_litType = LIT_VECTOR4;
            m_value.m_litVector4[ 0 ] = px.m_value.m_litFloat;
            m_value.m_litVector4[ 1 ] = py.m_value.m_litFloat;
            m_value.m_litVector4[ 2 ] = pz.m_value.m_litFloat;
            m_value.m_litVector4[ 3 ] = pw.m_value.m_litFloat;
        }
        
        void SetAngleAxis( const RttiParseItem & axis, RttiParseItem & angleDegrees ) {
            assert( axis.m_litType == LIT_VECTOR3 );
            assert( angleDegrees.m_litType == LIT_FLOAT );
            
            Vector3 vaxis( axis.m_value.m_litVector3[0], axis.m_value.m_litVector3[1], axis.m_value.m_litVector3[2] );
            Quaternion q;
            q.Set( vaxis, i3d::scalar::DegToRad(angleDegrees.m_value.m_litFloat));

            m_type = TYPE_LITERAL;
            m_litType = LIT_VECTOR4;
            m_value.m_litVector4[ 0 ] = q.X();
            m_value.m_litVector4[ 1 ] = q.Y();
            m_value.m_litVector4[ 2 ] = q.Z();
            m_value.m_litVector4[ 3 ] = q.W();
        }

        void SetObject( RttiObject * object ) {
            m_type = TYPE_OBJECTPTR;
            m_value.m_objectPtr = object;
        }

        bool IsObjectPtr() const {
            return m_type == TYPE_OBJECTPTR;
        }

        bool IsLiteralString() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_STRING;
        }

        bool IsLiteralInt() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_INT;
        }

        bool IsLiteralFloat() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_FLOAT;
        }

        bool IsLiteralVector2() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_VECTOR2;
        }

        bool IsLiteralVector3() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_VECTOR3;
        }

        bool IsLiteralVector4() const {
            return m_type == TYPE_LITERAL && m_litType == LIT_VECTOR4;
        }

        TYPE                m_type;
        LIT                 m_litType;
        union Value {
            Value() {
            }

            RttiObject *    m_objectPtr;
            float           m_litFloat;
            int32_t         m_litInt;
            char *          m_litString;
            float           m_litVector2[2];
            float           m_litVector3[3];
            float           m_litVector4[4];
        } m_value;
    };

    class RttiLoader : public LoaderBase {
    public:
        RttiObject *                            m_object;
        
        RttiLoader();
        
        ~RttiLoader();

        bool Load( const char * path, i3d::PLATFORM_ID platformId = i3d::PLATFORM_ID_NONE );
        
        bool Load( File * file, const char * path, i3d::PLATFORM_ID platformId = i3d::PLATFORM_ID_NONE );
        
        bool LoadAndCompileToStream( RttiStream & stream, const char * path, i3d::PLATFORM_ID platformId = i3d::PLATFORM_ID_NONE );
        
        bool LoadAndCompileToStream( RttiStream & stream, File * file, const char * path, i3d::PLATFORM_ID platformId );

        void LoadFromStream( const char * path, RttiStream & stream );
        
        void Reset();
        
        uint32_t BeginObject( const RttiParseItem & type );
        
        void EndObject();

        void AssignProperty( const RttiParseItem & lhs, const RttiParseItem & rhs );

        void BeginProperty( const RttiParseItem & propName );

        void EndProperty();

        void SetProperty( const RttiParseItem & rhs );

        const RttiPropDef * GetPropDef( const char * propName ) const;

        void ReadPropScalar( const RttiPropDef * pd, void * propData );

        void ReadPropArray( const RttiPropDef * pd, void * propData );

        void SeekStream( uint32_t pos );

        void SetPlatform( const RttiParseItem & platString );

        void ClearPlatform();
        
        bool ShouldHandleProperty();
        
        uint32_t GetObjectIndex() const { return m_streamBuilder->m_currContext.m_objectIndex; }
       
    protected:
        PLATFORM_ID             m_systemPlatformId;
        PLATFORM_ID             m_currPlatformId;
        RttiStreamBuilder *     m_streamBuilder;
    };
}

#endif
