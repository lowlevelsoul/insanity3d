//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_COLQUADTREE_H__
#define __XFIGHTER_COLQUADTREE_H__

#include "col/ColTypes.h"
#include "i3d/core/IntrusiveList.h"
    
class Shape;
class Collider;

class Cell {
public:
    static const size_t CHILD_COUNT = 4;
    typedef i3d::IntrusiveList<Collider> ObjectNode;
    
    i3d::Vector2            m_boundsMin;
    i3d::Vector2            m_boundsMax;
    i3d::Vector2            m_size;
    Cell*                   m_parent;
    Cell*                   m_children[CHILD_COUNT];
    ObjectNode              m_objects;
    
    void Initialise(Cell* parent, int32_t x, int32_t y);
    
    void Initialise(const i3d::Vector2& boundsMin, const i3d::Vector2& boundsMax);
    
    bool IsCompletelyContained(const i3d::Vector2& bmin, const i3d::Vector2& bmax) const {
        return (bmin.X() >= m_boundsMin.X() && bmax.X() <= m_boundsMax.X() &&
                bmin.Y() >= m_boundsMin.Y() && bmax.Y() <= m_boundsMax.Y());
    }
    
    bool HasChildren() const {
        return m_children[0] != nullptr;
    }
    
    void CalcChildIndices(int32_t& x0, int32_t& y0, int32_t& x1, int32_t& y1, const i3d::Vector2& bmin, const i3d::Vector2& bmax) const {
        i3d::Vector2 localBMin = bmin - m_boundsMin;
        i3d::Vector2 localBMax = bmax - m_boundsMin;
        
        i3d::Vector2 halfSize = m_size / 2.0f;
        
        x0 = (int32_t) (localBMin.X() / halfSize.X());
        x1 = (int32_t) (localBMax.X() / halfSize.X());
        y0 = (int32_t) (localBMin.Y() / halfSize.Y());
        y1 = (int32_t) (localBMax.Y() / halfSize.Y());
        
        assert(x0 >= 0 && x0 <= 1);
        assert(y0 >= 0 && y0 <= 1);
        assert(x1 >= 0 && x1 <= 1);
        assert(y1 >= 0 && y1 <= 1);
    }
    
    Cell* GetChildAt(int32_t x, int32_t y) const {
        assert(x >= 0 && x <= 1);
        assert(y >= 0 && y <= 1);
        return m_children[ (y*2) + x];
    }
    
    bool HasShapes() const {
        return m_objects.Next()->IsRoot() == false;
    }
};


class QuadTree {
public:
    typedef i3d::stl::Vector<Cell> CellArray;
    
    CellArray::type         m_cells;            ///< Array of cells that are allocated for the quad tree
    Cell*                   m_root;             ///< The root bode
    
    static uint32_t _CalcDepth(float startW, float startH, float minSize, uint32_t maxDepth);
    
    static size_t _CalcNumberOfCells(int32_t depth);
    
    static size_t _CalcNumberOfCells(int32_t &depth, float startW, float startH, float minSize, uint32_t maxDepth);
    
    QuadTree();
    
    ~QuadTree();
    
    void Initialise(const i3d::Vector2& boundsMin, const i3d::Vector2& boundsMax, int32_t depth);
    
    void BuildTree(Cell* parent, size_t& numCells, int32_t depth, int32_t maxDepth);
    
    Cell* DetermineCell( Collider* obj);
    
    Cell* DetermineChildCell( Cell* startCell, const i3d::Vector2& bmin, const i3d::Vector2& bmax);
};

#endif
