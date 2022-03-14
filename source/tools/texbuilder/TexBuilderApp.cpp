//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "texbuilder/TexBuilderApp.h"

TOOL_APP(TexBuilderApp)

enum ARG {
    ARG_HELP,
    ARG_VERSION,
    ARG_INFILE,
    ARG_OUTFILE,
    ARG_SIZE,
    ARG_BLOCK,
    ARG_GENMIPS,
    ARG_AMR,
    ARG_ROUGHNESS,
    ARG_METALLIC,
    ARG_AMBIENT,
    ARG_COUNT,
};

// 0.1.1        - Added support in base tool for platform profiles
// 0.1.0        - Initial version
static const uint32_t VERSION_MAJOR = 0;
static const uint32_t VERSION_MINOR = 1;
static const uint32_t VERSION_FIX = 0;

static const char HELP_TEXT_HELP[] = {
    "+help                              Displays the help text\n"
};

static const char HELP_TEXT_VERSION[] = {
    "+version                           Displays the texbuilder version\n"
};

static const char HELP_TEXT_INFILE[] = {
    "+infile <filename>                 Path to the input image\n"
};

static const char HELP_TEXT_OUTFILE[] = {
    "+outfile <filename>                Path to the output texture\n"
};

static const char HELP_TEXT_SIZE[] = {
    "+size <width> <height>             Resize the base image to be of size <width> x <height>\n"
};

static const char HELP_TEXT_BLOCK[] = {
    "+block <format>                    Output a block compressed texture with the given format\n"
};

static const char HELP_TEXT_GENMIPS[] = {
    "+genmips <num levels>              Generate mip maps with a given number of levels\n"
};

static const char HELP_TEXT_AMR[] = {
    "+amr                               Build an AMR texture for PBR rendering\n"
};

static const char HELP_TEXT_ROUGHNESS[] = {
    "+roughness <filename>              Specify the roughness source for the AMR texture\n"
    "                                   Only valid when --amr has been specified\n"
};

static const char HELP_TEXT_METALLIC[] = {
    "+metallic <filename>               Specify the metallic source for the AMR texture\n"
    "                                   Only valid when --amr has been specified\n"
};

static const char HELP_TEXT_AMBIENT[] = {
    "+ambient <filename>                Specify the abient occlusion source for the AMR texture\n"
    "                                   Only valid when --amr has been specified\n"
};

static const char* HELP_TEXT[] = {
    HELP_TEXT_HELP,
    HELP_TEXT_VERSION,
    HELP_TEXT_INFILE,
    HELP_TEXT_OUTFILE,
    HELP_TEXT_SIZE,
    HELP_TEXT_BLOCK,
    HELP_TEXT_GENMIPS,
    HELP_TEXT_AMR,
    HELP_TEXT_ROUGHNESS,
    HELP_TEXT_METALLIC,
    HELP_TEXT_AMBIENT,
    nullptr,
};

//===============================================================================================================================
TexBuilderApp::TexBuilderApp() {
    PublishArgEnum( "help", ARG_HELP);
    PublishArgEnum( "version", ARG_VERSION);
    PublishArgEnum( "infile", ARG_INFILE);
    PublishArgEnum( "outfile", ARG_OUTFILE);
    PublishArgEnum( "size", ARG_SIZE);
    PublishArgEnum( "block", ARG_BLOCK);
    PublishArgEnum( "genmips", ARG_GENMIPS);
    PublishArgEnum( "amr", ARG_AMR);
    PublishArgEnum( "roughness", ARG_ROUGHNESS );
    PublishArgEnum( "metallic", ARG_METALLIC );
    PublishArgEnum( "ambient", ARG_AMBIENT );
    
    m_displayHelp = false;
    m_displayVersion = false;
    m_buildAmr = false;
}

//===============================================================================================================================
void TexBuilderApp::DisplayHelp(int32_t helpItem) {
    LOG("texbuilder %u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_FIX);
    LOG("======================================\n");
    
    if (helpItem < 0 || helpItem >= ARG_COUNT) {
        for(uint32_t i=0; HELP_TEXT[i] != nullptr; ++i) {
            LOG("%s\n", HELP_TEXT[i]);
            if (i == ARG_BLOCK) {
                DisplayBlockFormats();
            }
        }
    }
    else {
        LOG("%s\n", HELP_TEXT[helpItem]);
        if (helpItem == ARG_BLOCK) {
            DisplayBlockFormats();
        }
    }
}

//===============================================================================================================================
void TexBuilderApp::DisplayBlockFormats() {
    for(const TexBuilder::BlockCompressFormat * fmt = TexBuilder::BLOCK_FORMATS;
        fmt->formatString != nullptr;
        ++fmt) {
        LOG("                                   %s\n", fmt->formatString);
    }
}
    
//===============================================================================================================================
TexBuilderApp::~TexBuilderApp() {
    
}

//===============================================================================================================================
bool TexBuilderApp::Run() {
    
    if (m_displayHelp == true) {
        DisplayHelp();
        return true;
    }
    
    if (m_displayVersion == true) {
        LOG("%u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_FIX);
        return true;
    }
    
    if (m_buildAmr == true) {
        LOG("Building AMR texture for PBR material\n");\
        
        std::string errorMsg;
        bool buildOk = m_amrBuilder.Process(errorMsg);
        if (buildOk == false) {
            LOG("Error building AMR texture\nReason: %s\n", errorMsg.c_str());
            return false;
        }
        
        // Just pass the texture built during the AMR pass to the
        // tex gen stage
        m_texgen.m_srcImage = m_amrBuilder.m_outTexture;
    }
    
    // Load an and process the image
    std::string errorMsg;
    
    bool processOk = m_texgen.Process(errorMsg);
    if (processOk == false) {
        LOG("%s\n", errorMsg.c_str());
        return false;
    }
    
    // Write out the final image
    bool writeOk = this->WriteOutputImage( errorMsg );
    if (writeOk == false) {
        LOG("%s\n", errorMsg.c_str());
        return false;
    }
    
    return true;
}

//===============================================================================================================================
bool TexBuilderApp::HandleArg( ToolArg& arg, uint32_t argEnum ) {
    
    switch( argEnum ) {
        case ARG_HELP:
            m_displayHelp = true;
            break;
            
        case ARG_VERSION:
            m_displayVersion = true;
            break;
            
        case ARG_INFILE:
            if (arg.m_params.size() != 1) {
                DisplayHelp(ARG_INFILE);
                return false;
            }
            m_texgen.m_srcPath = arg.m_params[0];
            break;
            
        case ARG_OUTFILE:
            if (arg.m_params.size() != 1) {
                LOG("outfile error %u\n", arg.m_params.size());
                DisplayHelp(ARG_OUTFILE);
                return false;
            }
            m_texgen.m_outPath = arg.m_params[ 0 ];
            break;

        case ARG_SIZE:
            if (arg.m_params.size() != 2) {
                DisplayHelp(ARG_SIZE);
                return false;
            }
            m_texgen.m_resize = true;
            m_texgen.m_newWidth = atoi(arg.m_params[0]);
            m_texgen.m_newHeight = atoi(arg.m_params[1]);
            break;
            
        case ARG_BLOCK:
            if (arg.m_params.size() != 1) {
                DisplayHelp(ARG_BLOCK);
                return false;
            }
            
            m_texgen.m_convertToBlock = true;
            m_texgen.m_outputFormat = m_texgen.EnumStringToFormat( arg.m_params[0] );
            
            if (m_texgen.m_outputFormat == ToolImage::FORMAT_COUNT) {
                DisplayHelp(ARG_BLOCK);
                return false;
            }
            break;
            
        case ARG_GENMIPS: {
            if (arg.m_params.size() != 1) {
                DisplayHelp(ARG_GENMIPS);
                return false;
            }
            
            uint32_t mipCount = atoi( arg.m_params[0] );
            if (mipCount > 32) {
                LOG("To many mip levels!\n");
                return false;
            }
            
            m_texgen.m_genMips = true;
            m_texgen.m_mipCount = mipCount;
            
            break;
        }
            
        case ARG_AMR:
            if (arg.m_params.empty() == false) {
                DisplayHelp(ARG_AMR);
                return false;
            }
            m_buildAmr = true;
            break;
            
        case ARG_ROUGHNESS:
            if (arg.m_params.size() != 1 || m_buildAmr == false) {
                DisplayHelp(ARG_ROUGHNESS);
                return false;
            }
            m_amrBuilder.m_srcTexturePath[AmrBuilder::SRC_TEX_ROUGHNESS] = arg.m_params[0];
            break;
            
        case ARG_METALLIC:
            if (arg.m_params.size() != 1 || m_buildAmr == false) {
                DisplayHelp(ARG_METALLIC);
                return false;
            }
            m_amrBuilder.m_srcTexturePath[AmrBuilder::SRC_TEX_METALLIC] = arg.m_params[0];
            break;
            
        case ARG_AMBIENT:
            if (arg.m_params.size() != 1 || m_buildAmr == false) {
                DisplayHelp(ARG_AMBIENT);
                return false;
            }
            m_amrBuilder.m_srcTexturePath[AmrBuilder::SRC_TEX_AO] = arg.m_params[0];;
            break;
            
    }
    
    return true;
}

//===============================================================================================================================
bool TexBuilderApp::WriteOutputImage( std::string& errorMsg ) {
    bool folderOk = CreateFolderAtPath( m_texgen.m_outPath.c_str() );
    if ( folderOk == false ) {
        errorMsg = sys->Vformat( "Error creating output folder for path %s", m_texgen.m_outPath.c_str() );
        return false;
    }
    
    bool writeOk = m_texgen.WriteOutputImage( errorMsg );
    if ( writeOk == false ) {
        errorMsg = sys->Vformat( "Error writing output image %s\nReason: %s", m_texgen.m_outPath.c_str(), errorMsg.c_str() );
        return false;
    }
    
    return true;
}
