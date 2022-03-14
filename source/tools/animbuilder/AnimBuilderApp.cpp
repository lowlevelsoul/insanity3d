//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "AnimBuilderApp.h"
#include "modelbuilder/ModelImporterAssImp.h"
#include "AnimWriter.h"

TOOL_APP(AnimBuilderApp)

enum CMD {
    CMD_ANIM = 0,
    CMD_OUTFILE,
    CMD_BIND,
};

//======================================================================================================================
AnimBuilderApp::AnimBuilderApp() {
    PublishArgEnum( "anim", CMD_ANIM );
    PublishArgEnum( "outfile", CMD_OUTFILE );
    PublishArgEnum( "bind", CMD_BIND );
    
    m_bindAnims = false;
}

//======================================================================================================================
AnimBuilderApp::~AnimBuilderApp() {
    
}

//======================================================================================================================
void AnimBuilderApp::DisplayHelp( int32_t helpItem ) {
    
}

//======================================================================================================================
bool AnimBuilderApp::Run() {
    
    if ( m_bindAnims == true ) {
        LoadBindSkeleton();
    }
    
    LoadAnims();
    
    ToolMemStream str;
    BuildAnims( str );
    
    SaveAnims( str );

    return true;
}

//======================================================================================================================
bool AnimBuilderApp::HandleArg( ToolArg& arg, uint32_t argEnum ) {
        
    switch( argEnum ) {
        case CMD_ANIM:
            if ( arg.GetParamCount() != 2) {
                return false;
            }
            
            m_currAnim = new AnimInfo;
            m_anims.push_back( m_currAnim );
            
            m_currAnim->m_path = arg.m_params[0];
            m_currAnim->m_name = arg.m_params[1];
            
            break;
            
        case CMD_OUTFILE:            
            m_outfile = arg.m_params[0];
            break;
            
        case CMD_BIND:
            m_bindSkelPath = arg.m_params[0];
            m_bindAnims = true;
            break;
    }
        
    return true;
}
//======================================================================================================================
void AnimBuilderApp::LoadBindSkeleton() {
    
    LOG( "Loading bind skeleton from \"%s\"\n", m_bindSkelPath.c_str() );
    
    ModelImporter::Options importOptions;
    ModelImporterAssImp importer(importOptions);
    
    ToolModel::ref_ptr_t srcModel = importer.Load( m_bindSkelPath.c_str() );
    
    SkeletonBuilder::Options skelOptions;
    m_bindSkel.m_options = skelOptions;
    
    m_bindSkel.BuildJointList( srcModel->m_skeleton );
}

//======================================================================================================================
void AnimBuilderApp::LoadAnims() {
    for ( AnimInfo * i : m_anims ) {
        ModelImporter::Options importOptions;
        ModelImporterAssImp importer(importOptions);
        
        LOG( "Loading anim \"%s\" from \"%s\"\n", i->m_name.c_str(), i->m_path.c_str() );
        
        ToolModel::ref_ptr_t model = importer.Load( i->m_path.c_str() );
        i->m_model = model;
    }
}

//======================================================================================================================
void AnimBuilderApp::BuildAnims( ToolMemStream & str ) {
    
    DictionaryWriter dictWriter;
    dictWriter.SetEntryType( R3D_MAKE_ID( A, n, i, m, a, t, i, o, n, _, _, _) );
    
    for (auto e : m_anims ) {
        ToolMemStream animStream;
        AnimWriter animWriter;
        
        if ( e->m_model->m_anims.empty() == false ) {
            LOG( "Compiling animation \"%s\"\n", e->m_name.c_str() );
            animWriter.Write( &animStream, e->m_model->m_anims[0] );
            
            dictWriter.AddEntry( animStream, e->m_name.c_str() );
            
        }
        else {
            LOG( "Loaded scene for \"%s\" did not contain an animation\n", e->m_name.c_str() );
        }
    }
    
    LOG(" Compiling animation dictionary...\n");
    dictWriter.Write( str );
    LOG(" Done.\n");
}

//======================================================================================================================
void AnimBuilderApp::SaveAnims( ToolMemStream & str ) {
    if (! CreateFolderAtPath( m_outfile.c_str() ) ) {
        ERROR(false, "Could not create output path for %s\n", m_outfile.c_str() );
    }
    
    r3d::File * file = fs->FileOpen( m_outfile.c_str(), "wb");
    ERROR( file == nullptr, "Could not open %s for writing\n", m_outfile.c_str());
    
    file->Write( str.GetStream(), str.Length() );
    file->Close();
}
