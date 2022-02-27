//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTITYPE_H__
#define __I3D_RTTITYPE_H__

#include "i3d/core/Types.h"
#include "i3d/rtti/RttiPropType.h"
#include "i3d/rtti/RttiPropTypeTraits.h"

namespace i3d {

    class RttiEnumType {
    protected:
        static RttiEnumType *   INIT_LIST_HEAD;
        static RttiEnumType *   INIT_LIST_TAIL;
        RttiEnumType *          m_initNext;
        
        RttiEnumElement *       m_elements;
        const char *            m_name;
    public:
        RttiEnumType(const char * name, const RttiEnumElement * elements);
        
        ~RttiEnumType();
    };

    class RttiObject;
    typedef RttiObject* (*RttiObjectFactory)();

    class RttiType {
        friend class RttiSystem;
        friend class RttiSystemLocal;
    public:
        static RttiType*    INIT_LIST_HEAD;
        static RttiType*    INIT_LIST_TAIL;
        RttiType*           m_initNext;
    protected:
        struct PropInfo {
            std::vector<uint64_t>           m_propHashes;
            std::vector<int32_t>            m_propMap;
            std::vector<RttiPropDef>        m_props;
        };
        
        const char*         m_name;                 ///< The name of this class
        uint64_t            m_nameId;               ///< The hashed value of the name
        RttiType *          m_super;                ///< Pointer to the super/parents class type
        RttiObjectFactory   m_factory;              ///< Factory function for creating an instance of the class represented by this type
        const RttiPropDef * m_typeProps;            ///< Props that are reigstered by this type
        uint32_t            m_propStart;            ///< Index into the global property table for the start of this types props
        uint32_t            m_propCount;            ///< Number of properties
        
        PropInfo *          m_props;
        

    public:
        
        RttiType(const char* name, RttiType* super, RttiObjectFactory factory, const RttiPropDef* props);

        RttiObject* Create();

        const RttiType* GetSuper() const { return m_super; }
        
        RttiType* GetSuper() { return m_super; }
        
        const RttiPropDef* FindProp(const char* name);
        
        const RttiPropDef* FindProp(uint64_t nameHash);
        
        const char* GetName() const { return m_name; }
        
        uint64_t GetNameId() const { return m_nameId; }
        
        bool IsOfType(const RttiType* type) const;
        
        size_t GetPropCount() const { return (m_props == nullptr) ? 0 : m_props->m_props.size(); }
        
        const RttiPropDef * GetPropAt( uint32_t index ) const {
            assert(m_props != nullptr);
            assert(index < GetPropCount() );
            return &m_props->m_props[index];
        }
        
    protected:
        
        void PublishProps();
    };
    
}

#define RTTI_OFFSET_OF(__struct__, __member__) ((uintptr_t)&(((__struct__*)(0))->__member__))

#define RTTI_CLASS_DECLARE_NAMED(__type__, __super__)\
    public: typedef __type__ ThisType;\
    public: typedef __super__ Super;\
    public: typedef i3d::RttiRefPointer<__type__> ref_ptr_t;\
    protected: static i3d::RttiType TYPE;\
    protected: static const i3d::RttiPropDef PROPS[];\
    public: static i3d::RttiType* GetClassType();\
    public: virtual i3d::RttiType* GetType() override;\
    public: static i3d::RttiObject* Factory();

#define RTTI_CLASS_DECLARE(__type__, __super__)\
    public: typedef __type__ ThisType;\
    public: typedef __super__ Super;\
    public: typedef i3d::RttiRefPointer<__type__> ref_ptr_t;\
    protected: static i3d::RttiType TYPE;\
    protected: static const i3d::RttiPropDef PROPS[];\
    public: static i3d::RttiType* GetClassType();\
    public: virtual i3d::RttiType* GetType() override;\
    public: static i3d::RttiObject* Factory();

#define RTTI_CLASS_BEGIN(__type__)\
    const i3d::RttiPropDef __type__::PROPS[] = {

#define RTTI_PROP(__type__, __name__, __member__) { i3d::RTTI_PROP_TYPE_##__type__, __name__, 0, (uint32_t)RTTI_OFFSET_OF(ThisType, __member__), 0 },
#define RTTI_PROP_ARRAY(__type__, __name__, __member__) { i3d::RTTI_PROP_TYPE_##__type__, __name__, 0, (uint32_t)RTTI_OFFSET_OF(ThisType, __member__), 1 },

#define RTTI_CLASS_END_NAMED(__type__, __name__)\
    { i3d::RTTI_PROP_TYPE_NULL, nullptr, 0, 0}\
    };\
    i3d::RttiType __type__::TYPE(__name__, __type__::Super::GetClassType(), __type__::Factory, __type__::PROPS);\
    i3d::RttiType* __type__::GetClassType() { return &__type__::TYPE; }\
    i3d::RttiType* __type__::GetType() { return &TYPE; }\
    i3d::RttiObject* __type__::Factory() { return new __type__; }

#define RTTI_CLASS_END(__type__) RTTI_CLASS_END_NAMED( __type__, #__type__)

#endif
