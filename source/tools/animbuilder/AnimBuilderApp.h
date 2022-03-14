//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ANIMBUILDER_H__
#define __ANIMBUILDER_H__

#include "toolmodel/ToolModel.h"
#include "modelbuilder/ModelImporter.h"
#include "modelbuilder/SkeletonBuilder.h"
#include "misc/DictionaryWriter.h"

class AnimBuilderApp : public ToolApp {
public:
    class AnimInfo : public RefObject {
    public:
        AnimInfo() {
            m_scale = 1;
        }
        
        virtual ~AnimInfo() {
            
        }
        
        std::string             m_path;
        std::string             m_name;
        std::string             m_scale;
        ToolModel::ref_ptr_t    m_model;
        
        typedef RefPointer<AnimInfo> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
    };
    
    AnimBuilderApp();
    
    virtual ~AnimBuilderApp();
    
    void DisplayHelp(int32_t helpItem = -1);
    
    virtual bool Run();
    
    virtual bool HandleArg( ToolArg& arg, uint32_t argEnum );
    
    void LoadAnims();
    
    void BuildAnims( ToolMemStream & str );
    
    void SaveAnims( ToolMemStream & str );
    
    void LoadBindSkeleton();
    
public:
    AnimInfo *              m_currAnim;
    AnimInfo::vector_t      m_anims;            ///< List of loaded anims
    std::string             m_outfile;          ///< Path of the output file
    std::string             m_bindSkelPath;     ///< Path to the model file contianing the bind skeleton
    
    SkeletonBuilder         m_bindSkel;          ///< The refernce skeleton info that we loaded from a model file
    bool                    m_bindAnims;        ///< Id true, we want to bind the aims to a spefici skeleton
    
};

#endif
