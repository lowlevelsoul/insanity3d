//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "DictionaryWriter.h"

//======================================================================================================================
DictionaryWriter::DictionaryWriter() {
    m_entryType = 0;
}

//======================================================================================================================
DictionaryWriter::~DictionaryWriter() {
    
}

//======================================================================================================================
void DictionaryWriter::AddEntry( ToolMemStream & data, const char * name ) {
    Entry * entry = new Entry;
    entry->m_data.Write( data );
    
    uint64_t hash = r3d::fh64::CalcFromString( name );
    
    int32_t insertPos = -1;
    bool found = r3d::Sys::BinarySearch<uint64_t, int32_t>( insertPos, hash, &m_entryHashArray[0], m_entryHashArray.size() );
    ERROR( found == true, "Dictionary entry with name '%s' already exists\n", name );
    
    uint32_t index = (uint32_t) m_entryData.size();
    
    m_entryHashArray.insert( m_entryHashArray.begin() + insertPos, hash );
    m_entryHashMap.insert( m_entryHashMap.begin() + insertPos, index );
    m_entryData.push_back( entry );
    m_entrySize.push_back( (uint32_t) entry->m_data.Length() );
    m_entryOffset.push_back( 0 );           // Dummy offset.
}

//======================================================================================================================
void DictionaryWriter::Write( ToolMemStream & str ) {
    r3d::DictionaryStream header;
    memset( &header, 0, sizeof( header ) );
    header.m_version = r3d::DictionaryStream::VERSION;
    header.m_entryType = m_entryType;
    header.m_entryCount = (uint32_t) m_entryData.size();
    
    // Initial write of header
    uintptr_t headerOffs = str.Tell();
    WriteHeader( str, header );
    
    // Write out entry info
    
    // Entry hash map
    str.PadToAlignment(16);
    header.m_offsEntryHashArray = (uint32_t) ( str.Tell() - headerOffs );
    str.Write( &m_entryHashArray[0], m_entryHashArray.size() );
    
    str.PadToAlignment(16);
    header.m_offsEntryHashMap = (uint32_t) ( str.Tell() - headerOffs );
    str.Write( &m_entryHashMap[0], m_entryHashMap.size() );
    
    // Entry offset + size
    str.PadToAlignment(16);
    header.m_offsEntryArray = (uint32_t) ( str.Tell() - headerOffs );
    str.Write( &m_entryOffset[0], m_entryOffset.size() );
    
    // (Note: we'll need to re-write the entry offset array, as it contains dummy values right now)
    str.PadToAlignment(16);
    header.m_offsEntrySizeArray = (uint32_t) ( str.Tell() - headerOffs );
    str.Write( &m_entrySize[0], m_entrySize.size() );
    
    // Write out the entry data
    uint32_t index = 0;
    for ( auto e : m_entryData ) {
        str.PadToAlignment(16);
        
        // Store data offset in array
        m_entryOffset[index] = (uint32_t) ( str.Tell() - headerOffs );
        str.Write( e->m_data );
        ++index;
    }
    
    uintptr_t endPos = str.Tell();
    
    // Re-write entry offsets
    str.Seek( headerOffs + header.m_offsEntryArray );
    str.Write( &m_entryOffset[0], m_entryOffset.size() );
    
    // Re-write dictionary header
    str.Seek( headerOffs );
    WriteHeader( str, header );
    
    // Back to the end of the stream
    str.Seek( endPos );
}

//======================================================================================================================
void DictionaryWriter::WriteHeader( ToolMemStream & str, r3d::DictionaryStream & header ) {
    str.Write( &header.m_version, 1 );
    str.Write( &header.m_flags, 1 );
    str.Write( &header.m_entryType, 1 );
    str.Write( &header.m_entryCount, 1 );
    str.Write( &header.m_offsEntryHashArray, 1 );
    str.Write( &header.m_offsEntryHashMap, 1 );
    str.Write( &header.m_offsEntryArray, 1 );
    str.Write( &header.m_offsEntrySizeArray, 1 );
    str.Write( &header.m_pad, 1 );
}
