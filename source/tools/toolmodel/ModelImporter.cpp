//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "toolmodel/ModelImporter.h"

//==========================================================================================================================================
ModelImporter::ModelImporter( Options & options ) {
    m_options = options;
}

//==========================================================================================================================================
ModelImporter::~ModelImporter() {

}

//==========================================================================================================================================
ToolModel::ref_ptr_t ModelImporter::Load( const char * path ) {
    return nullptr;
}
