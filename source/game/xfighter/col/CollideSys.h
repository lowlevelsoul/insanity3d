//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLLIDESYS_H__
#define __COLLIDESYS_H__

#include "col/ColTypes.h"
#include "col/ColQuadTree.h"
#include "col/Collider.h"
#include "col/Collide_overlaptest.h"

// Pointer to the one and only collision system
class CollideSys;
extern CollideSys* colSys;

class CollideSys {
public:
    static const size_t CIRCLE_VERTS_COUNT = 24;
    static const size_t MEMORY_SIZE = 1024 * 1024 * 2;
    static const size_t DEBUG_COLOUR_COUNT = 8;
    static const size_t PAIR_CAPACITY = 4096;
    static const size_t SHAPE_CAPACITY = 512;
            
    struct Pair {
        Collider *              m_shape0;
        Collider *              m_shape1;
        OverlapResult           m_testResult;
        
        void Set( Collider * obj0, Collider * obj1) {
            assert(obj0 != obj1);
            if ( obj0 < obj1 ) {
                m_shape0 = obj0;
                m_shape1 = obj1;
            }
            else {
                m_shape0 = obj1;
                m_shape1 = obj0;
            }
            
            m_testResult.m_overlap = false;
        }
        
        uint64_t CalcHash() const {
            uint64_t hash = i3d::fh64::Calc( &m_shape0, sizeof( Shape * ) * 2 );
            return hash;
        }
        
        void GetBounds( i3d::Vector2 & bmin, i3d::Vector2 & bmax) {
            i3d::Vector2 bmin0, bmax0;
            i3d::Vector2 bmin1, bmax1;
            
            m_shape0->GetWorldBounds( bmin0, bmax0 );
            m_shape1->GetWorldBounds( bmin1, bmax1 );
            
            bmin.Min( bmin0, bmin1 );
            bmax.Max( bmax0, bmax1 );
        }
    };
    
    typedef i3d::stl::Vector<Collider*>             ObjectArray;
    typedef i3d::stl::Vector<Pair>                  PairArray;
    typedef i3d::stl::Vector<Pair*>                 PairPointerArray;
    typedef i3d::stl::Vector<uint64_t>              PairHashArray;
    typedef i3d::stl::Map<uint64_t, Pair>           PairMap;
    
    typedef i3d::stl::Map<uint64_t, uint32_t>       MaskMap;
    
    Collider::ListNode                  m_shapeList;            ///< The list of shapes in the collision system
    ObjectArray::type                   m_dirtyList;            ///< List of nodes that are dirty and must be updated
    size_t                              m_dirtyCount;           ///< Number of nodes that are dirty
    ObjectArray::type                   m_removeList;
    PairMap::type                       m_pairs;                
    
    size_t                              m_removeCount;
    uint32_t                            m_debugDrawBoundsGroups;
    uint32_t                            m_debugDrawCelGroups;
    uint32_t                            m_debugDrawPairs;
    
    QuadTree                            m_quadTree;
    
    i3d::Vector3                        m_circleVerts[CIRCLE_VERTS_COUNT];
    i3d::Vector4                        m_debugColours[DEBUG_COLOUR_COUNT];
    
    MaskMap::type                       m_maskNameHashes;
    
    CollideSys();
    
    ~CollideSys();
    
    void Initialise(const i3d::Vector2& areaBoundsMin, const i3d::Vector2& areaBoundsMax);
    
    void Finalise();
    
    void Think(float timeStep);
    
    uint32_t FindMask( const char * name ) const;
    
    void UpdateDirty();
    
    void TestPairs();
    
    void BuildPairs();
    
    void BuildPairsForShape(Collider* obj, Cell* cell);
    
    void BuildChildPairs(Collider*, Cell* cell);
    
    void CreateDebugVerts();
    
    void SetupDebugColours();
    
    void AddShape(Collider* obj);
    
    void RemoveShape(Collider* obj);
    
    void AddDirty(Collider* obj);
    
    void DrawDebugCircle(const i3d::Vector2& pos, float radius, const i3d::Vector4& colour);
    
    void DrawAxisBox(const i3d::Vector2& bmin, const i3d::Vector2& bmax, const i3d::Vector4& colour);
    
    void DebugDraw();
    
    void DrawCell(Cell* cell);
    
    void DrawPair(Pair* pair);
    
    void DebugSetDrawBoundsGroups(uint32_t group) { m_debugDrawBoundsGroups = group; }
    
    void DebugSetDrawCellGroups(uint32_t group) { m_debugDrawCelGroups = group; }
    
    void AddMaskName( uint32_t value, const char * name );
    
protected:
    
    uint32_t BuildCollisionMaskFromString( const char * str );
};

#endif
