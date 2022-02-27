//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MATH3D_INL__
#define __I3D_MATH3D_INL__

namespace i3d {

    //======================================================================================================================
    // Vector3
    //======================================================================================================================

    //======================================================================================================================
    inline Vector3::Vector3() {
        // Empty - does nothing
    }

    //======================================================================================================================
    inline Vector3::Vector3(const Vector3& rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_pad = 0;
    }

    //======================================================================================================================
    inline Vector3::Vector3(const Vector4 & rhs) {
        Set(rhs);
    }

    //======================================================================================================================
    inline Vector3::Vector3(float x, float y, float z) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_pad = 0;
    }

    //======================================================================================================================
    inline float Vector3::X() const { return m_x; }

    //======================================================================================================================
    inline float Vector3::Y() const { return m_y; }

    //======================================================================================================================
    inline float Vector3::Z() const { return m_z; }

    //======================================================================================================================
    inline float& Vector3::X() { return m_x; }

    //======================================================================================================================
    inline float& Vector3::Y() { return m_y; }

    //======================================================================================================================
    inline float& Vector3::Z() { return m_z; }

    //======================================================================================================================
    inline void Vector3::Set( float x, float y, float z ) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_pad = 0;
    }

    //======================================================================================================================
    inline void Vector3::Set( const Vector4 & rhs ) {
        m_x = rhs.X();
        m_y = rhs.Y();
        m_z = rhs.Z();
        m_pad = 0;
    }
    //======================================================================================================================
    inline Vector3 Vector3::operator-() const {
        return Vector3(-m_x, -m_y, -m_z);
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator=(const Vector3& rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator=(const Vector4& rhs) {
        m_x = rhs.X();
        m_y = rhs.Y();
        m_z = rhs.Z();
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline float Vector3::Magnitude() const {
        return std::sqrt(Dot(*this));
    }

    //======================================================================================================================
    inline float Vector3::Normalise(const Vector3& rhs) {
        float mag = rhs.Magnitude();
        float fac = 1.0f / mag;

        *this = rhs * fac;
        return mag;
    }

    //======================================================================================================================
    inline float Vector3::Normalise() {
        return Normalise( *this );
    }

    //======================================================================================================================
    inline float Vector3::Dot(const Vector3& rhs) const {
        return (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z);
    }

    //======================================================================================================================
    inline void Vector3::Cross(const Vector3& lhs, const Vector3& rhs) {
        m_x = (lhs.m_y * rhs.m_z) - (lhs.m_z * rhs.m_y);
        m_y = (lhs.m_z * rhs.m_x) - (lhs.m_x * rhs.m_z);
        m_z = (lhs.m_x * rhs.m_y) - (lhs.m_y * rhs.m_x);
        m_pad = 0;
    }

    //======================================================================================================================
    inline void Vector3::Mix( const Vector3& from, const Vector3& to, float t ) {
        float tClamped = t;
        *this = (from * (1.0f - tClamped)) + (to * tClamped);
    }

    //======================================================================================================================
    inline void Vector3::Min(const Vector3& lhs, const Vector3& rhs) {
        m_x = std::fmin(lhs.m_x, rhs.m_x);
        m_y = std::fmin(lhs.m_y, rhs.m_y);
        m_z = std::fmin(lhs.m_z, rhs.m_z);
        m_pad = 0;
    }

    //======================================================================================================================
    inline void Vector3::Max(const Vector3& lhs, const Vector3& rhs) {
        m_x = std::fmax(lhs.m_x, rhs.m_x);
        m_y = std::fmax(lhs.m_y, rhs.m_y);
        m_z = std::fmax(lhs.m_z, rhs.m_z);
        m_pad = 0;
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator+( const Vector3& rhs ) const {
        return Vector3( m_x + rhs.m_x,
                        m_y + rhs.m_y,
                        m_z + rhs.m_z );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator-( const Vector3& rhs ) const {
        return Vector3( m_x - rhs.m_x,
                        m_y - rhs.m_y,
                        m_z - rhs.m_z );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator*( const Vector3& rhs ) const {
        return Vector3( m_x * rhs.m_x,
                        m_y * rhs.m_y,
                        m_z * rhs.m_z );
    }

    inline Vector3 Vector3::operator/( const Vector3& rhs ) const {
        return Vector3( m_x / rhs.m_x,
                        m_y / rhs.m_y,
                        m_z / rhs.m_z );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator+( scalar_t rhs ) const {
        return *this + Vector3( rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator-( scalar_t rhs ) const {
        return *this - Vector3( rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator*( scalar_t rhs ) const {
        return *this * Vector3( rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector3 Vector3::operator/( scalar_t rhs ) const {
        return *this / Vector3( rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator+=( const Vector3 & rhs ) {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator-=( const Vector3 & rhs ) {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator*=( const Vector3 & rhs ) {
        m_x *= rhs.m_x;
        m_y *= rhs.m_y;
        m_z *= rhs.m_z;
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator/=( const Vector3 & rhs ) {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
        m_z /= rhs.m_z;
        m_pad = 0;
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator+=( scalar_t rhs ) {
        *this += Vector3( rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator-=( scalar_t rhs ) {
        *this -= Vector3( rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator*=( scalar_t rhs ) {
        *this *= Vector3( rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector3& Vector3::operator/=( scalar_t rhs ) {
        *this /= Vector3( rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    // Vector4
    //======================================================================================================================

    //======================================================================================================================
    inline Vector4::Vector4() {
        // Empty - does nothing
    }

    //======================================================================================================================
    inline Vector4::Vector4( const Vector4 & rhs ) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;
    }

    //======================================================================================================================
    inline Vector4::Vector4( const Vector3& rhs, scalar_t w ) {
        Set( rhs, w );
    }

    //======================================================================================================================
    inline Vector4::Vector4( scalar_t x, scalar_t y, scalar_t z, scalar_t w ) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    //======================================================================================================================
    inline scalar_t Vector4::X() const { return m_x; }

    //======================================================================================================================
    inline scalar_t Vector4::Y() const { return m_y; }

    //======================================================================================================================
    inline scalar_t Vector4::Z() const { return m_z; }

    //======================================================================================================================
    inline scalar_t Vector4::W() const { return m_w; }

    //======================================================================================================================
    inline scalar_t& Vector4::X() { return m_x; }

    //======================================================================================================================
    inline scalar_t& Vector4::Y() { return m_y; }

    //======================================================================================================================
    inline scalar_t& Vector4::Z() { return m_z; }

    //======================================================================================================================
    inline scalar_t& Vector4::W() { return m_w; }

    //======================================================================================================================
    inline void Vector4::Set( scalar_t x, scalar_t y, scalar_t z, scalar_t w ) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    //======================================================================================================================
    inline void Vector4::Set( const Vector3 & v, scalar_t w ) {
        m_x = v.X();
        m_y = v.Y();
        m_z = v.Z();
        m_w = w;
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator-() const {
        return Vector4(-m_x, -m_y, -m_z, -m_w);
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator=( const Vector4 & rhs ) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator=( const Vector3& rhs ) {
        m_x = rhs.X();
        m_y = rhs.Y();
        m_z = rhs.Z();
        m_w = 0;
        return *this;
    }

    //======================================================================================================================
    inline scalar_t Vector4::Magnitude() const {
        return scalar::Sqrt( Dot( *this ) );
    }

    //======================================================================================================================
    inline scalar_t Vector4::Normalise( const Vector4 & rhs ) {
        scalar_t mag = rhs.Magnitude();
        scalar_t fac = 1.0f / mag;
        
        *this *= fac;
        return mag;
    }

    //======================================================================================================================
    inline scalar_t Vector4::Dot( const Vector4 & rhs ) const {
        return (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z) + (m_w * rhs.m_w);
    }

    //======================================================================================================================
    inline void Vector4::Mix(const Vector4 & from, const Vector4 & to, scalar_t t) {
        scalar_t tClamped = scalar::Clamp( t, 0, 1 );
        
        *this = (from * ( 1.0f - tClamped ) ) + ( to * tClamped ) ;
    }

    //======================================================================================================================
    inline void Vector4::Min( const Vector4 & lhs, const Vector4 & rhs ) {
        m_x = scalar::Min( lhs.m_x, rhs.m_x );
        m_y = scalar::Min( lhs.m_y, rhs.m_y );
        m_z = scalar::Min( lhs.m_z, rhs.m_z );
        m_w = scalar::Min( lhs.m_w, rhs.m_w );
    }

    //======================================================================================================================
    inline void Vector4::Max(const Vector4 & lhs, const Vector4 & rhs) {
        m_x = scalar::Max( lhs.m_x, rhs.m_x );
        m_y = scalar::Max( lhs.m_y, rhs.m_y );
        m_z = scalar::Max( lhs.m_z, rhs.m_z );
        m_w = scalar::Max( lhs.m_w, rhs.m_w );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator+( const Vector4 & rhs ) const {
        return Vector4( m_x + rhs.m_x,
                        m_y + rhs.m_y,
                        m_z + rhs.m_z,
                        m_w + rhs.m_w );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator-( const Vector4 & rhs ) const {
        return Vector4( m_x - rhs.m_x,
                        m_y - rhs.m_y,
                        m_z - rhs.m_z,
                        m_w - rhs.m_w );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator*( const Vector4 & rhs ) const {
        return Vector4( m_x * rhs.m_x,
                        m_y * rhs.m_y,
                        m_z * rhs.m_z,
                        m_w * rhs.m_w );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator/( const Vector4 & rhs ) const {
        return Vector4( m_x / rhs.m_x,
                        m_y / rhs.m_y,
                        m_z / rhs.m_z,
                        m_w / rhs.m_w );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator+( scalar_t rhs ) const {
        return *this + Vector4( rhs, rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator-( scalar_t rhs ) const {
        return *this - Vector4( rhs, rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator*( scalar_t rhs ) const {
        return *this * Vector4( rhs, rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector4 Vector4::operator/( scalar_t rhs ) const {
        return *this / Vector4( rhs, rhs, rhs, rhs );
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator+=( const Vector4 & rhs ) {
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        m_z += rhs.m_z;
        m_w += rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator-=( const Vector4 & rhs ) {
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        m_z -= rhs.m_z;
        m_w -= rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator*=( const Vector4 & rhs ) {
        m_x *= rhs.m_x;
        m_y *= rhs.m_y;
        m_z *= rhs.m_z;
        m_w *= rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator/=( const Vector4 & rhs ) {
        m_x /= rhs.m_x;
        m_y /= rhs.m_y;
        m_z /= rhs.m_z;
        m_w /= rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator+=( scalar_t rhs ) {
        *this += Vector4( rhs, rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator-=( scalar_t rhs ) {
        *this -= Vector4( rhs, rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator*=( scalar_t rhs ) {
        *this *= Vector4( rhs, rhs, rhs, rhs );
        return *this;
    }

    //======================================================================================================================
    inline Vector4 & Vector4::operator/=( scalar_t rhs ) {
        *this /= Vector4( rhs, rhs, rhs, rhs );
        return *this;
    }  

    //======================================================================================================================
    // Matrix3
    //======================================================================================================================  

    //======================================================================================================================
    inline Matrix3::Matrix3() {
    // Empty - does nothing
    }

    //======================================================================================================================
    inline Matrix3::Matrix3(const Matrix3 & rhs) {
    m_rows[ 0 ] = rhs.m_rows[ 0 ];
    m_rows[ 1 ] = rhs.m_rows[ 1 ];
    m_rows[ 2 ] = rhs.m_rows[ 2 ];
    }

    //======================================================================================================================
    inline Matrix3::Matrix3(const Vector3 & right, const Vector3 & up, const Vector3 & at) {
    m_rows[ 0 ] = right;
    m_rows[ 1 ] = up;
    m_rows[ 2 ] = at;
    }

    //======================================================================================================================
    inline Matrix3::Matrix3(float right, float up, float at) {
    m_rows[ 0 ].Set(right, 0, 0);
    m_rows[ 1 ].Set(0, up, 0);
    m_rows[ 2 ].Set(0, 0, at);
    }

    //======================================================================================================================
    inline void Matrix3::Set(const Vector3 & right, const Vector3 & up, const Vector3 & at) {
    m_rows[ 0 ] = right;
    m_rows[ 1 ] = up;
    m_rows[ 2 ] = at;
    }

    //======================================================================================================================
    inline void Matrix3::Set(float right, float up, float at) {
    m_rows[ 0 ].Set(right, 0, 0);
    m_rows[ 1 ].Set(0, up, 0);
    m_rows[ 2 ].Set(0, 0, at);
    }

    //======================================================================================================================
    inline void Matrix3::Concat(const Matrix3 & lhs, const Matrix3 & rhs) {
    m_rows[ 0 ] = rhs.m_rows[ 0 ] * lhs.m_rows[ 0 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 0 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 0 ].Z();
    
    m_rows[ 1 ] = rhs.m_rows[ 0 ] * lhs.m_rows[ 1 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 1 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 1 ].Z();
    
    m_rows[ 2 ] = rhs.m_rows[ 0 ] * lhs.m_rows[ 2 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 2 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 2 ].Z();
    }

    //======================================================================================================================
    inline void Matrix3::Transform(Vector3 & dst, const Vector3 & src) const {
    dst =   m_rows[ 0 ] * src.X() +
            m_rows[ 1 ] * src.Y() +
            m_rows[ 2 ] * src.Z();
    }

    //======================================================================================================================
    inline Vector3 Matrix3::operator*(const Vector3 & rhs) const {
        Vector3 res;
        Transform(res, rhs);
        return res;
    }

    //======================================================================================================================
    inline Matrix3 Matrix3::operator*(const Matrix3 & rhs) const {
        Matrix3 res;
        res.Concat(*this, rhs);
        return res;
    }

    //======================================================================================================================
    inline Matrix3 Matrix3::operator-() const {
        Matrix3 res;
        res.Inverse(*this);
        return res;
    }

    //======================================================================================================================
    inline Matrix3 & Matrix3::operator*=(const Matrix3 & rhs) {
        Matrix3 tmp = *this;
        Concat(tmp, rhs);
        return *this;
    }

    //======================================================================================================================
    // Matrix4
    //======================================================================================================================

    //======================================================================================================================
    inline Matrix4::Matrix4() {
        // Empty - does nothing
    }

    //======================================================================================================================
    inline Matrix4::Matrix4(const Matrix4& rhs) {
        m_rows[ 0 ] = rhs.m_rows[ 0 ];
        m_rows[ 1 ] = rhs.m_rows[ 1 ];
        m_rows[ 2 ] = rhs.m_rows[ 2 ];
        m_rows[ 3 ] = rhs.m_rows[ 3 ];
    }

    //======================================================================================================================
    inline Matrix4::Matrix4(const Vector4& right, const Vector4& up, const Vector4& at, const Vector4& pos) {
        m_rows[ 0 ] = right;
        m_rows[ 1 ] = up;
        m_rows[ 2 ] = at;
        m_rows[ 3 ] = pos;
    }

    //======================================================================================================================
    inline Matrix4::Matrix4(scalar_t right, scalar_t up, scalar_t at, scalar_t pos) {
        m_rows[ 0 ].Set(right, 0, 0, 0);
        m_rows[ 1 ].Set(0, up, 0, 0);
        m_rows[ 2 ].Set(0, 0, at, 0);
        m_rows[ 3 ].Set(0, 0, 0, pos);
    }

    //======================================================================================================================
    inline void Matrix4::Set(const Vector4& right, const Vector4& up, const Vector4& at, const Vector4& pos) {
        m_rows[ 0 ] = right;
        m_rows[ 1 ] = up;
        m_rows[ 2 ] = at;
        m_rows[ 3 ] = pos;
    }

    //======================================================================================================================
    inline void Matrix4::Set(scalar_t right, scalar_t up, scalar_t at, scalar_t pos) {
        m_rows[ 0 ].Set(right, 0, 0, 0);
        m_rows[ 1 ].Set(0, up, 0, 0);
        m_rows[ 2 ].Set(0, 0, at, 0);
        m_rows[ 3 ].Set(0, 0, 0, pos);
    }

    //======================================================================================================================
    inline void Matrix4::Concat(const Matrix4& lhs, const Matrix4& rhs) {
        m_rows[ 0 ] =   rhs.m_rows[ 0 ] * lhs.m_rows[ 0 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 0 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 0 ].Z() +
                    rhs.m_rows[ 3 ] * lhs.m_rows[ 0 ].W();
        
        m_rows[ 1 ] =   rhs.m_rows[ 0 ] * lhs.m_rows[ 1 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 1 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 1 ].Z() +
                    rhs.m_rows[ 3 ] * lhs.m_rows[ 1 ].W();
        
        m_rows[ 2 ] =   rhs.m_rows[ 0 ] * lhs.m_rows[ 2 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 2 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 2 ].Z() +
                    rhs.m_rows[ 3 ] * lhs.m_rows[ 2 ].W();
        
        m_rows[ 3 ] =   rhs.m_rows[ 0 ] * lhs.m_rows[ 3 ].X() +
                    rhs.m_rows[ 1 ] * lhs.m_rows[ 3 ].Y() +
                    rhs.m_rows[ 2 ] * lhs.m_rows[ 3 ].Z() +
                    rhs.m_rows[ 3 ] * lhs.m_rows[ 3 ].W();
    }

    //======================================================================================================================
    inline void Matrix4::Transform(Vector4& dst, const Vector4& src) const {
        dst =   m_rows[ 0 ] * src.X() +
                m_rows[ 1 ] * src.Y() +
                m_rows[ 2 ] * src.Z() +
                m_rows[ 3 ] * src.W();
    }

    //======================================================================================================================
    inline void Matrix4::Transform(Vector3& dst, const Vector3& src) const {
        Vector4 tmp =   m_rows[ 0 ] * src.X() +
                m_rows[ 1 ] * src.Y() +
                m_rows[ 2 ] * src.Z() +
                m_rows[ 3 ];
        
        dst.Set(tmp.X(), tmp.Y(), tmp.Z());
    }

    //======================================================================================================================
    inline void Matrix4::TransformVector(Vector3& dst, const Vector3& src) const {
        Vector4 tmp =  m_rows[ 0 ] * src.X() +
                    m_rows[ 1 ] * src.Y() +
                    m_rows[ 2 ] * src.Z();
        
        dst.Set(tmp.X(), tmp.Y(), tmp.Z());
    }

    //======================================================================================================================
    inline void Matrix4::Transform3(Vector3& dst, const Vector3& src) const {
        Vector4 tmp =   m_rows[ 0 ] * src.X() +
                m_rows[ 1 ] * src.Y() +
                m_rows[ 2 ] * src.Z();
        
        dst.Set(tmp.X(), tmp.Y(), tmp.Z());
    }


    //======================================================================================================================
    inline void Matrix4::Transpose( const Matrix4 & rhs ) {
        m_rows[ 0 ].Set ( rhs.m_rows[ 0 ].X(),   rhs.m_rows[ 1 ].X(),   rhs.m_rows[ 2 ].X(),   rhs.m_rows[ 3 ].X() );
        m_rows[ 1 ].Set ( rhs.m_rows[ 0 ].Y(),   rhs.m_rows[ 1 ].Y(),   rhs.m_rows[ 2 ].Y(),   rhs.m_rows[ 3 ].Y() );
        m_rows[ 2 ].Set ( rhs.m_rows[ 0 ].Z(),   rhs.m_rows[ 1 ].Z(),   rhs.m_rows[ 2 ].Z(),   rhs.m_rows[ 3 ].Z() );
        m_rows[ 3 ].Set ( rhs.m_rows[ 0 ].W(),   rhs.m_rows[ 1 ].W(),   rhs.m_rows[ 2 ].W(),   rhs.m_rows[ 3 ].W() );
    }

    //======================================================================================================================
    inline void Matrix4::SetTranslation( const Vector3& trans ) {
        m_rows[ 3 ].Set(trans, 1);
    }

    //======================================================================================================================
    inline Matrix4& Matrix4::operator=(const Matrix4& rhs) {
        m_rows[ 0 ] = rhs.m_rows[ 0 ];
        m_rows[ 1 ] = rhs.m_rows[ 1 ];
        m_rows[ 2 ] = rhs.m_rows[ 2 ];
        m_rows[ 3 ] = rhs.m_rows[ 3 ];
        return *this;
    }

    //======================================================================================================================
    inline Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
        Matrix4 res;
        res.Concat(*this, rhs);
        return res;
    }

    //======================================================================================================================
    inline Vector4 Matrix4::operator*(const Vector4& rhs) const {
        Vector4 tmp;
        Transform(tmp, rhs);
        return tmp;
    }

    //======================================================================================================================
    inline Vector3 Matrix4::operator*(const Vector3& rhs) const {
        Vector4 rhs4;
        rhs4.Set(rhs, 1);
        
        Vector4 tmp;
        Transform(tmp, rhs4);
        
        Vector3 res(tmp.X(), tmp.Y(), tmp.Z());
        
        return res;
    }

    //======================================================================================================================
    // Quaternion
    //======================================================================================================================

    //======================================================================================================================
    inline Quaternion::Quaternion() {
        // Empty - does nothing
    }

    //======================================================================================================================
    inline Quaternion::Quaternion(const Quaternion & rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;
    }

    //======================================================================================================================
    inline Quaternion::Quaternion(scalar_t x, scalar_t y, scalar_t z, scalar_t w) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    //======================================================================================================================
    inline scalar_t Quaternion::X() const { return m_x; }

    //======================================================================================================================
    inline scalar_t Quaternion::Y() const { return m_y; }

    //======================================================================================================================
    inline scalar_t Quaternion::Z() const { return m_z; }

    //======================================================================================================================
    inline scalar_t Quaternion::W() const { return m_w; }

    //======================================================================================================================
    inline scalar_t& Quaternion::X() { return m_x; }

    //======================================================================================================================
    inline scalar_t& Quaternion::Y() { return m_y; }

    //======================================================================================================================
    inline scalar_t& Quaternion::Z() { return m_z; }

    //======================================================================================================================
    inline scalar_t& Quaternion::W() { return m_w; }

    //======================================================================================================================
    inline void Quaternion::Set(scalar_t x, scalar_t y, scalar_t z, scalar_t w) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    //======================================================================================================================
    inline void Quaternion::Set(const Vector3 & axis, scalar_t angleRads) {

        scalar_t c = scalar::Cos( angleRads / 2.0f );
        scalar_t s = scalar::Sin( angleRads / 2.0f );

        m_x = axis.X() * s;
        m_y = axis.Y() * s;
        m_z = axis.Z() * s;
        m_w = c;
    }

    //======================================================================================================================
    inline void Quaternion::Set(const Vector3 & rhs) {
        scalar_t t = 1.0f - (rhs.X() * rhs.X()) - (rhs.Y() * rhs.Y()) - (rhs.Z() * rhs.Z());
        scalar_t w = 0;
        
        if (t < 0.0f) {
            w = 0;
        }
        else {
            w = -scalar::Sqrt( t );
        }

        m_x = rhs.X();
        m_y = rhs.Y();
        m_z = rhs.Z();
        m_w = w;
    }

    //======================================================================================================================
    inline scalar_t Quaternion::Magnitude() const {
        return std::sqrt( Dot( *this ) );
    }

    //======================================================================================================================
    inline Quaternion & Quaternion::Concat( const Quaternion & lhs, const Quaternion & rhs ) {

        m_x = rhs.m_x * lhs.m_w + rhs.m_y * lhs.m_z - rhs.m_z * lhs.m_y + rhs.m_w * lhs.m_x;
        m_y = -rhs.m_x * lhs.m_z + rhs.m_y * lhs.m_w + rhs.m_z * lhs.m_x + rhs.m_w * lhs.m_y;
        m_z = rhs.m_x * lhs.m_y - rhs.m_y * lhs.m_x + rhs.m_z * lhs.m_w + rhs.m_w * lhs.m_z;
        m_w = -rhs.m_x * lhs.m_x - rhs.m_y * lhs.m_y - rhs.m_z * lhs.m_z + rhs.m_w * lhs.m_w;

        return *this;
    }

    //======================================================================================================================
    inline scalar_t Quaternion::Dot( const Quaternion & rhs ) const {
        return m_x * rhs.m_x +
            m_y * rhs.m_y +
            m_z * rhs.m_z +
            m_w * rhs.m_w;
    }

    //======================================================================================================================
    inline scalar_t Quaternion::Normalise( const Quaternion & rhs ) {
        scalar_t len = Magnitude();
        scalar_t factor = 1.0f / len;
        
        m_x = rhs.m_x * factor;
        m_y = rhs.m_y * factor;
        m_z = rhs.m_z * factor;
        m_w = rhs.m_w * factor;
        
        return len;
    }
    
    //======================================================================================================================
    inline void Quaternion::Inverse( const Quaternion & rhs ) {
        
        Quaternion tmp;
        tmp.Conjugate( rhs);
        
        scalar_t mag = tmp.Normalise( tmp );
        
        *this = tmp / mag;
    }
    
    //======================================================================================================================
    inline void Quaternion::Inverse() {
        Inverse( *this );
    }

    //======================================================================================================================
    inline Quaternion Quaternion::operator-() const {
        return Quaternion( m_x, m_y, m_z, -m_w);
    }

    //======================================================================================================================
    inline Quaternion & Quaternion::operator=(const Quaternion & rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
        m_w = rhs.m_w;
        return *this;
    }

    //======================================================================================================================
    inline Quaternion Quaternion::operator*(const Quaternion & rhs) const {
        Quaternion tmp;
        tmp.Concat( *this, rhs);
        return tmp;
    }

    //======================================================================================================================
    inline Quaternion & Quaternion::operator*=(const Quaternion & rhs) {
        Quaternion tmp = *this;
        Concat(tmp, rhs);
        return *this;
    }
}

#endif
