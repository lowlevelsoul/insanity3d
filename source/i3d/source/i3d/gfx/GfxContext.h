//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXCONTEXT_H__
#define __GFXCONTEXT_H__

namespace i3d { namespace gfx {
    
    class Context {
    public:
        struct Params {
            uint32_t        m_bufferCount;
            void *          m_nativeView;
            
            Params() {
                m_bufferCount = 3;
                m_nativeView = nullptr;
            }
        };
        
        virtual ~Context() {}
        
        virtual void Create( Params & params ) = 0;
        
        virtual void GetDrawableSize( uint32_t & width, uint32_t & height ) = 0;
    };
}}

#endif
