//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __BUILDERTOOL_H__
#define __BUILDERTOOL_H__

#include "toolapp/RttiType.h"

class Builder;

class BuilderTool : public RttiObject {
    RTTI_CLASS_DECLARE( BuilderTool, RttiObject )
public:
    BuilderTool();
    
    virtual ~BuilderTool();
    
    virtual bool Run();
    
    std::string MakeInputFilePath( const char * inputFile );
    
    std::string MakeOutputFilePath( const char * ext );
    
    std::string GetToolExePath();
    
    virtual void GatherInputs( std::vector<std::string> & inputs );
    
    virtual void GatherOutputs( std::vector<std::string> & outputs );
    
    bool CheckStale();
    
    virtual const char * GetExeName() const;
    
    const std::string GetMetaFullPath() const;
    
public:
    std::string         m_inputPath;        ///<
    std::string         m_outputPath;       ///<
    Builder *           m_builder;          ///< Pointer to the builder class that owns this tool
    std::string         m_metaPath;         ///< Path to the metal file that was used to create this tool instance
};

#endif
