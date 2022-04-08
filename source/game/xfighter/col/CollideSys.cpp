//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/CollideSys.h"
#include "col/Collider.h"
#include "col/ColShape.h"

static uint8_t colSysMemory[sizeof(CollideSys)];
CollideSys* colSys = nullptr;

i3d::CVar   col_dbgDraw("col_dbgDraw", false, "Enables / disables the drawing of collision debug info");
i3d::CVar   col_dbgDrawShapes("col_dbgDrawShapes", false, "Enables / disables the drawing of collision shapes");
i3d::CVar   col_dbgDrawCells("col_dbgDrawCells", false, "Enables / disables the drawing of collision cells");
i3d::CVar   col_dbgDrawPairs("col_dbgDrawPairs", false, "Enables / disables the drawing of collision pairs");

i3d::CVar   col_dbgDrawShapesGroups("col_dbgDrawShapesGroups", "", "String defining which groups to draw for collisions shapes");
i3d::CVar   col_dbgDrawCellGroups("col_dbgDrawCellGroups", "", "String defining which groups to draw for collisions shapes");
i3d::CVar   col_dbgDrawPairGroups("col_dbgDrawPairGroups", "", "String defining which groups to draw for collisions pairs");

//======================================================================================================================
void CollideCreate() {
    if (colSys == nullptr ) {
        colSys = new( colSysMemory ) CollideSys;
    }
}

//======================================================================================================================
void CollideDestroy() {
    if ( colSys == nullptr ) {
        colSys->~CollideSys();
        colSys = nullptr;
    }
}

//======================================================================================================================
CollideSys::CollideSys() {    
    m_removeCount = 0;
    m_debugDrawBoundsGroups = 0;
    m_debugDrawCelGroups = 0;
    m_debugDrawPairs = 0;
}

//======================================================================================================================
CollideSys::~CollideSys() {
    
}

//======================================================================================================================
void CollideSys::Initialise(const i3d::Vector2& areaBoundsMin, const i3d::Vector2& areaBoundsMax) {
    
    XE_LOG("ColSys:Init\n Bounds <%5.5f, %5.5f> <%5.5f, %5.5f>\n",
         areaBoundsMin.X(), areaBoundsMin.Y(), areaBoundsMax.X(), areaBoundsMax.Y());
    
    m_removeList.resize(SHAPE_CAPACITY);
    m_dirtyList.resize(SHAPE_CAPACITY);
    m_dirtyCount = 0;
    
    m_removeCount = 0;
    
    m_quadTree.Initialise(areaBoundsMin, areaBoundsMax, 4);
    
    CreateDebugVerts();
    SetupDebugColours();
    
    AddMaskName( col::GROUP_PLAYER, "player" );
    AddMaskName( col::GROUP_ENEMY, "enemy" );
    AddMaskName( col::GROUP_ENEMY_BULLET, "enemy_bullet" );
    AddMaskName( col::GROUP_PLAYER_BULLET, "player_bullet" );
}

//======================================================================================================================
void CollideSys::AddMaskName( uint32_t value, const char * name ) {
    uint64_t hash = i3d::fh64::CalcFromString( name );
    m_maskNameHashes[ hash ] = value;
}

//======================================================================================================================
void CollideSys::Finalise() {
    
}

//======================================================================================================================
void CollideSys::SetupDebugColours() {
    m_debugColours[0].Set(0, 0, 1, 1);
    m_debugColours[1].Set(0, 1, 0, 1);
    m_debugColours[2].Set(1, 0, 0, 1);
    m_debugColours[3].Set(0, 1, 1, 1);
    m_debugColours[4].Set(1, 0, 1, 1);
    m_debugColours[5].Set(1, 1, 0, 1);
    m_debugColours[6].Set(1, 1, 1, 1);
    m_debugColours[7].Set(0.5f, 0.5f, 0.5f, 1);
}

//======================================================================================================================
void CollideSys::CreateDebugVerts() {
    float currAngle = 0;
    float arc = i3d::scalar::PI / ((float)(CIRCLE_VERTS_COUNT) / 2);
    
    float x = 0;
    float z = 1;
    
    for(int32_t i=0; i < (int32_t) CIRCLE_VERTS_COUNT; ++i) {
        float s = std::sin(currAngle);
        float c = std::cos(currAngle);
        
        float rx = x * c + z * s;
        float rz = x * s - z * c;
        
        m_circleVerts[i].Set(rx, 0, rz);
        
        currAngle += arc;
    }
}

//======================================================================================================================
void CollideSys::Think(float timeStep) {
    UpdateDirty();
    
    BuildPairs();
    
    TestPairs();
}


//======================================================================================================================
uint32_t CollideSys::FindMask( const char * name ) const {
    MaskMap::const_iterator findIt = m_maskNameHashes.find( i3d::fh64::CalcFromString( name ) );
    if (findIt == m_maskNameHashes.end()) {
        return 0;
    }
    
    return findIt->second;
}

//======================================================================================================================
void CollideSys::UpdateDirty() {
    
    for(uint32_t d = 0; d < m_dirtyCount; ++d) {
        Collider* dirtyObj = m_dirtyList[d];
        
        // JS 05/04/2020
        // An object may have been removed from the system after being added to
        // the dirty list. When this happens, the entry in the list is marked as
        // null
        if (dirtyObj == nullptr) {
            continue;
        }

        dirtyObj->UpdateBounds();
        
        Cell* cell = m_quadTree.DetermineCell(dirtyObj);
    
        dirtyObj->AddToCell(cell);
        dirtyObj->ClearDirty();
    }
    
    m_dirtyCount = 0;
}

//======================================================================================================================
void CollideSys::BuildPairs() {
    m_pairs.clear();
    
    for(Collider::ListNode* shapeNode = m_shapeList.Next();
        shapeNode->IsRoot() == false;
        shapeNode = shapeNode->Next()) {
        
        Collider* obj = *shapeNode;
        
        // Bulild a list of pairs for the shapes current cell
        BuildPairsForShape(obj, obj->m_cell);
        
        // Build a list of pairs for in the parent cells
        for(Cell* parentCell = obj->m_cell->m_parent; parentCell != nullptr; parentCell = parentCell->m_parent) {
            BuildPairsForShape(obj, parentCell);
        }
        
        if (obj->m_cell->m_children[0] != nullptr) {
            for(int32_t c=0;  c < 4; ++c) {
                BuildChildPairs(obj, obj->m_cell->m_children[c]);
            }
        }
    }
}

//======================================================================================================================
void CollideSys::TestPairs() {
    
    // Perform collision tests
    for ( auto & p : m_pairs ) {
        Pair & pair = p.second;
        TestShape( pair.m_testResult,  pair.m_shape0->m_shape, pair.m_shape1->m_shape);
    }
    
    // Peform collision response
    for ( auto & p : m_pairs ) {
        
        Pair & thisPair = p.second;
    
        if ( thisPair.m_testResult.m_overlap == true ) {
            
            bool callbackShape0 = thisPair.m_shape0->m_filter & thisPair.m_shape1->m_group;
            bool callbackShape1 = thisPair.m_shape1->m_filter & thisPair.m_shape0->m_group;
            
            if ( callbackShape0 ) {
                thisPair.m_shape0->DoHitCallback(thisPair.m_testResult, thisPair.m_shape1);
            }
            
            if ( callbackShape1 ) {
                OverlapResult result1 = thisPair.m_testResult;
                result1.m_axis = -result1.m_axis;
                thisPair.m_shape1->DoHitCallback( result1, thisPair.m_shape0 );
            }
        }
    }
}

//======================================================================================================================
void CollideSys::BuildChildPairs(Collider* obj, Cell* cell) {
    
    if (cell->HasShapes() == true) {
        BuildPairsForShape(obj, cell);
    }
    
    if (cell->m_children[0] != nullptr) {
        for(int32_t c=0; c < 4; ++c) {
            BuildChildPairs(obj, cell->m_children[c]);
        }
    }
}

//======================================================================================================================
void CollideSys::BuildPairsForShape( Collider * obj, Cell * cell) {
    XE_ASSERT(cell != nullptr);
    
    // Loop through all of the shapes in this
    for( Collider::ListNode * otherNode = cell->m_objects.Next();
        otherNode->IsRoot() == false;
        otherNode = otherNode->Next()) {
        
        Collider * otherShape = *otherNode;
        
        // Make sure we're not trying to collide the shape with itself and
        // check that the other shape and the shape are allowed to collide
        if ((otherShape != obj) &&
            ((otherShape->m_group & obj->m_filter) != 0) &&
            ((otherShape->m_filter & obj->m_group) != 0)) {
#if 0
            // Calculate the squared maximum seperating distance
            float maxDistSqr = (obj->m_radius * obj->m_radius) + (otherShape->m_radius * otherShape->m_radius);
            
            // Calculate the squared distance between the two shapes and adjust so that overlap occurs if
            // the distance <= zero
            Vector2 delta = otherShape->m_pos - obj->m_pos;
            float distSqr = delta.Dot(delta);
            distSqr -= maxDistSqr;
            
            if (distSqr <= 0) {
#else
            {
#endif
                XE_ASSERT( obj->m_shape != nullptr );
                XE_ASSERT( otherShape->m_shape != nullptr );
                
                // The two shapes are over lapping! We attempt to add a collision pair
                Pair pair;
                pair.Set( obj, otherShape );
                
                uint64_t pairHash = pair.CalcHash();
                PairMap::iterator findIt = m_pairs.find( pairHash );
                if ( findIt == m_pairs.end() ) {
                    m_pairs[pairHash] = pair;
                }
            }
        }
    }
}

//======================================================================================================================
void CollideSys::AddDirty(Collider* obj) {
    assert(obj->IsDirty() == false);
    assert(m_dirtyCount < SHAPE_CAPACITY);
    
    // JS - Need to store the index of object in the dirty list
    obj->m_dirtyIndex = (int32_t)m_dirtyCount;
    
    m_dirtyList[(int32_t)m_dirtyCount] = obj;
    ++m_dirtyCount;
    
    obj->SetDirty();
}

//======================================================================================================================
void CollideSys::AddShape(Collider* obj) {
    assert(obj->IsAdded() == false);
    
    m_shapeList.InsertAfter( &obj->m_listNode );

    AddDirty(obj);
}

//======================================================================================================================
void CollideSys::RemoveShape(Collider* obj) {
    assert(obj != nullptr);
    assert(obj->IsAdded() == true);
    
    // JS 05/04/2020
    // Need to check if the object is in the dity list. If so, we'll mark the entry
    // as nul. This fixes an issue where removed objects were still being marked as dirty.
    if (obj->IsDirty() == true) {
        assert(obj->m_dirtyIndex != -1);
        m_dirtyList[obj->m_dirtyIndex] = nullptr;
    }
    
    // Remove the shape from the cell that it occupies
    obj->RemoveFromCell();
    obj->m_listNode.Remove();
    obj->ClearDirty();
}

//======================================================================================================================
uint32_t CollideSys::BuildCollisionMaskFromString( const char * str ) {
    const char * currStr = col_dbgDrawShapesGroups.GetString();
    char itemBuff[128];
    
    uint32_t mask = 0;
    
    bool gotStr = false;
    do {
        gotStr = i3d::Sys::GetNextStringItem(currStr, itemBuff, 128);
        if ( gotStr == true ) {
            uint32_t thisMask = FindMask( itemBuff );
            if ( thisMask != 0 ) {
                mask |= thisMask;
            }
        }
        
    } while( gotStr == true);
    
    return mask;
}
