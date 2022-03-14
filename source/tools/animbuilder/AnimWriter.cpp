//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "AnimWriter.h"
#include "toolmodel/ToolAnimation.h"

const AnimWriter::Options AnimWriter::Options::DEFAULT;

//======================================================================================================================
AnimWriter::AnimWriter( const Options & options ) {
    m_options = options;
}

//======================================================================================================================
AnimWriter::~AnimWriter() {
    
}

//======================================================================================================================
void AnimWriter::Write( ToolMemStream * str, ToolAnimation * anim ) {
    
    // Build the node track info and stream
    std::vector<r3d::AnimNodeTrackStream> nodes;
    std::vector<uint64_t> nodeNames;
    std::vector<uint32_t> nodeFlags;
    std::vector<uint64_t> sortedNames;
    std::vector<uint32_t> sortedNameMap;
    ToolMemStream posTrack, rotTrack;
    
    BuildAnimNodes( anim, nodes, nodeNames, nodeFlags, posTrack, rotTrack);
    
    // Build the sorted name map
    for ( uint32_t i = 0; i < nodeNames.size(); ++i ) {
        int32_t insertPos = -1;
        r3d::Sys::BinarySearch<uint64_t, int32_t>( insertPos, nodeNames[ i ], &sortedNames[0], sortedNames.size() );
        
        sortedNames.insert( sortedNames.begin() + insertPos, nodeNames[i]);
        sortedNameMap.insert( sortedNameMap.begin() + insertPos, i );
    }
    

    // Write out the
    r3d::AnimStream header;
    memset(&header, 0, sizeof(header) );
    header.m_version        = r3d::AnimStream::VERSION;
    header.m_flags          |= (anim->m_flags.m_absolute == 1) ? r3d::AnimStream::F_ABSOLUTE : 0;
    header.m_frameCount     = anim->m_frameCount;
    header.m_frameRate      = anim->m_framesPerSecond;
    header.m_duration       = (1.0f / (float) anim->m_framesPerSecond) * (float) anim->m_frameCount;
    header.m_nodeCount      = (uint32_t) anim->m_nodes.size();
    
    // Write out the known info about the header
    uintptr_t headerPos = str->Tell();
    WriteHeader( str, &header );
    
    // Write out node name array
    str->PadToAlignment(16);
    header.m_offsNodeNames = (uint32_t) ( str->Tell() - headerPos );
    str->Write( &nodeNames[0], nodeNames.size() );
    
    // Write out node flag array
    str->PadToAlignment(16);
    header.m_offsNodeFlags = (uint32_t) ( str->Tell() - headerPos );
    str->Write( &nodeFlags[0], nodeFlags.size() );
    
    str->PadToAlignment(16);
    header.m_offsNodeSortedNames = (uint32_t) ( str->Tell() - headerPos );
    str->Write( &sortedNames[0], sortedNames.size() );
    
    str->PadToAlignment(16);
    header.m_offsNodeSortedNameMap = (uint32_t) ( str->Tell() - headerPos );
    str->Write( &sortedNameMap[0], sortedNameMap.size() );
    
    // Write out node track info
    str->PadToAlignment(16);
    header.m_offsNodeTracks = (uint32_t) ( str->Tell() - headerPos );
    
    for (auto n : nodes ) {
        WriteAnimNode( str, &n );
    }
    
    // Write out track data
    str->PadToAlignment(16);
    header.m_offsPosTrack = (uint32_t) ( str->Tell() - headerPos );
    str->Write( posTrack );
    
    str->PadToAlignment(16);
    header.m_offsRotTrack = (uint32_t) ( str->Tell() - headerPos );
    str->Write( rotTrack );
    
    // Re-write header
    uintptr_t endPos = str->Tell();

    
    str->Seek( headerPos );
    
    WriteHeader( str, &header );
    
    str->Seek( endPos );
    
}

//======================================================================================================================
void AnimWriter::WriteHeader( ToolMemStream * str, r3d::AnimStream * header ) {
    str->Write( &header->m_version, 1 );
    str->Write( &header->m_flags, 1 );
    str->Write( &header->m_frameRate, 1 );
    str->Write( &header->m_frameCount, 1 );
    str->Write( &header->m_nodeCount, 1 );
    str->Write( &header->m_duration, 1 );
    
    str->Write( &header->m_offsNodeNames, 1);
    str->Write( &header->m_offsNodeFlags, 1);
    str->Write( &header->m_offsNodeSortedNames, 1);
    str->Write( &header->m_offsNodeSortedNameMap, 1);
    str->Write( &header->m_offsNodeTracks, 1);
    str->Write( &header->m_offsPosTrack, 1 );
    str->Write( &header->m_offsRotTrack, 1 );
    str->Write( &header->m_pad, 1);
}

//======================================================================================================================
void AnimWriter::WriteAnimNode( ToolMemStream * str, r3d::AnimNodeTrackStream * header ) {
    str->Write( &header->m_offsTrackTranslate, 1 );
    str->Write( &header->m_offsTrackRotate, 1 );
    str->Write( &header->m_offsTrackScale, 1 );
    str->Write( &header->m_pad, 1 );
}

//======================================================================================================================
void AnimWriter::BuildAnimNodes( ToolAnimation * anim, std::vector<r3d::AnimNodeTrackStream> & nodes,
                                 std::vector<uint64_t> & nodeNames, std::vector<uint32_t> & nodeFlags,
                                 ToolMemStream & posTrack,  ToolMemStream & rotTrack ) {
    
    for ( uint32_t i = 0; i < anim->m_nodes.size(); ++i ) {
        r3d::AnimNodeTrackStream n;
        memset( &n, 0, sizeof( n ) );
        nodes.push_back( n );
        
        const char * nodeNameStr = anim->m_nodes[i]->m_name.c_str();
        uint64_t nameHash = r3d::fh64::CalcFromString( nodeNameStr );
        
        nodeNames.push_back( nameHash );        
        nodeFlags.push_back( 0 );
    }
    
    for ( uint32_t i = 0; i < anim->m_nodes.size(); ++i ) {
        ToolAnimNode * animNode = anim->m_nodes[i];
        r3d::AnimNodeTrackStream * track = &nodes[i];
        
        // Write out the position track
        
        // Get track offset
        posTrack.PadToAlignment(16);
        track->m_offsTrackTranslate = (uint32_t) posTrack.Tell();
        
        // Write out frames
        for ( auto & p : animNode->m_trackTranslate ) {
            float pos[3] = { p.X(), p.Y(), p.Z() };
            posTrack.Write( pos, 3);
        }
        
        // Write out the rotation track
        
        // Get track offset
        rotTrack.PadToAlignment(16);
        track->m_offsTrackRotate = (uint32_t) rotTrack.Tell();
        
        // Write out frames
        for ( auto & p : animNode->m_trackRotate ) {
            r3d::Quaternion r(p.X(), p.Y(), p.Z(), p.W());
        
            float rot[4] = { r.X(), r.Y(), r.Z(), r.W() };
            rotTrack.Write( rot, 4);
        }
    }
}
