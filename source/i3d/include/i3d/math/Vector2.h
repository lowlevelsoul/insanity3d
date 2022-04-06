//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_VECTOR2_H__
#define __I3D_VECTOR2_H__

#include "i3d/math/Scalar.h"

#define VEC2_OP_BINARY(OP) Vector2 operator OP ( const Vector2& rhs) const { return Vector2(m_x OP rhs.m_x, m_y OP rhs.m_y); }
#define VEC2_OP_BINARY_SCALAR(OP) Vector2 operator OP ( float rhs) const { return Vector2(m_x OP rhs, m_y OP rhs); }

#define VEC2_OP_UNARY(OP) Vector2& operator OP (const Vector2& rhs) { m_x OP rhs.m_x; m_y OP rhs.m_y; return *this; }
#define VEC2_OP_UNARY_SCALAR(OP) Vector2& operator OP (float rhs) { m_x OP rhs; m_y OP rhs; return *this; }

namespace i3d {

    class Vector2 {
    protected:
        float m_x;
        float m_y;
    public:
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 ZERO;
        
        Vector2();
        
        Vector2(const Vector2& rhs);
        
        Vector2(float x, float y);
        
        float X() const;
        float Y() const;
        
        float& X();
        float& Y();
        
        void Set(float x, float y);
        
        Vector2 operator-() const;
        
        Vector2& operator=(const Vector2& rhs);
        
        float Magnitude() const;
        
        float Normalise(const Vector2& rhs);
        
        float Dot(const Vector2& rhs) const;
        
        void Lerp(const Vector2& from, const Vector2& to, float t);
        
        void Min(const Vector2& lhs, const Vector2& rhs);
        
        void Max(const Vector2& lhs, const Vector2& rhs);
        
        VEC2_OP_BINARY(+)
        VEC2_OP_BINARY(-)
        VEC2_OP_BINARY(*)
        VEC2_OP_BINARY(/)
        VEC2_OP_BINARY_SCALAR(+)
        VEC2_OP_BINARY_SCALAR(-)
        VEC2_OP_BINARY_SCALAR(*)
        VEC2_OP_BINARY_SCALAR(/)
        VEC2_OP_UNARY(+=)
        VEC2_OP_UNARY(-=)
        VEC2_OP_UNARY(*=)
        VEC2_OP_UNARY(/=)
        VEC2_OP_UNARY_SCALAR(+=)
        VEC2_OP_UNARY_SCALAR(-=)
        VEC2_OP_UNARY_SCALAR(*=)
        VEC2_OP_UNARY_SCALAR(/=)
    };
    
    //======================================================================================================================
    inline Vector2::Vector2() {
        // Empty - does nothing
    }

    //======================================================================================================================
    inline Vector2::Vector2(const Vector2& rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
    }

    //======================================================================================================================
    inline Vector2::Vector2(float x, float y) {
        m_x = x;
        m_y = y;
    }

    //======================================================================================================================
    inline float Vector2::X() const { return m_x; }

    //======================================================================================================================
    inline float Vector2::Y() const { return m_y; }

    //======================================================================================================================
    inline float& Vector2::X() { return m_x; }

    //======================================================================================================================
    inline float& Vector2::Y() { return m_y; }

    //======================================================================================================================
    inline void Vector2::Set(float x, float y) {
        m_x = x;
        m_y = y;
    }

    //======================================================================================================================
    inline Vector2 Vector2::operator-() const {
        return Vector2(-m_x, -m_y);
    }

    //======================================================================================================================
    inline Vector2& Vector2::operator=(const Vector2& rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        return *this;
    }

    //======================================================================================================================
    inline float Vector2::Magnitude() const {
        return std::sqrt(Dot(*this));
    }

    //======================================================================================================================
    inline float Vector2::Normalise(const Vector2& rhs) {
        float mag = rhs.Magnitude();
        float fac = 1.0f / mag;

        *this = rhs * fac;
        return mag;
    }

    //======================================================================================================================
    inline float Vector2::Dot(const Vector2& rhs) const {
        return (m_x * rhs.m_x) + (m_y * rhs.m_y);
    }

    //======================================================================================================================
    inline void Vector2::Lerp(const Vector2& from, const Vector2& to, float t) {
        float tClamped = t;
        *this = (from * (1.0f - tClamped)) + (to * tClamped);
    }

    //======================================================================================================================
    inline void Vector2::Min(const Vector2& lhs, const Vector2& rhs) {
        m_x = std::fmin(lhs.m_x, rhs.m_x);
        m_y = std::fmin(lhs.m_y, rhs.m_y);
    }

    //======================================================================================================================
    inline void Vector2::Max(const Vector2& lhs, const Vector2& rhs) {
        m_x = std::fmax(lhs.m_x, rhs.m_x);
        m_y = std::fmax(lhs.m_y, rhs.m_y);
    }
}

#endif
