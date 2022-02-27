//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_RTTICOMPILER_H__
#define __XENGINE_RTTICOMPILER_H__

#include "xe/util/ToolMemStream.h"
#include "xe/res/ResourceCompiler.h"

namespace xe {

    class RttiCompiler : public xe::ResourceCompiler {
    public:
        RTTI_CLASS_DECLARE( RttiCompiler, ResourceCompiler )
        
        RttiCompiler();
        
        virtual ~RttiCompiler();
        
        virtual void GetDependencies( Resource * resource,
                                      xe::Array<xe::String> & inputs );
        
        virtual void Compile( Resource * resource );
        
    public:
        xe::String      m_input;
    };
}

#endif 
