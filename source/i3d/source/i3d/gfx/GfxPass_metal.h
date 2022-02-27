//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXPASS_METAL_H__
#define __I3D_GFXPASS_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxPass.h"

namespace i3d { namespace gfx {
    
    class PassMetal : public Pass {
    public:
        PassMetal();
        
        virtual ~PassMetal();
        
        virtual void Create( const char * label ) override;
        
    public:
        const char *                m_label;
        id<MTLRenderCommandEncoder> m_encoder;
    };
}}


#endif
