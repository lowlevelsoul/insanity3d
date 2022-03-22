//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RTTICOMPILER_H__
#define __I3D_RTTICOMPILER_H__

#include "i3d/util/ToolMemStream.h"
#include "i3d/res/ResourceBuilder.h"

namespace i3d {

    class RttiBuilder : public ResourceBuilder {
    public:
        RTTI_CLASS_DECLARE( RttiBuilder, ResourceBuilder )
        
        RttiBuilder();
        
        virtual ~RttiBuilder();
        
        virtual void GetDependencies( Resource * resource,
                                      stl::Vector<stl::String::type>::type & inputs );
        
        virtual void Compile( const char * path );
        
    public:
        stl::String::type      m_input;
    };
}

#endif 
