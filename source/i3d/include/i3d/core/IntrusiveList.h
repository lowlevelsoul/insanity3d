//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_INTRUSIVE_LIST_H__
#define __I3D_INTRUSIVE_LIST_H__

namespace i3d {
    
    template<typename __type__>
    class IntrusiveList {
    public:
        typedef __type__  value_t;
        
        typedef __type__ * ptr_t;
        typedef __type__ & ref_t;
        
        typedef const __type__ * const_ptr_t;
        typedef const __type__ & const_ref_t;
        
        struct Iterator {
        public:
            Iterator() {
                m_item = nullptr;
            }
            
            Iterator( const Iterator & rhs ) {
                m_item = rhs.m_item;
            }
            
            ~Iterator() {
                // Empty - does nothing
            }
            
        protected:
            IntrusiveList *     m_item;
        };
        
        IntrusiveList() {
            m_root = this;
            m_prev = this;
            m_next = this;
            m_value = nullptr;
        }
        
        IntrusiveList( ptr_t rhs ) {
            m_root = this;
            m_prev = this;
            m_next = this;
            m_value = rhs;
        }
        
        ~IntrusiveList() {
            Remove();
        }
        
        void InsertBefore( IntrusiveList * item ) {
            if (item->m_root != item ) {
                item->Remove();
            }

            item->m_prev = m_prev;
            item->m_next = this;
            item->m_root = m_root;
            
            item->m_prev->m_next = item;
            item->m_next->m_prev = item;
        }
        
        void InsertAfter( IntrusiveList * item ) {
            if (item->m_root != item ) {
                item->Remove();
            }

            item->m_prev = this;
            item->m_next = m_next;
            
            item->m_prev->m_next = item;
            item->m_next->m_prev = item;
            item->m_root = m_root;
        }

        void Remove() {
            if ( m_root == this ) {
                return;
            }
            
            m_prev->m_next = m_next;
            m_next->m_prev = m_prev;
            
            m_prev = this;
            m_next = this;
            m_root = this;
        }
        
        IntrusiveList * Root() const { return m_root; }
        IntrusiveList * Prev() const { return m_prev; }
        IntrusiveList * Next() const { return m_next; }
        
        void Set( const_ptr_t rhs ) { m_value = rhs; }
        
        ptr_t Get() { return m_value; }
        
        const_ptr_t Get() const { return m_value; }
        
        operator ptr_t() { return m_value; }
        
        operator const_ptr_t() const { return m_value; }
        
        bool Empty() const { return m_next == this; }
        
        bool IsRoot() const { return this == m_root; }
        
    protected:
        IntrusiveList *         m_root;
        IntrusiveList *         m_prev;
        IntrusiveList *         m_next;
        ptr_t                   m_value;
    };
}


#endif
