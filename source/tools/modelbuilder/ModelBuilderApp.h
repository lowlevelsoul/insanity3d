//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __MODELBUILDERAPP_H__
#define __MODELBUILDERAPP_H__

#include "toolmodel/ToolModel.h"
#include "modelbuilder/SkeletonBuilder.h"
#include "modelbuilder/ModelBuilder.h"

class ModelBuilderApp : public ToolApp {
public:
    enum MODE {
        MODE_MODEL = 0,
        MODE_SKELETON
    };
    
    std::string                 m_inputPath;
    std::string                 m_outputPath;
    std::string                 m_skeletonPath;
    bool                        m_genMaterials;
    bool                        m_info;
    bool                        m_allowSkeleton;
    bool                        m_help;
    int32_t                     m_helpItem;
    bool                        m_version;
    ModelImporter::Options      m_importOptions;
    ModelBuilder::Options       m_modelBuilderOpt;
    SkeletonBuilder::Options    m_skeletonBuilderOpt;
    float                       m_scale;
    bool                        m_flipFaces;
    bool                        m_genNormals;
    MODE                        m_mode;
    bool                        m_stripMixamo;
    
    
    i3d::stl::Vector<i3d::stl::String::type>::type    m_meshFilters;          ///< List of mesh filters
    std::vector<uint64_t>       m_meshFilterHashes;     ///< List of hashed mesh filter names
    
    
    ToolModel::ref_ptr_t        m_model;            ///< Pointer to the model that was imported
    
    ModelBuilderApp();
    
    virtual ~ModelBuilderApp();
    
    void DisplayHelp(int32_t helpItem = -1);
    
    virtual bool Run();
    
    virtual bool HandleArg( ToolArg& arg, uint32_t argEnum );
    
    void ShowModelInfo( ToolModel * model );
    
    void ShowJointInfo( ToolJoint * joint, uint32_t level );
    
    void BuildModel( ToolModel * model );
    
    void BuildSkeleton( ToolModel * model );

};

#endif
