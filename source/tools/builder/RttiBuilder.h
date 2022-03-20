//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __RTTIBUILDER_H__
#define __RTTIBUILDER_H__

#include "BuilderTool.h"

class RttiBuilder : public BuilderTool {
    RTTI_CLASS_DECLARE( RttiBuilder, BuilderTool )
public:
    RttiBuilder();
    
    virtual ~RttiBuilder();
    
    virtual bool Run() override;
    
    virtual void GatherInputs( std::vector<i3d::stl::String::type> & inputs );
    
    virtual void GatherOutputs( std::vector<i3d::stl::String::type> & outputs );
    
public:
    i3d::stl::String::type      m_input;
    i3d::stl::String::type      m_outputExt;
};

#endif
