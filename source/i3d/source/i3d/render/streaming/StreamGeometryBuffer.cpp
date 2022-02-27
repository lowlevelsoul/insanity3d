//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "r3d/render/streaming/StreamGeometryBuffer.h"

#if 0

namespace r3d {

    //======================================================================================================================
    bool StreamGeometryBuffer::Alloc( PageList & pages, size_t elementSize, size_t elementCount ) {
        
        // Calculate the number of required pages, in the word case scenario
        size_t elementsPerPage = m_pageSize / elementSize;
        size_t wantedPageCount = elementCount / elementsPerPage;
        wantedPageCount += ((elementCount % elementsPerPage) == 0) ? 0 : 1;
    
        Page * first = m_pageFreeList;
        Page * last = first;
        size_t pageCount = 1;
        bool isContiguous = true;
        
        // Early out if there are no blocks
        if (last == nullptr ) {
            return false;
        }
        
        do {
            // Check to see if there are no more pages. If not, we need to break
            // out of the loop
            if (last->m_next == nullptr) {
                break;
            }
            
            // Check to see if the next page is contiguous. If not, then
            // we set the contiguous flag to be false.
            if (last+1 != last->m_next) {
                isContiguous = false;
            }
            
            // Next page
            last = last->m_next;
            ++pageCount;

            if ( pageCount == wantedPageCount ) {
                break;
            }
            
        } while ( last != nullptr );
        
        if ( isContiguous == true ) {
            
            // We have a lost of pages that are contiguous. Let's check to see if we
            // can shrink that list a little by fully occupying the pages.
            // Note: We do this before checking if we have enough pages or not, because
            //       it could be that we can shrink the page list enough to use the
            //       remaining page.
            
            size_t totalDataSize = (elementSize * elementCount);
            size_t contigPageCount = totalDataSize / m_pageSize;
            contigPageCount += ((totalDataSize % m_pageSize) != 0) ? 1 : 0;
            
            // If the nunber of contiguous pages is less than the page count, then
            // we can shrink the page requirements
            if ( contigPageCount <= pageCount) {
                last = &first[ contigPageCount - 1 ];
                wantedPageCount = contigPageCount;
                pageCount = pageCount;
            }
        }
        
        // Did we get enough pages? If not, bail with an error
        if ( pageCount < wantedPageCount ) {
            return false;
        }
        
        // Fill in the page list for the caller
        pages.m_first = first;
        pages.m_last = last;
        pages.m_count = pageCount;
        pages.m_elementSize = elementSize;
        pages.m_elementCount = elementCount;
        pages.m_elementCountPerPage = elementsPerPage;
        pages.m_contiguous = isContiguous;
        
        // Remove the allocated pages from the free list
        m_pageFreeList = last->m_next;
        m_pageFreeCount -= pageCount;
        last->m_next = nullptr;
        
        return true;
    }

    //======================================================================================================================
    void StreamGeometryBuffer::Free( PageList & pages ) {
    
    }

}

#endif
