//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "BuilderTool.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( BuilderTool )
RTTI_CLASS_END( BuilderTool )


//======================================================================================================================
BuilderTool::BuilderTool() {
    m_builder = nullptr;
}

//======================================================================================================================
BuilderTool::~BuilderTool() {
    
}

//======================================================================================================================
bool BuilderTool::Run() {
    return false;
}

//======================================================================================================================
void BuilderTool::GatherInputs( std::vector<i3d::stl::String::type> & inputs ) {
    
}

//======================================================================================================================
void BuilderTool::GatherOutputs( std::vector<i3d::stl::String::type> & outputs ) {
    
}

//======================================================================================================================
i3d::stl::String::type BuilderTool::MakeInputFilePath( const char * inputFile ) {
    // Join the input base path and the path to the metal file to give
    // us the folder location of the input file
    i3d::stl::String::type inputFilePath = m_inputPath;
    fs->AppendPath(inputFilePath, m_metaPath.c_str());
    
    // Extract the file name to remove the metal file and add the input file name
    i3d::stl::String::type temp;
    fs->ExtractFilename(temp, inputFilePath);
    fs->AppendPath( inputFilePath, inputFile );
    
    return inputFilePath;
}

//======================================================================================================================
i3d::stl::String::type BuilderTool::MakeOutputFilePath( const char * ext ) {
    i3d::stl::String::type outFilePath = m_outputPath;
    
    const char * metaPath = m_metaPath.c_str();
    const char * outPath = outFilePath.c_str();
    
    fs->AppendPath( outFilePath, m_metaPath.c_str() );
    fs->StripExtension( outFilePath );
    fs->AppendExtension( outFilePath, ext );
    
    return outFilePath;
}

//======================================================================================================================
i3d::stl::String::type BuilderTool::GetToolExePath() {
    i3d::stl::String::type appPath, toolName;
    fs->GetApplicationPath( appPath );
    fs->ExtractFilename( toolName, appPath);
    fs->AppendPath( appPath, GetExeName() );
    
    return appPath;
}

//======================================================================================================================
const char * BuilderTool::GetExeName() const {
    return "";
}

//======================================================================================================================
bool BuilderTool::CheckStale() {
    // Gathe inpit and outputs for this tool
    std::vector<i3d::stl::String::type> inputs;
    GatherInputs( inputs );
    
    std::vector<i3d::stl::String::type> outputs;
    GatherOutputs( outputs );
    
    // If either the inputs or outpurs are empty, we'll just assume that the
    // output is stale and needed to be built.
    if (inputs.empty() == true || outputs.empty() == true) {
        return true;
    }

    // Add the tool exe and the metal file to the inputs. They may have changed or been
    // updated and will trigger a build.
    inputs.push_back( GetToolExePath() );
    inputs.push_back( GetMetaFullPath() );
    
    for ( auto & o : outputs ) {
        
        // Check to see if the output exists. If not, then the tool output
        // is stale and needed to be built
        bool exists = fs->DoesFileExist( o.c_str() );
        if ( exists == false ) {
            return true;
        }
        
        uint64_t outputTimeStamp;
        fs->GetModifiedTimestamp(outputTimeStamp, o.c_str());
        
        for ( auto & i : inputs ) {
            exists = fs->DoesFileExist( i.c_str() );
            XE_ERROR(exists == false, "Input file %s does not exist!\n", i.c_str() );
            
            uint64_t inputTimeStamp;
            fs->GetModifiedTimestamp( inputTimeStamp, i.c_str());
            
            // If the input is newer than the output, then we the output
            // is stale and needs to be built.
            if ( inputTimeStamp > outputTimeStamp ) {
                return true;
            }
        }
    }
    
    // If we get to here the output is not stale and we don't need to build it
    return false;
}

//======================================================================================================================
const i3d::stl::String::type BuilderTool::GetMetaFullPath() const {
    i3d::stl::String::type path = m_inputPath;
    fs->AppendPath( path, m_metaPath.c_str() );
    return path;
}
