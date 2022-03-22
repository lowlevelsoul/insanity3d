//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_BUILDER_H__
#define __XFIGHTER_BUILDER_H__

#include "i3d/res/ResourceBuilder.h"
#include "i3d/util/RefObject.h"
#include "i3d/util/RefPointer.h"

class XFighterBuilder {
public:
    static const char * FOLDERS[];

    XFighterBuilder();
    
    virtual ~XFighterBuilder();
    
    void Run( int argc, const char ** argv );

    void ScanFolder( const char * folder );

protected:

    static void BuildFileCallback( i3d::stl::String::type & path, void * context );
    
    void ProcessCmdLine( int argc, const char ** argv );
    
    bool NeedsBuild( i3d::ResourceBuilder * builder, const char * path );
    
    class BuildTool : public i3d::RefObject {
    public:
        BuildTool() {
            m_builder = nullptr;
        }
        
        virtual ~BuildTool() {
            SAFE_DELETE( m_builder );
        }
        
    public:
        i3d::ResourceBuilder *      m_builder;
        std::string                 m_builderPath;
    };
    
    struct BuildContext {
        std::string     m_folder;
    };

public:
    std::vector<BuildTool*>     m_buildTasks;
    i3d::LogReporterDebug       m_logReporter;
};

#endif
