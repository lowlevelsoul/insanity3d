//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "modelbuilder/ModelBuilderApp.h"
#include "modelbuilder/MeshWriter.h"

TOOL_APP(ModelBuilderApp)

enum CMD {
    CMD_INFILE=0,
    CMD_OUTFILE,
    CMD_INFO,
    CMD_GENMATERIALS,
    CMD_MESHFILTER,
    CMD_SKELETON,
    CMD_NOSKELETON,
    CMD_HELP,
    CMD_VERSION,
    CMD_SCALE,
    CMD_FLIPFACES,
    CMD_GENNORMALS,
    CMD_STRIPMIXAMO,
};

// 0.1.2        - Added support in base tool for platform profiles
// 0.1.3        - Texture V coordinate is flipped from OGL standard
static const uint32_t VERSION_MAJOR = 0;
static const uint32_t VERSION_MINOR = 1;
static const uint32_t VERSION_PATCH = 3;

static const char HELP_TEXT_ARGS[] = {
    "Arguments parameters\n."
    "Required parameters specified in the help text are given\n"
    "by <>. Optional parameters are enclosed using [].\n"
};

static const char HELP_TEXT_INFILE[] = {
    "--infile <filename>            Path to the input file.\n"
};

static const char HELP_TEXT_OUTFILE[] = {
    "--outfile <filename>           Path to the output model file. If the model has a skeleton\n"
    "                               then the sksleton file will also be written to the same location\n"
    "                               with a .skel extension.\n"
};

static const char HELP_TEXT_INFO[] = {
    "--info                         Displays information about the model specified by --infile.\n"
};

static const char HELP_TEXT_GENMATERIALS[] = {
    "--genmats <filename>           Generates a material script file to the specified location.\n"
};

static const char HELP_TEXT_MESHFILTER[] = {
    "--meshfilter <meshname>..<meshname n> Specifies the names of the meshes in the model to be included\n"
    "                               in the exported mode.l\n"
};

static const char HELP_TEXT_NOSKELETON[] = {
    "--noskeleton                   Do not generate a skeleton file\n"
};

static const char HELP_TEXT_HELP[] = {
    "--help                         Displays the help text.\n"
};


static const char HELP_TEXT_VERSION[] = {
    "--version                      Displays the version number.\n"
};

static const char HELP_TEXT_SCALE[] = {
    "--scale <scaling factor>       Scales the mesh vertices, node translations and animation\n"
    "                               translations keys by the specified amount.\n"
};

static const char HELP_TEXT_FLIPFACES[] = {
    "--flipfaces                    Changes the winding order on the faces during import.\n"
};

static const char HELP_TEXT_GENNORMALS[] = {
    "--gennormals                    Calculates smoothed vertex normals\n"
};

static const char HELP_TEXT_SKELETON[] = {
    "--skeleton                     Write out the skeleton from the model\n"
};

static const char HELP_TEXT_MIXAMO[] = {
    "--stripmixamo                  Strip mixamo marker joints from skeleton\n"
};


static const char *HELP_TEXT[] = {
    HELP_TEXT_INFILE,
    HELP_TEXT_OUTFILE,
    HELP_TEXT_INFO,
    HELP_TEXT_GENMATERIALS,
    HELP_TEXT_MESHFILTER,
    HELP_TEXT_SKELETON,
    HELP_TEXT_NOSKELETON,
    HELP_TEXT_HELP,
    HELP_TEXT_VERSION,
    HELP_TEXT_SCALE,
    HELP_TEXT_FLIPFACES,
    HELP_TEXT_GENNORMALS,
    HELP_TEXT_MIXAMO,
    nullptr,
};

//======================================================================================================================
ModelBuilderApp::ModelBuilderApp() {
    m_genMaterials = false;
    m_info = false;
    m_allowSkeleton = true;
    m_help = false;
    m_helpItem = -1;
    m_version = false;
    
    PublishArgEnum( "infile", CMD_INFILE);
    PublishArgEnum( "outfile", CMD_OUTFILE);
    PublishArgEnum( "info", CMD_INFO);
    PublishArgEnum( "genmats", CMD_GENMATERIALS);
    PublishArgEnum( "meshfilter", CMD_MESHFILTER);
    PublishArgEnum( "skeleton", CMD_SKELETON);
    PublishArgEnum( "noskeleton", CMD_NOSKELETON);
    PublishArgEnum( "help", CMD_HELP);
    PublishArgEnum( "version", CMD_VERSION);
    PublishArgEnum( "scale", CMD_SCALE);
    PublishArgEnum( "flipfaces", CMD_FLIPFACES);
    PublishArgEnum( "gennormals", CMD_GENNORMALS);
    PublishArgEnum( "stripmixamo", CMD_STRIPMIXAMO);
    
    m_scale = 1;
    m_flipFaces = false;
    m_genNormals = false;
    m_mode = MODE_MODEL;
    m_stripMixamo = false;
}

//======================================================================================================================
ModelBuilderApp::~ModelBuilderApp() {
    
}

//======================================================================================================================
void ModelBuilderApp::DisplayHelp(int32_t helpItem) {
    if (helpItem != -1) {
        m_helpItem = helpItem;
    }
    
    if (m_helpItem == -1) {
        XE_LOG("Model Builder\n");
        XE_LOG("Version %u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
        XE_LOG("Help\n");
        XE_LOG("=================================================================\n");
        //XE_LOG("%s", HELP_TEXT_ARGS);
        //XE_LOG("=================================================================\n");
        
        for(const char** item = HELP_TEXT; *item != nullptr; ++item) {
            XE_LOG("%s", *item);
        }
    }
    else {
        XE_LOG("%s", HELP_TEXT[helpItem]);
    }
}

//======================================================================================================================
bool ModelBuilderApp::Run() {
    
    if (m_help == true) {
        DisplayHelp();
        return true;
    }
    
    if (m_version == true) {
        XE_LOG("ModelBuilderApp\n%u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
        return true;
    }
    
    if (m_inputPath.size() == 0) {
        XE_LOG("No input file specified\n");
        return true;
    }
    
    XE_LOG("ModelBuilderApp %u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    XE_LOG("Loading %s\n", m_inputPath.c_str());
    
    ModelImporter::Options importOptions;
    ModelImporterAssImp importer(importOptions);
    
    ToolModel::ref_ptr_t model = importer.Load( m_inputPath.c_str() );
    
    m_skeletonBuilderOpt.m_scale = m_scale;
    m_skeletonBuilderOpt.m_pruneMixamoMarkers = m_stripMixamo;
    
    m_modelBuilderOpt.m_scale = m_scale;
    m_modelBuilderOpt.m_flipFaces = m_flipFaces;
    m_modelBuilderOpt.m_genNormals = m_genNormals;
    m_modelBuilderOpt.m_stripMixamo = m_stripMixamo;
    m_modelBuilderOpt.m_skeletonOpt = m_skeletonBuilderOpt;
    
    if ( m_info == true ) {
        ShowModelInfo( model );
        return true;
    }
    
    switch( m_mode ) {
        case MODE_MODEL:
            XE_LOG("Building and writing model\n");
            BuildModel( model );
            break;
        case MODE_SKELETON:
            XE_LOG("Building and writing skeleton\n");
            BuildSkeleton( model );
            break;
    }
    
    XE_LOG(" Done. \n");
    
    return true;
}

//======================================================================================================================
void ModelBuilderApp::BuildModel( ToolModel * model ) {
    ModelBuilder modelBuilder( m_modelBuilderOpt );
    ToolMemStream meshStream;
    modelBuilder.WriteToStream( meshStream, model );
    
    if (! CreateFolderAtPath( m_outputPath.c_str() ) ) {
        XE_ERROR(false, "Could not create output path for %s\n", m_outputPath.c_str() );
    }
    
    i3d::File * file = fs->FileOpen( m_outputPath.c_str(), "wb");
    XE_ERROR( file == nullptr, "Could not open %s for writing\n", m_outputPath.c_str());
    
    file->Write( meshStream.GetStream(), meshStream.Length() );
    file->Close();
}

//======================================================================================================================
void ModelBuilderApp::BuildSkeleton( ToolModel * model ) {
    SkeletonBuilder builder( m_skeletonBuilderOpt );
    
    XE_LOG("Building skeleton\n");
    
    ToolMemStream skelStr;
    builder.Write( skelStr, model->m_skeleton );
    
    XE_LOG("Exporting Skeleton with %u joints\n", builder.m_joints.size());
    
    if (! CreateFolderAtPath( m_outputPath.c_str() ) ) {
        XE_ERROR(false, "Could not create output path for %s\n", m_outputPath.c_str() );
    }
    
    i3d::File * file = fs->FileOpen( m_outputPath.c_str(), "wb");
    XE_ERROR( file == nullptr, "Could not open %s for writing\n", m_outputPath.c_str());
    
    file->Write( skelStr.GetStream(), skelStr.Length() );
    file->Close();
}

//======================================================================================================================
bool ModelBuilderApp::HandleArg(  ToolArg& arg, uint32_t argEnum ) {
    
    switch( argEnum ) {
            
        case CMD_INFILE : {
            if (arg.m_params.size() != 1) {
                XE_LOG("Invalid argument for \'infile\'.\n");
                DisplayHelp(CMD_INFILE);
                return false;
            }
            
            m_inputPath = arg.m_params[0];
            break;
        }
            
        case CMD_OUTFILE : {
            if (arg.m_params.size() != 1) {
                XE_LOG("Invalid argument for \'outfile\'.\n");
                DisplayHelp(CMD_OUTFILE);
                return false;
            }
            
            m_outputPath = arg.m_params[0];
            break;
        }
            
        case CMD_INFO : {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argument for \'info\'.\n");
                DisplayHelp(CMD_GENMATERIALS);
                return false;
            }
            m_info = true;
            break;
        }
            
        case CMD_GENMATERIALS : {
            if (arg.m_params.size() != 1) {
                XE_LOG("Invalid argument for \'genmats\'. \n");
                DisplayHelp(CMD_GENMATERIALS);
                return false;
            }
            
            m_outputPath = arg.m_params[0];
            m_genMaterials = true;
            break;
        }
            
        case CMD_MESHFILTER : {
            if (arg.m_params.size() == 0) {
                XE_LOG("Invalid argument for \'meshfilter\'\n");
                DisplayHelp(CMD_MESHFILTER);
                return false;
            }

            for(auto f : arg.m_params) {
                // AddMeshFilter( f );
            }
            break;
        }
            
        case CMD_SKELETON: {
            if (arg.m_params.size() > 0) {
                XE_LOG("Invalid argument for \'skeleton\'\n");
                DisplayHelp(CMD_SKELETON);
                return false;
            }
            
            m_mode = MODE_SKELETON;            
            break;
        }
            
        case CMD_NOSKELETON: {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argument for \'noskeleton\'\n");
                DisplayHelp(CMD_NOSKELETON);
                return false;
            }
            
            m_allowSkeleton = false;
            break;
        }
            
        case CMD_HELP: {
            m_help = true;
            break;
        }
            
        case CMD_VERSION: {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argument for \'version\'\n");
                DisplayHelp(CMD_VERSION);
                return false;
            }
            
            m_version = true;
            
            break;
        }
            
        case CMD_SCALE: {
            if (arg.m_params.size() != 1) {
                XE_LOG("Invalid argument for \'scale\'\n");
                DisplayHelp(CMD_SCALE);
                return false;
            }
            
            m_scale = (float) atof(arg.m_params[0] );
            
            break;
        }
            
        case CMD_FLIPFACES: {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argument for \'flipfaces\'\n");
                DisplayHelp(CMD_FLIPFACES);
                return false;
            }
            
            m_flipFaces = true;
            
            break;
        }
            
        case CMD_GENNORMALS: {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argumentS for \'gennormals\'\n");
                DisplayHelp(CMD_GENNORMALS);
                return false;
            }
            
            m_genNormals = true;
            
            break;
        }
            
        case CMD_STRIPMIXAMO : {
            if (arg.m_params.size() != 0) {
                XE_LOG("Invalid argumentS for \'gennormals\'\n");
                DisplayHelp(CMD_STRIPMIXAMO);
                return false;
            }
            
            m_stripMixamo = true;
            
            break;
        }
    }
    
    return true;
}

//======================================================================================================================
void ModelBuilderApp::ShowModelInfo( ToolModel * model ) {
    
    ModelBuilder modelBuilder( m_modelBuilderOpt );
    modelBuilder.GatherMeshes( model );
    modelBuilder.ModifyMeshes( model );
    
    i3d::Vector3 dim = modelBuilder.m_boundsMax - modelBuilder.m_boundsMin;
    
    // Basic model info
    XE_LOG( "==================================================\n");
    XE_LOG( "Model Info\n");
    XE_LOG( "==================================================\n");
    XE_LOG( "         Mesh Count : %u\n", model->m_meshes.size() );
    XE_LOG( "     Material Count : %u\n", model->m_materials.size() );
    XE_LOG( "       Has skeleton : %s\n", (model->m_skeleton == nullptr) ? "NO" : "YES");
    XE_LOG( " Has Skinned Meshes : %s\n", (model->HasSkinnedMeshes()==false) ? "NO" : "YES");
    XE_LOG( "         Bounds Min : <%5.5f %5.5f %5.5f>\n", modelBuilder.m_boundsMin.X(), modelBuilder.m_boundsMin.Y(), modelBuilder.m_boundsMin.Z());
    XE_LOG( "         Bounds Min : <%5.5f %5.5f %5.5f>\n", modelBuilder.m_boundsMax.X(), modelBuilder.m_boundsMax.Y(), modelBuilder.m_boundsMax.Z());
    XE_LOG( "         Dimensions : <%5.5f %5.5f %5.5f>\n", dim.X(), dim.Y(), dim.Z());
    
    // Mesh info
    XE_LOG( "==================================================\n");
    XE_LOG( "Mesh Info\n");
    XE_LOG( "==================================================\n");
    
    for ( auto sm : modelBuilder.m_srcMeshes ) {
        XE_LOG( " Mesh %s\n", sm->m_mesh->m_name.c_str() );
        XE_LOG( "        Skinned : %s\n", sm->m_mesh->IsSkinned() == true ? "YES" : "NO" );
        XE_LOG( "   Vertex Count :  %u\n", (uint32_t) sm->m_mesh->m_verts.size() );
        XE_LOG( " Triangle Count :  %u\n", (uint32_t) sm->m_mesh->m_triangleIndices.size() / 3 );
    }
    
    // Skeleton Info
    SkeletonBuilder::Options opt;
    opt.m_scale = m_scale;
    opt.m_pruneMixamoMarkers = m_stripMixamo;
    SkeletonBuilder builder( opt );
    builder.BuildJointList( model->m_skeleton );
    
    // Mesh info
    XE_LOG( "==================================================\n");
    XE_LOG( "Skeleton Info\n");
    XE_LOG( "==================================================\n");
    ShowJointInfo( builder.m_joints[0], 0 );
}

//======================================================================================================================
void ModelBuilderApp::ShowJointInfo( ToolJoint * joint, uint32_t level ) {
    
    if (level > 0) {
        std::string margin;
        for( int32_t i = 0; i < level; ++i) {
            margin += ' ';
        }
        XE_LOG("%s", margin.c_str());
    }
    
    XE_LOG("%s\n", joint->m_name.c_str());
    
    for (auto c : joint->m_children) {
        ShowJointInfo( c, level + 1);
    }
}
