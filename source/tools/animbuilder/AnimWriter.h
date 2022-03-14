//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ANIMWRITER_H__
#define __ANIMWRITER_H__

#include "engine/anim/AnimStream.h"

class ToolAnimation;

class AnimWriter {
public:
    class Options {
    public:
        float       m_scale;
        
        Options() {
            m_scale = 1;
        }
        
        static const Options DEFAULT;
    };
    
    AnimWriter( const Options & options = Options::DEFAULT );
    
    ~AnimWriter();
    
    void Write( ToolMemStream * str, ToolAnimation * anim );
    
    void WriteHeader( ToolMemStream * str, r3d::AnimStream * header );
    
    void WriteAnimNode( ToolMemStream * str, r3d::AnimNodeTrackStream * header );
    
    void BuildAnimNodes( ToolAnimation * anim, std::vector<r3d::AnimNodeTrackStream> & nodes,
                        std::vector<uint64_t> & nodeNames, std::vector<uint32_t> & nodeFlags,
                        ToolMemStream & posTrack,  ToolMemStream & rotTrack );
    
    
    
protected:
    Options         m_options;
};

#endif
