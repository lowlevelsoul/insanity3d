//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __MODELBUILDER_H__
#define __MODELBUILDER_H__

#include "BuilderTool.h"

class ModelBuilder : public BuilderTool {
    RTTI_CLASS_DECLARE( ModelBuilder, BuilderTool )
public:
    ModelBuilder();
    
    virtual ~ModelBuilder();
    
    virtual bool Run() override;
    
    virtual void GatherInputs( std::vector<std::string> & inputs );
    
    virtual void GatherOutputs( std::vector<std::string> & outputs );
    
    virtual const char * GetExeName() const override { return "modelbuilder"; }
    
public:
    std::string     m_input;
    float           m_scale;
    bool            m_flipFaces;
    bool            m_genNormals;
    std::string     m_rootNode;
    bool            m_stripMixamo;
};

#endif
