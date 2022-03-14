//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __DICTIONARYWRITER_H__
#define __DICTIONARYWRITER_H__

#include "toolapp/ToolMemStream.h"
#include "engine/misc/DictionaryStream.h"

class DictionaryWriter {
public:
    DictionaryWriter();
    
    ~DictionaryWriter();
    
    void SetEntryType( uint64_t typeId ) {
        m_entryType = typeId;
    }
    
    void AddEntry( ToolMemStream & data, const char * name );
    
    void Write( ToolMemStream & str );
    
    void WriteHeader( ToolMemStream & str, r3d::DictionaryStream & header );
    
protected:
    
    class Entry : public RefObject {
    public:
        typedef RefPointer<Entry> ref_ptr_t;
        typedef std::vector<ref_ptr_t> vector_t;
        
        Entry() {}
        virtual ~Entry() {}
    
        ToolMemStream   m_data;
    };
    
    uint64_t                    m_entryType;
    Entry::vector_t             m_entryData;
    std::vector<uint64_t>       m_entryHashArray;
    std::vector<uint32_t>       m_entryHashMap;
    std::vector<uint32_t>       m_entryOffset;
    std::vector<uint32_t>       m_entrySize;
};

#endif 
