//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_REFOBJECT_H__
#define __I3D_REFOBJECT_H__

#include <stdint.h>
#include <assert.h>

namespace i3d {

    class RefObject {
    public:
        RefObject() {
            m_refCount = 0;
        }

        virtual ~RefObject() {

        }

        void IncRef() {
            ++m_refCount;
        }

        void DecRef() {
            assert( m_refCount > 0 );
            --m_refCount;
            if (m_refCount == 0) {
                delete this;
            }
        }

    protected:
        uint64_t		m_refCount;
    };
}


#endif 
