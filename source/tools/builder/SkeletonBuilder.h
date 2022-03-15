//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SKSLETONLBUILDER_H__
#define __SKELETONBUILDER_H__

#include "BuilderTool.h"

class SkeletonBuilder : public BuilderTool {
    RTTI_CLASS_DECLARE( SkeletonBuilder, BuilderTool )
public:
    SkeletonBuilder();
    
    virtual ~SkeletonBuilder();
    
    virtual bool Run() override;
    
    virtual void GatherInputs( std::vector<i3d::stl::String::type> & inputs );
    
    virtual void GatherOutputs( std::vector<i3d::stl::String::type> & outputs );
    
    virtual const char * GetExeName() const override { return "modelbuilder"; }
    
public:
    i3d::stl::String::type     m_input;
    i3d::stl::String::type     m_rootNode;
    float           m_scale;
    bool            m_stripMixamo;
};

#endif
