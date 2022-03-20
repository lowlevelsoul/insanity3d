//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLUNIQUELIST_H__
#define __TOOLUNIQUELIST_H__

namespace i3d {

    template<typename __type__>
    class ToolUniqueList {
    public:
        ToolUniqueList() {
            
        }
        
        ~ToolUniqueList() {
        }
        
        uint32_t AddItem( const __type__ & item, float tol = 0.6e-6f ) {
            return -1;
        }
        
        std::vector<__type__> m_items;
    };

    template<>
    class ToolUniqueList<i3d::Vector3> {
    public:
        ToolUniqueList() {
            
        }
        
        ~ToolUniqueList() {
        }
        
        uint32_t AddItem( const i3d::Vector3 & item, float tol = 0.6e-6f ) {
            uint32_t index = 0;
            for ( const auto & i : m_items) {
                i3d::Vector3 d = i - item;
                float lsqr = d.Dot(d);
                if (lsqr <= (tol * tol)) {
                    return index;
                }
                
                ++index;
            }
            
            index = (uint32_t) m_items.size();
            m_items.push_back( item );
            
            return index;
        }
        
    public:
        std::vector<i3d::Vector3> m_items;
    };
}

#endif
