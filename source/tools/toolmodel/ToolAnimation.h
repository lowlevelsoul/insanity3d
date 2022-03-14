//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLANIMATION_H__
#define __TOOLANIMATION_H__

class ToolAnimation;
class ToolModel;

class ToolAnimNode : public RefObject {
public:
    typedef RefPointer<ToolAnimNode> ref_ptr_t;
    typedef std::vector<ref_ptr_t> vector_t;
    typedef vector_t::iterator itetator_t;
    typedef vector_t::const_iterator const_itetator_t;
    
    ToolAnimNode( ToolAnimation * anim ) {
        m_animation = anim;
    }
    
    virtual ~ToolAnimNode() {
        
    }
    
public:
    ToolAnimation *                 m_animation;
    std::string                     m_name;
    std::vector<i3d::Vector3>       m_trackTranslate;
    std::vector<float>              m_trackTranslateTime;
    std::vector<i3d::Quaternion>    m_trackRotate;
    std::vector<float>              m_trackRotateTime;
};

class ToolAnimation : public RefObject {
public:
    struct Flags {
        uint64_t        m_absolute : 1;         ///< If set to 1, the animation uses absolute local transforms
        
        Flags() {
            m_absolute = 0;
        }
    };
    
    typedef RefPointer<ToolAnimation> ref_ptr_t;
    typedef std::vector<ref_ptr_t> vector_t;
    typedef vector_t::iterator itetator_t;
    typedef vector_t::const_iterator const_itetator_t;
    
    ToolAnimation( ToolModel * model );
    
    virtual ~ToolAnimation();
    
    ToolAnimNode * AddNode( const char * name );
    
public:
    ToolModel *                 m_model;
    Flags                       m_flags;
    uint32_t                    m_framesPerSecond;
    uint32_t                    m_frameCount;
    ToolAnimNode::vector_t      m_nodes;
    
};

#endif 
