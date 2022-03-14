//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TOOLAPP_H__
#define __TOOLAPP_H__

#include <vector>
#include <memory>
#include <map>
#include <string>

class ToolArg {
public:
    typedef std::unique_ptr<ToolArg> unique_ptr_t;
    typedef std::vector<unique_ptr_t> vector_t;
    
    const char*                 m_name;
    std::vector<const char*>    m_params;
    
    bool HasParams() { return m_params.empty() == false; }
    size_t GetParamCount() { return m_params.size(); }
    
    void AddParam(const char* param) { m_params.push_back(param); }
};

class ToolApp {
public:
    typedef std::map<uint64_t, uint32_t> arg_enum_map_t;
    ToolArg::vector_t               m_args;
    arg_enum_map_t                  m_argEnums;
    
    ToolApp();
    
    virtual ~ToolApp();
    
    bool Run(int32_t argc, const char * argv[]);
    
    virtual bool Run();
    
    virtual bool HandleArg(ToolArg& arg, uint32_t argEnum);
    
    const i3d::stl::String::type Vformat(const char* fmt, ...);
    
    const i3d::stl::String::type Vformat(const char* fmt, va_list vaArgs);
    
    bool CreateFolderAtPath( const char * path );
    
protected:
    void PublishArgEnum(const char * argName, uint32_t argEnum);
    
    void BuildArgList(int32_t argc, const char * argv[]);
};

extern ToolApp* tool;

#define TOOL_APP(__tool__)\
ToolApp * tool = nullptr;\
static uint8_t toolAppMem[sizeof(__tool__)];\
extern void ToolEngineInitialise();\
extern void ToolEngineFinalise();\
int main(int argc, const char* argv[]) {\
    ToolEngineInitialise();\
    tool = new(toolAppMem) __tool__;\
    bool ok = tool->Run(argc, argv);\
    ((__tool__*)tool)->~__tool__();\
    tool = nullptr;\
    ToolEngineFinalise();\
    return (ok == true) ? -1 : 0;\
}

#endif
