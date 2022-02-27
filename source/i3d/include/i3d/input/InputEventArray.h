//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_INPUTEVENTARRAY_H__
#define __I3D_INPUTEVENTARRAY_H__

#include "i3d/core/Types.h"
#include "i3d/input/InputTypes.h"

namespace i3d {
    
    class InputEventArray {
    public:
        InputEventArray();
        
        ~InputEventArray();
        
        void Reset();
        
        bool PushKeyEvent( Key key, uint32_t mod, float m_value );
        
        size_t GetCount() const;
        
        const InputEvent & GetEventAt( uint32_t index ) const;
        
    protected:
        i3d::Array<InputEvent>   m_events;
    };
    
    //======================================================================================================================
    inline void InputEventArray::Reset() {
        m_events.Resize(0);
    }
    
    inline size_t InputEventArray::GetCount() const {
        return m_events.GetSize();
    }
    
    //======================================================================================================================
    inline const InputEvent & InputEventArray::GetEventAt( uint32_t index ) const {
        XE_ASSERT( index < m_events.GetSize());
        return m_events[ index ];
    }
}

#endif
