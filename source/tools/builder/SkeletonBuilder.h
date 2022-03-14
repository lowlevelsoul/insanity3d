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
    
    virtual void GatherInputs( std::vector<std::string> & inputs );
    
    virtual void GatherOutputs( std::vector<std::string> & outputs );
    
    virtual const char * GetExeName() const override { return "modelbuilder"; }
    
public:
    std::string     m_input;
    std::string     m_rootNode;
    float           m_scale;
    bool            m_stripMixamo;
};

#endif
