//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SCALAR_H__
#define __I3D_SCALAR_H__

namespace i3d {

    typedef float scalar_t;

    namespace scalar {
        
        static const scalar_t PI = 3.14159265359f;
        
        inline scalar_t DegToRad( scalar_t rhs ) { return rhs * (PI / 180.0f); }
        inline scalar_t RadToDeg( scalar_t rhs ) { return rhs * (180.0f / PI); }
        
        inline bool IsNan( scalar_t rhs ) { return (rhs == rhs); }
        inline bool Abs( scalar_t rhs ) { return std::fabs( rhs ); }
        
        inline scalar_t Cos( scalar_t rhs ) { return std::cos(rhs); }
        inline scalar_t Sin( scalar_t rhs ) { return std::sin(rhs); }
        inline scalar_t Tan( scalar_t rhs ) { return std::tan(rhs); }
        inline scalar_t Acos( scalar_t rhs ) { return std::acos(rhs); }
        inline scalar_t Asin( scalar_t rhs ) { return std::asin(rhs); }
        inline scalar_t Atan( scalar_t rhs ) { return std::atan(rhs); }
        inline scalar_t Atan2( scalar_t lhs, scalar_t rhs) { return std::atan2f(lhs, rhs); }
        
        inline scalar_t Sqrt(scalar_t rhs ) { return sqrtf(rhs); }
        inline scalar_t Min( scalar_t lhs, scalar_t rhs ) { return ::fmin(lhs, rhs); }
        inline scalar_t Max( scalar_t lhs, scalar_t rhs ) { return ::fmax(lhs, rhs); }
        inline scalar_t Clamp( scalar_t val, scalar_t limitMin, scalar_t limitMax ) { return scalar::Max( limitMin, scalar::Min( limitMax, val ) ); }
        
        //======================================================================================================================
        inline scalar_t Mix( scalar_t from, scalar_t to, scalar_t t ) {
            return (from * ( scalar_t( 1 ) - t ) ) + ( to * t );
        }

        //======================================================================================================================
        inline scalar_t MixSafe( scalar_t from, scalar_t to, scalar_t t ) {
            scalar_t clampedT = scalar::Clamp( t, 0, 1 );
            return Mix( from, to, clampedT );
        }
        
        //======================================================================================================================
        template<typename __type__>
        inline bool BitScan(int32_t& index, const __type__& val) {
            if (val == 0) {
                return false;
            }
            
            int32_t bitIndex = 0;
            while ((val & __type__(1 << bitIndex)) != 0) {
                ++bitIndex;
            }
            
            index = bitIndex;
            return true;
        }
        
        //======================================================================================================================
        template<typename __type__>
        inline bool BitScanReverse(int32_t& index, const __type__& val) {
            if (val == 0) {
                
                return false;
            }
            
            int32_t bitIndex = (sizeof(__type__) * 8) - 1;
            while ((val & __type__(1 << bitIndex)) == 0) {
                --bitIndex;
            }
            
            index = bitIndex;
            return true;
        }
        
        //======================================================================================================================
        template<typename __type__>
        inline bool IsPowerOfTwo(const __type__& rhs) {
            __type__ tmp = rhs & (rhs-1);
            return (tmp == 0);
        }
        
        //======================================================================================================================
        template<typename __type__>
        inline __type__ GetNextPowerOfTwo(const __type__& rhs) {

            if ((rhs == 0)  || (IsPowerOfTwo(rhs))) {
                return rhs;
            }
            
            int32_t msbIndex = -1;
            BitScanReverse(msbIndex, rhs);
            
            __type__ tmp = (1 << msbIndex) - 1;
            tmp &= rhs;
            if (tmp != 0) {
                ++msbIndex;
            }
            
            return (1 << msbIndex);
        }
    }
}

#endif
