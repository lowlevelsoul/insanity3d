//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RTTISCRIPTRESOURCE_H__
#define __I3D_RTTISCRIPTRESOURCE_H__

#include "i3d/res/Resource.h"
#include "i3d/rtti/RttiObject.h"

namespace i3d {

    class RttiScriptResource : public Resource {
    public:
        DECLARE_RESOURCE( RttiScriptResource )
        
        RttiScriptResource();
        
        virtual ~RttiScriptResource();
        
        virtual void Load( File * file, const char * path ) override;
        
    protected:
        RttiObject *        m_object;           ///< Pointer to the rtti object that was loaded
    };
}

#endif

