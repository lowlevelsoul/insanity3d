//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ToolApp.h"
#include "i3d/core/Hash.h"
#include "i3d/core/FileSystem.h"
#include "i3d/core/Log.h"

namespace i3d {
    extern void LogCreate();
    extern void LogDestroy();

    extern void FsCreate();
    extern void FsDestroy();

    extern void SysCreate();
    extern void SysDestroy();
    
    extern void RttiCreate();
    extern void RttiDestroy();
}



/// The interface for printing out the log messages
class ToolPrintReporter: public i3d::LogReporter {
public:
    virtual ~ToolPrintReporter() {
    }

    virtual void Printf(const i3d::LogCat_t& cat, const char* fmt, va_list vargs) {
        i3d::stl::String::type formatedStr = tool->Vformat(fmt, vargs);
        printf("%s", formatedStr.c_str());
    }
};

ToolPrintReporter toolLogReporter;

//======================================================================================================================
void ToolEngineInitialise() {
    i3d::LogCreate();
    logsys->PublishReporter( &toolLogReporter );
    
    i3d::SysCreate();
    i3d::FsCreate();
    
    i3d::RttiCreate();
}

//======================================================================================================================
void ToolEngineFinalise() {
    
}


//======================================================================================================================
ToolApp::ToolApp() {
    
}

//======================================================================================================================
ToolApp::~ToolApp() {
    
}

//======================================================================================================================
bool ToolApp::Run(int32_t argc, const char * argv[]) {
    BuildArgList(argc, argv);
    
    for(uint32_t a = 0; a < m_args.size(); ++a) {
        
        ToolArg * arg = m_args[a].get();
        
        uint64_t hash = i3d::fh64::CalcFromString(arg->m_name);
        arg_enum_map_t::iterator findIt = m_argEnums.find(hash);
        if (findIt != m_argEnums.end()) {
            HandleArg(*arg, findIt->second);
        }
    }
    
    return Run();
}

//======================================================================================================================
bool ToolApp::Run() {
    return true;
}

//======================================================================================================================
bool ToolApp::HandleArg(ToolArg& arg, uint32_t argEnum) {
    return true;
}

//======================================================================================================================
void ToolApp::PublishArgEnum(const char * argName, uint32_t argEnum) {
    uint64_t hash = i3d::fh64::CalcFromString(argName);
    m_argEnums[hash] = argEnum;
}

//======================================================================================================================
void ToolApp::BuildArgList(int32_t argc, const char * argv[]) {
    ToolArg * currArg = nullptr;
    
    for(int32_t a = 0; a < argc; ++a) {
        const char* currArgv = argv[a];
        if (currArgv[0] == '+') {
            
            currArg = new ToolArg;
            currArg->m_name = currArgv+1;
            
            m_args.push_back(ToolArg::unique_ptr_t(currArg));
        }
        else if (currArg != nullptr) {
            currArg->m_params.push_back(currArgv);
        }
    }
}

//======================================================================================================================
bool ToolApp::CreateFolderAtPath( const char * path ) {
    
    i3d::stl::String::type actualPath = path;
    i3d::stl::String::type filename;
    fs->ExtractFilename(filename, actualPath);
    
    bool createOk = fs->CreateFolder( actualPath.c_str(), true );
    return createOk == true;
}

//======================================================================================================================
const i3d::stl::String::type ToolApp::Vformat(const char* fmt, ...) {
    // initialize use of the variable argument array
    va_list vaArgs, vaArgsCopy;
    va_start(vaArgs, fmt);
    
    // reliably acquire the size
    // from a copy of the variable argument array
    // and a functionally reliable call to mock the formatting
    va_copy(vaArgsCopy, vaArgs);
    size_t len = std::vsnprintf(NULL, 0, fmt, vaArgsCopy);
    va_end(vaArgsCopy);
    
    i3d::stl::String::type ret;
    ret.resize(len);

    // return a formatted string without risking memory mismanagement
    // and without assuming any compiler or platform specific behavior
    std::vsnprintf(&ret[0], len+1, fmt, vaArgs);
    va_end(vaArgs);
    
    return ret;
}

//======================================================================================================================
const i3d::stl::String::type ToolApp::Vformat(const char* fmt, va_list vaArgs) {
    // initialize use of the variable argument array
    va_list vaArgsCopy;
    
    // reliably acquire the size
    // from a copy of the variable argument array
    // and a functionally reliable call to mock the formatting
    va_copy(vaArgsCopy, vaArgs);
    size_t len = std::vsnprintf(NULL, 0, fmt, vaArgsCopy);
    va_end(vaArgsCopy);
    
    i3d::stl::String::type ret;
    ret.resize(len);

    // return a formatted string without risking memory mismanagement
    // and without assuming any compiler or platform specific behavior
    std::vsnprintf(&ret[0], len+1, fmt, vaArgs);
    va_end(vaArgs);
    
    return ret;
}
