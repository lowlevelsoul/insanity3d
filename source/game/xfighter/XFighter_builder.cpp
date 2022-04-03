//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter_builder.h"
#include "i3d/rtti/RttiLoader.h"
#include "i3d/rtti/RttiStream.h"

namespace i3d {
    extern void LogCreate();
    extern void LogDestroy();

    extern void FsCreate();
    extern void FsDestroy();

    extern void SysCreate();
    extern void SysDestroy();
    
    extern void RttiCreate();
    extern void RttiDestroy();
    
    extern void CVarCreate();
    extern void CVarDestroy();
}

const char * XFighterBuilder::FOLDERS[] = {
    "@/models",
    "@/textures",
    "@/materials",
    "@/prototypes",
    "@/entities",
    "@/rtti",
    nullptr,
};

XFighterBuilder builder;

namespace i3d {
    extern CVar res_buildPlatform;
}

//======================================================================================================================
int main( int argc, const char ** argv ) {
    builder.Run( argc, argv );
    return 0;
}

//======================================================================================================================
void XFighterBuilder::BuildFileCallback( i3d::stl::String::type & path, void * context ) {
    
#if 1
    i3d::PLATFORM_ID platId = i3d::GetPlatformIdFromString( i3d::res_buildPlatform.GetString() );
    XE_ERROR( platId == i3d::PLATFORM_ID_NONE, "Invalid platform value : %s\n", i3d::res_buildPlatform.GetString() );
    
    BuildContext * buildCtxt = (BuildContext*) context;

    i3d::stl::String::type assetPath = buildCtxt->m_folder.c_str();
    fs->AppendPath( assetPath, path.c_str() );
    
    const char * assetPathC = assetPath.c_str();
    
    i3d::RttiLoader loader;
    loader.Load( assetPath.c_str(), platId );

    if ( loader.m_object != nullptr ) {
        i3d::ResourceBuilder * tool = loader.m_object->SafeCast<i3d::ResourceBuilder>();
        
        BuildTool * bt = new BuildTool;
        bt->m_builder = tool;
        bt->m_builderPath = assetPath;
        
        builder.m_buildTasks.push_back( bt );
    }
    else {
        XE_ERROR( false, "Not a valid build file. Contains no objects derived from ResourceBuilder.\n");
    }
#endif
}

//======================================================================================================================
XFighterBuilder::XFighterBuilder() {
    
}

//======================================================================================================================
XFighterBuilder::~XFighterBuilder() {
    
}

//======================================================================================================================
void XFighterBuilder::Run( int argc, const char ** argv ) {
    i3d::LogCreate();
    logsys->PublishReporter( &m_logReporter );
    
    i3d::SysCreate();
    i3d::FsCreate();
    
    i3d::RttiCreate();
    i3d::CVarCreate();
    
    ProcessCmdLine( argc, argv );
    
    // Must have a valid platform
    i3d::PLATFORM_ID platId = i3d::GetPlatformIdFromString( i3d::res_buildPlatform.GetString() );
    XE_ERROR( platId == i3d::PLATFORM_ID_NONE, "Invalid platform value : %s\n", i3d::res_buildPlatform.GetString() );
    
    // Make sure the data and asset folder has been set accordingly
    i3d::stl::String::type dataFolder = "data";
    fs->AppendPath( dataFolder, i3d::res_buildPlatform.GetString() );
    fs->AppendPath( dataFolder, "data" );
    
    fs->SetDataMountFolder( dataFolder.c_str() );
    fs->SetAssetMountFolder( "assets" );
    
    size_t lastBuildCount = 0;
    
    for ( uint32_t f = 0; FOLDERS[ f ] != nullptr; ++f ) {
        
        XE_LOG("Scanning %s for meta files...", FOLDERS[ f ] );
        ScanFolder( FOLDERS[ f ] );
        XE_LOG("Done.\n");
        
        size_t numAdded = m_buildTasks.size() - lastBuildCount;
        lastBuildCount = m_buildTasks.size();
        XE_LOG("    Found %u build files\n", numAdded );
    }
    
    XE_LOG("Performing build tasks\n");
    
    for ( auto bt : m_buildTasks ) {
        
        i3d::stl::String::type dstPath = bt->m_builderPath.c_str();
        fs->RemoveExtension( dstPath );
        dstPath[0] = '~';
        
        bool shouldBuild = NeedsBuild( bt->m_builder, dstPath.c_str() );
        if ( shouldBuild ) {
            XE_LOG(    "%s is stale. Building.\n", dstPath.c_str() );
            bt->m_builder->Compile( dstPath.c_str() );
        }
    }
}

//======================================================================================================================
void XFighterBuilder::ScanFolder( const char * folder ) {
    i3d::stl::String::type fullPath;
    fs->MakeFullPath(fullPath, folder );
    
    BuildContext ctxt;
    ctxt.m_folder = folder;
    
    fs->EnumeratePathConents( fullPath.c_str(), BuildFileCallback, &ctxt, "build");
}

//======================================================================================================================
void XFighterBuilder::ProcessCmdLine( int argc, const char ** argv ) {
    
    std::string varName;
    std::string varValue;
    bool haveVar = false;
    
    for ( int i = 0; i <= argc; ++i ) {
        
        const char * currArg = argv[ i ];
        
        if ( i == argc || currArg[0] == '+' ) {
            
            if ( haveVar == true && varValue.empty() == false ) {
                i3d::CVar * var = cvars->Find( varName.c_str() );
                if ( var != nullptr ) {
                    XE_LOG( "Setting %s = %s\n", varName.c_str(), varValue.c_str() );
                    var->Set( varValue.c_str() );
                }
            }
            
            if ( i < argc ) {
                varName = ( currArg + 1 );
                varValue.clear();
                haveVar = true;
            }
        }
        else if ( varName.empty() == false ) {
            if ( varValue.empty() == false ) {
                varValue.append(";");
            }
            
            varValue.append( currArg );
        }
    }
}


//======================================================================================================================
bool XFighterBuilder::NeedsBuild( i3d::ResourceBuilder * builder, const char * path ) {
    
    i3d::stl::Vector<i3d::stl::String::type>::type inputs;
    
    if ( fs->DoesFileExist( path )  == false ) {
        // The file we're checking, doesn't actually exist. So just bail with true,
        // meaning that the resource needs to be compiled
        return true;
    }
        
    inputs.push_back( path );
    builder->GetDependencies( nullptr, inputs);
    
    // Get the respufe target file time stamp
    uint64_t resFileTimeStamp;
    fs->GetModifiedTimestamp( resFileTimeStamp, path );
    
    // Get the time stamps of the inputs
    i3d::stl::Vector<uint64_t>::type inputTimeStamps;
    
    for ( auto & i : inputs ) {
        if ( fs->DoesFileExist( i.c_str() ) == true ) {
            uint64_t timestamp;
            fs->GetModifiedTimestamp(timestamp, i.c_str() );
            inputTimeStamps.push_back( timestamp );
        }
    }

    // Check all of the output file timestamp agains the input timestamps. If the
    // input timestamps are newere than any of the outputs, we need to compile the
    // resource
    for ( auto inputTs : inputTimeStamps) {
        if (resFileTimeStamp < inputTs) {
            return true;
        }
    }
    
    // If we get to hear, the resource is up-to-date.
    return false;
}
