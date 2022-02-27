//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __STREAMGEOMETRYBUFFER_H__
#define __STREAMGEOMETRYBUFFER_H__



#if 0

#include "r3d/container/Array.h"

namespace r3d {
    
    class StreamGeometryBuffer {
    public:
        class Params {
        public:
            Params() {
                
            }
            
            size_t      m_size;         ///< Size of the buffer in bytes
            size_t      m_pageSize;     ///< Size of the pages that the buffer is split into

        };
        
        struct Page {
            uint64_t        m_age;          ///< How many frames that this page head has not been used
            uint64_t        m_offset;       ///< Offset into the buffer for the start of this page
            Page *          m_next;         ///< Pointer to the next page in this page list
            Page *          m_usedNext;    ///<
        };
        
        struct PageList {
            Page *      m_first;                ///< The first page in the list
            Page *      m_last;                 ///< The last page in the list
            size_t      m_count;                ///< The number of pages that we allocated
            size_t      m_elementSize;          ///< The size of the elements contained in the pages
            size_t      m_elementCount;         ///< Number of total elements the pages will contain
            size_t      m_elementCountPerPage;  ///< Number of complete elements that fit in a single page
            bool        m_contiguous;           ///< Are the allocated pages contiguous?
        };
        
        virtual ~StreamGeometryBuffer() {
            // Empty page
        }
        
        virtual void Initialise( size_t size, size_t pageSize = 1024 * 16) = 0;
        
        bool Alloc( PageList & pages, size_t elementSize, size_t elementCount );
        
        void Free( PageList & pages );
        
    protected:
        Page *                      m_pageCache;                ///< 
        Page *                      m_pageFreeList;             ///< Linked list of free pages
        
        size_t                      m_pageSize;                 ///< The size of a single page
        size_t                      m_pageFreeCount;            ///< Number of free pages
    };
};

#endif

#endif
