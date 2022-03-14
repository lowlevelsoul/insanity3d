//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TEXBUILDERAPP_H__
#define __TEXBUILDERAPP_H__

#include "toolapp/ToolApp.h"
#include "texbuilder/TexBuilder.h"
#include "texbuilder/AmrBuilder.h"


class TexBuilderApp : public ToolApp {
public:
    bool                m_displayHelp;
    bool                m_displayVersion;
    bool                m_buildAmr;
    AmrBuilder          m_amrBuilder;
    TexBuilder          m_texgen;
    
    TexBuilderApp();
    
    virtual ~TexBuilderApp();
    
    void DisplayHelp(int32_t helpItem = -1);
    
    void DisplayBlockFormats();
    
    virtual bool Run() override;
    
    virtual bool HandleArg( ToolArg& arg, uint32_t argEnum );
    
    bool WriteOutputImage( std::string& errorMsg );
};

#endif
