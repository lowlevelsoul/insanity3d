//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SCENESTREAM_H__
#define __SCENESTREAM_H__

class SceneStream {
public:
    typedef i3d::stl::Vector<char> char_array;
    typedef i3d::stl::Vector<uint32_t> uint_array;
    typedef i3d::stl::Vector<uint8_t> byte_array;
    
    SceneStream();
    
    virtual ~SceneStream();
    
public:
    char_array::type        m_strings;              ///< Strings used by the scene
    uint_array::type        m_entityDefs;           ///< Indices into the strings array of the entity defs to load
    byte_array::type        m_eventData;            ///< Binday stream that holds all of the event data
    uint_array::type        m_events;               ///< List of events as offsets into the event data stream
};

#endif
