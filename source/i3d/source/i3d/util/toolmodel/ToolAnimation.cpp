//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "i3d/util/toolmodel/ToolAnimation.h"

namespace i3d {

    //==========================================================================================================================================
    ToolAnimation::ToolAnimation( ToolModel * model ) {
        m_model = model;
        m_framesPerSecond = 0;
        m_frameCount = 0;
    }

    //==========================================================================================================================================
    ToolAnimation::~ToolAnimation() {
        
    }

    //==========================================================================================================================================
    ToolAnimNode * ToolAnimation::AddNode( const char * name ) {
        
        ToolAnimNode * node = new ToolAnimNode(this);
        node->m_animation = this;
        node->m_name = name;
        
        m_nodes.push_back( node );
        
        return node;
    }
}
