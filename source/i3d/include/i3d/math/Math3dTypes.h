
#ifndef __I3D_MATH3DTYPES_H__
#define __I3D_MATH3DTYPES_H__

#include "i3d/math/Scalar.h"
#include "i3d/math/Vector2.h"
#include "i3d/math/Matrix2.h"

namespace i3d {

    class Vector3;
    class Vector4;
    class Quaternion;
    class Matrix3;
    class Matrix4;
    
    

    class Vector3 {
    public:
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 ZERO;
    
        Vector3();
    
        Vector3( const Vector3 & rhs );
    
        Vector3( const Vector4 & rhs );
    
        Vector3( scalar_t x, scalar_t y, scalar_t z );
    
        scalar_t X() const;
        scalar_t Y() const;
        scalar_t Z() const;
    
        scalar_t & X();
        scalar_t & Y();
        scalar_t & Z();
    
        void Set( scalar_t x, scalar_t y, scalar_t z );
    
        void Set( const Vector4 & rhs );
    
        scalar_t Magnitude() const;
    
        scalar_t Normalise();
    
        scalar_t Normalise( const Vector3 & rhs );
    
        scalar_t Dot( const Vector3 & rhs ) const;
    
        void Cross( const Vector3 & lhs, const Vector3 & rhs );
    
        void Mix( const Vector3 & from, const Vector3 & to, scalar_t t );
    
        void Min( const Vector3 & lhs, const Vector3 & rhs);
    
        void Max( const Vector3 & lhs, const Vector3 & rhs);
    
        Vector3 operator - () const;
    
        Vector3 & operator = ( const Vector3& rhs );
    
        Vector3 & operator = ( const Vector4& rhs );
    
        Vector3 operator + ( const Vector3& rhs ) const;
        Vector3 operator - ( const Vector3& rhs ) const;
        Vector3 operator * ( const Vector3& rhs ) const;
        Vector3 operator / ( const Vector3& rhs ) const;
        Vector3 operator + ( scalar_t rhs ) const;
        Vector3 operator - ( scalar_t rhs ) const;
        Vector3 operator * ( scalar_t rhs ) const;
        Vector3 operator / ( scalar_t rhs ) const;
    
        Vector3 & operator += ( const Vector3 & rhs );
        Vector3 & operator -= ( const Vector3 & rhs );
        Vector3 & operator *= ( const Vector3 & rhs );
        Vector3 & operator /= ( const Vector3 & rhs );
        Vector3 & operator += ( scalar_t rhs );
        Vector3 & operator -= ( scalar_t rhs );
        Vector3 & operator *= ( scalar_t rhs );
        Vector3 & operator /= ( scalar_t rhs );
    
    protected:
        scalar_t m_x;
        scalar_t m_y;
        scalar_t m_z;
        scalar_t m_pad;
    };

    class Vector4 {
    public:
        static const Vector4 UNIT_X;
        static const Vector4 UNIT_Y;
        static const Vector4 UNIT_Z;
        static const Vector4 UNIT_W;
        static const Vector4 ZERO;
    
        Vector4();
    
        Vector4(const Vector4& rhs);
    
        Vector4(const Vector3& rhs, float w = 0);
    
        Vector4( scalar_t x, scalar_t y, scalar_t z, scalar_t w );
    
        scalar_t X() const;
        scalar_t Y() const;
        scalar_t Z() const;
        scalar_t W() const;
    
        scalar_t& X();
        scalar_t& Y();
        scalar_t& Z();
        scalar_t& W();
    
        void Set( scalar_t x, scalar_t y, scalar_t z, scalar_t w );
    
        void Set(const Vector4& rhs);
    
        void Set(const Vector3& rhs, float w = 1);
    
        Vector4 operator-() const;
    
        Vector4& operator=(const Vector4& rhs);
    
        Vector4& operator=(const Vector3& rhs);
    
        scalar_t Magnitude() const;
    
        scalar_t Normalise(const Vector4& rhs);
    
        scalar_t Dot(const Vector4& rhs) const;
    
        void Mix(const Vector4& from, const Vector4& to, scalar_t t);
    
        void Min(const Vector4& lhs, const Vector4& rhs);
    
        void Max(const Vector4& lhs, const Vector4& rhs);
    
        Vector4 operator+( const Vector4 & rhs ) const;
        Vector4 operator-( const Vector4 & rhs ) const;
        Vector4 operator*( const Vector4 & rhs ) const;
        Vector4 operator/( const Vector4 & rhs ) const;
        Vector4 operator+( scalar_t rhs ) const;
        Vector4 operator-( scalar_t rhs ) const;
        Vector4 operator*( scalar_t rhs ) const;
        Vector4 operator/( scalar_t rhs ) const;
    
        Vector4 & operator+=( const Vector4 & rhs );
        Vector4 & operator-=( const Vector4 & rhs );
        Vector4 & operator*=( const Vector4 & rhs );
        Vector4 & operator/=( const Vector4 & rhs );
        Vector4 & operator+=( scalar_t rhs );
        Vector4 & operator-=( scalar_t rhs );
        Vector4 & operator*=( scalar_t rhs );
        Vector4 & operator/=( scalar_t rhs );
    
    protected:
        scalar_t m_x;
        scalar_t m_y;
        scalar_t m_z;
        scalar_t m_w;
    };

    class Matrix3 {
    public:
        static const size_t ROW_COUNT = 3;
        static const Matrix3 IDENTITY;

        Matrix3();
    
        Matrix3(const Matrix3 & rhs);
  
        Matrix3(const Vector3 & right, const Vector3 & up, const Vector3 & at);
  
        Matrix3(float right, float up, float at);
  
        void Set(const Vector3 & right, const Vector3 & up, const Vector3 & at);
  
        void Set(float right, float up, float at);
  
        void Concat(const Matrix3 & lhs, const Matrix3 & rhs);
  
        void Transform(Vector3 & dst, const Vector3 & src) const;
  
        void Inverse(const Matrix3 & rhs);
  
        void SetRotationAA(const Vector3& axis, float angle);
  
        void SetRotationQ(const Quaternion & rhs);
    
        Matrix3 operator-() const;
    
        Vector3 operator*(const Vector3& rhs) const;
    
        Matrix3 operator*(const Matrix3& rhs) const;
    
        Matrix3 & operator*=(const Matrix3&rhs);
    
    public:
        Vector3        m_rows[ ROW_COUNT ];
    };

    class Matrix4 {
    public:
        static const size_t ROW_COUNT = 4;
        static const Matrix4 IDENTITY;

        Matrix4();

        Matrix4(const Matrix4& rhs);

        Matrix4(const Vector4& right, const Vector4& up, const Vector4& at, const Vector4& pos);

        Matrix4(scalar_t  right, scalar_t  up, scalar_t  at, scalar_t  pos);

        void Set(const Vector4& right, const Vector4& up, const Vector4& at, const Vector4& pos);

        void Set(scalar_t  right, scalar_t  up, scalar_t  at, scalar_t  pos);

        void Concat(const Matrix4& lhs, const Matrix4& rhs) ;

        void Transform(Vector4& dst, const Vector4& src) const;

        void Transform(Vector3& dst, const Vector3& src) const;

        void TransformVector(Vector3& dst, const Vector3& src) const;

        void Transform3(Vector3& dst, const Vector3& src) const;

        void Inverse(const Matrix4& rhs);
    
        void Transpose( const Matrix4 & rhs );

        void SetRotationAA(const Vector3& axis, scalar_t  angle);

        void SetRotationQ(const Quaternion& rhs);

        /// Set this matrix to represent a left-handed perspective projection
        ///@param fov The vertical filed of view defined in radians
        ///@param aspect The vertocal aspect ratioj
        ///@param nearClip The distance of the near clip plane from the viewer
        ///@param farClip The distance of the far clip plane from the viewer
        void SetProjectionPerspLH(scalar_t  fov, scalar_t  aspect, scalar_t  nearClip, scalar_t  farClip);

        void SetProjectionPerspLHInf(scalar_t  fov, scalar_t  aspect, scalar_t  nearClip, scalar_t  farClip);

        /// Set this matrxi to represent a left-handed orthographic projection
        ///@param width The horizontal scale
        ///@param height The horizontal scale
        ///@param nearClip The distance from the viewer to the near clip plane
        ///@param farClip The distance from the viewer to the far clip plane
        void SetProjectionOrthoLH(scalar_t  width, scalar_t  height, scalar_t  nearClip, scalar_t  farClip);

        /// Set this matrix to represent a 3d-transfrom that gives as a view matrix from a position and a target
        ///@param eye The location of the viewer
        ///@param lookAt The point the view is looking at
        ///@param up Natural up vector
        void SetLookAt(const Vector3& eye, const Vector3& lookAt, const Vector3& up);
    
        void SetTranslation( const Vector3& trans );
    
        void GetRotationQ( Quaternion & rot ) const;

        Matrix4& operator=(const Matrix4& rhs);

        Matrix4 operator*(const Matrix4& rhs) const;

        Vector4 operator*(const Vector4& rhs) const;

        Vector3 operator*(const Vector3& rhs) const;
    
    public:
        Vector4         m_rows[ ROW_COUNT ];
    };

    class Quaternion {
    public:
        static const Quaternion ZERO;
        static const Quaternion IDENTITY;

        Quaternion();

        Quaternion( const Quaternion & rhs );

        Quaternion( scalar_t x, scalar_t y, scalar_t z, scalar_t w );
    
        scalar_t X() const;
        scalar_t Y() const;
        scalar_t Z() const;
        scalar_t W() const;
    
        scalar_t& X();
        scalar_t& Y();
        scalar_t& Z();
        scalar_t& W();

        void Set(scalar_t x, scalar_t y, scalar_t z, scalar_t w);
    
        void Set(const Vector3 & axis, scalar_t angleRads);
    
        void Set(const Vector3 & rhs);
    
        inline scalar_t Magnitude() const;

        inline Quaternion & Concat( const Quaternion & lhs, const Quaternion & rhs );
    
        inline scalar_t Dot( const Quaternion & rhs ) const;
    
        inline scalar_t Normalise( const Quaternion & rhs );
    
        Quaternion & Slerp( const Quaternion & from, const Quaternion & to, scalar_t t );
        
        void SetFromMatrix( const Matrix3 & rhs );
        
        void SetFromMatrix( const Matrix4 & rhs );
        
        void Conjugate( const Quaternion & rhs ) {
            Set( -rhs.X(), -rhs.Y(), -rhs.Z(), rhs.W() );
        }
        
        void Conjugate() {
            Conjugate( * this );
        }
        
        void Inverse( const Quaternion & rhs );
        
        void Inverse();
    
        inline Quaternion operator-() const;
    
        inline Quaternion & operator=(const Quaternion & rhs);
    
        inline Quaternion operator*(const Quaternion & rhs) const;

        inline Quaternion & operator*=(const Quaternion & rhs);
        
        inline Quaternion operator / ( const scalar_t & rhs ) const {
            return Quaternion( m_x / rhs, m_y / rhs, m_z / rhs, m_w / rhs );
        }
    
    protected:
        scalar_t m_x;
        scalar_t m_y;
        scalar_t m_z;
        scalar_t m_w;
    };
}

namespace std {
    template<>
    struct is_pod<i3d::Vector3> {
        static const bool value = true;
        operator bool() const { return value; }
    };
    
#if 0
    // Speclaised versions of is_pod for the math classes. We can treat them
    // as pod types since we never rely on them being constructed despite there being
    // a defaukt constructor.
    template<> inline constexpr bool is_pod<i3d::Vector3>() { return true; }
    template<> inline constexpr bool is_pod<i3d::Vector4>() { return true; }
    template<> inline constexpr bool is_pod<i3d::Quaternion>() { return true; }
    template<> inline constexpr bool is_pod<i3d::Matrix3>() { return true; }
    template<> inline constexpr bool is_pod<i3d::Matrix4>() { return true; }
#endif
}

#endif
