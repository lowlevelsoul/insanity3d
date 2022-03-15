//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __BUILDERTOOL_H__
#define __BUILDERTOOL_H__

class Builder;

class BuilderTool : public i3d::RttiObject {
    RTTI_CLASS_DECLARE( BuilderTool, i3d::RttiObject )
public:
    BuilderTool();
    
    virtual ~BuilderTool();
    
    virtual bool Run();
    
    i3d::stl::String::type MakeInputFilePath( const char * inputFile );
    
    i3d::stl::String::type MakeOutputFilePath( const char * ext );
    
    i3d::stl::String::type GetToolExePath();
    
    virtual void GatherInputs( std::vector<i3d::stl::String::type> & inputs );
    
    virtual void GatherOutputs( std::vector<i3d::stl::String::type> & outputs );
    
    bool CheckStale();
    
    virtual const char * GetExeName() const;
    
    const i3d::stl::String::type GetMetaFullPath() const;
    
public:
    i3d::stl::String::type          m_inputPath;        ///<
    i3d::stl::String::type          m_outputPath;       ///<
    Builder *                       m_builder;          ///< Pointer to the builder class that owns this tool
    i3d::stl::String::type          m_metaPath;         ///< Path to the metal file that was used to create this tool instance
};

#endif
