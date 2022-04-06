//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/Collider.h"
#include "col/ColQuadTree.h"
    
//======================================================================================================================
#pragma mark Cell
//======================================================================================================================

//======================================================================================================================
void Cell::Initialise(Cell* parent, int32_t x, int32_t y) {
    m_parent = parent;
    
    // Calculate the bounds of this cell
    i3d::Vector2 parentCellSize = parent->m_boundsMax - parent->m_boundsMin;
    i3d::Vector2 cellSize = parentCellSize / 2.0f;
    
    float offsetX = (x == 0) ? 0 : cellSize.X();
    float offsetY = (y == 0) ? 0 : cellSize.Y();
    i3d::Vector2 offset(offsetX, offsetY);
    
    m_boundsMin = parent->m_boundsMin + offset;
    m_boundsMax = m_boundsMin + cellSize;
    m_size = cellSize;
    
    m_children[0] = nullptr;
    m_children[1] = nullptr;
    m_children[2] = nullptr;
    m_children[3] = nullptr;
}

//======================================================================================================================
void Cell::Initialise(const i3d::Vector2& boundsMin, const i3d::Vector2& boundsMax) {
    m_boundsMin = boundsMin;
    m_boundsMax = boundsMax;
    m_size = boundsMax - boundsMin;
    m_parent = nullptr;

    m_children[0] = nullptr;
    m_children[1] = nullptr;
    m_children[2] = nullptr;
    m_children[3] = nullptr;
}

//======================================================================================================================
#pragma mark QuadTree
//======================================================================================================================

//======================================================================================================================
uint32_t QuadTree::_CalcDepth(float startW, float startH, float minSize, uint32_t maxDepth) {
    uint32_t currDepth = 0;
    float currW = startW;
    float currH = startH;
    
    do {
        if (currDepth >= maxDepth) {
            break;
        }
        
        currW = currW / 2.0f;
        currH = currH / 2.0f;
        if (currW < minSize || currH < minSize) {
            break;
        }
        
        ++currDepth;
        
    } while(true);

    return currDepth;
}

//======================================================================================================================
size_t QuadTree::_CalcNumberOfCells(int32_t depth) {
    size_t currDim = 1;
    size_t count = 0;
    
    for(int32_t i=0; i <= depth; ++i) {
        count += (currDim * currDim);
        currDim *= 2;
    }
    
    return count;
}



//======================================================================================================================
QuadTree::QuadTree() {
    m_root = nullptr;
}

//======================================================================================================================
QuadTree::~QuadTree() {
    
}

//======================================================================================================================
void QuadTree::Initialise(const i3d::Vector2 & boundsMin, const i3d::Vector2 & boundsMax, int32_t depth) {
    
    XE_LOG("Creating QuadTree\n");
    
    i3d::Vector2 boundsSize = boundsMax - boundsMin;
    uint32_t maxDepth = _CalcDepth(boundsSize.X(), boundsSize.Y(), 64.0f, depth);
    XE_LOG("          Max depth: %u\n", maxDepth);
    
    size_t cellCount = _CalcNumberOfCells(maxDepth);
    XE_LOG("    Number of cells: %u\n", cellCount);
    
    m_cells.resize(cellCount);
    
    m_root = &m_cells[0];
    m_root->Initialise(boundsMin, boundsMax);
    
    XE_LOG("    Building tree\n");
    size_t numCells = 1;
    BuildTree(m_root, numCells, 1, maxDepth);
}

//======================================================================================================================
void QuadTree::BuildTree(Cell* parent, size_t& numCells, int32_t depth, int32_t maxDepth) {
    assert(numCells+4 <= m_cells.size());
    
    parent->m_children[0] = &m_cells[(int32_t)numCells];
    parent->m_children[1] = &m_cells[(int32_t)numCells+1];
    parent->m_children[2] = &m_cells[(int32_t)numCells+2];
    parent->m_children[3] = &m_cells[(int32_t)numCells+3];
    
    numCells += 4;
    
    parent->m_children[0]->Initialise(parent, 0, 0);
    parent->m_children[1]->Initialise(parent, 1, 0);
    parent->m_children[2]->Initialise(parent, 0, 1);
    parent->m_children[3]->Initialise(parent, 1, 1);
    
    if (depth < maxDepth) {
        for(int32_t i=0; i < 4; ++i) {
            BuildTree(parent->m_children[i], numCells, depth+1, maxDepth);
        }
    }
}

//======================================================================================================================
Cell* QuadTree::DetermineCell(Collider * obj) {
    
    // Determine starting cell. If the shape is attached to a cell, then we'll start there. If the
    // shape is not attached to a cell, we'll start from the root.
    Cell* currCell = obj->m_cell != nullptr ? obj->m_cell : m_root;
    
    i3d::Vector2 shapeBoundsMin, shapeBoundsMax;
    obj->GetWorldBounds(shapeBoundsMin, shapeBoundsMax);
    
    bool loop = true;
    
    do {
    
        bool contained = currCell->IsCompletelyContained(shapeBoundsMin, shapeBoundsMax);
        if (contained) {
            // The shape is fully contained within currCell, so we want to iterate
            // down the tree to see it may be in a child of currCell
            currCell = DetermineChildCell(currCell, shapeBoundsMin, shapeBoundsMax);
            loop = false;
            
        } else {
            // Go to the cell parent and on the start of the next iteration, check
            // to see if the shape is wholly contained within the cell.
            if (currCell->m_parent == nullptr) {
                // No parent to go to, so just use this cell
                // TODO: If we're here, this means that the shape is outside of the tree and should be flagged as such
                loop = false;
            }
            else {
                currCell = currCell->m_parent;
            }
        }
        
    } while(loop == true);
    
    return currCell;
}
    
    
//======================================================================================================================
Cell* QuadTree::DetermineChildCell( Cell* startCell, const i3d::Vector2 & bmin, const i3d::Vector2 & bmax ) {
    bool loopDown = true;
    
    Cell* currCell = startCell;
    
    int loopCount = 0;
    
    do {
        // Check to see if we're at the bottom of the tree.
        bool hasChildren = currCell->HasChildren();
        if ( hasChildren == false ) {
            // We can't go any further, so just use the currCell and bail
            loopDown = false;
        }
        else {
            // Work out the child cells that the shape covers
            int32_t x0, y0, x1, y1;
            currCell->CalcChildIndices( x0, y0, x1, y1, bmin, bmax );
            if ( ( x0 == x1 ) && ( y0==y1 ) ) {
                // The shape only covers a single child, so we get that child and loop
                // again
                currCell = currCell->GetChildAt( x0, y1 );
            }
            else {
                // The shape covers more than one child, so this cell is the cell thay we want
                loopDown = false;
            }
        }
        
        ++loopCount;
        
    } while( loopDown == true );
    
    return currCell;
}

