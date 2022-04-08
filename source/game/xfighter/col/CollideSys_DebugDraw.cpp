//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/CollideSys.h"
#include "col/Collider.h"
#include "col/ColShape.h"
#include "col/ColShapeBox.h"
#include "col/ColShapeCircle.h"

extern i3d::CVar col_dbgDraw;
extern i3d::CVar col_dbgDrawShapesGroups;
extern i3d::CVar col_dbgDrawCellGroups;
extern i3d::CVar col_dbgDrawPairGroups;
extern i3d::CVar col_dbgDrawCells;
extern i3d::CVar col_dbgDrawPairs;

//======================================================================================================================
void CollideSys::DrawDebugCircle(const i3d::Vector2& pos, float radius, const i3d::Vector4& colour) {
    i3d::Vector3 verts[CIRCLE_VERTS_COUNT];
    i3d::Vector3 p3(pos.X(), 0, pos.Y());
    
    for(int32_t i=0; i < (int32_t) CIRCLE_VERTS_COUNT; ++i) {
        verts[i] = p3 + (m_circleVerts[i] * radius);
    }
    
    render->DrawClosedLineList(verts, CIRCLE_VERTS_COUNT, colour);
}

//======================================================================================================================
void CollideSys::DebugDraw() {
    
    m_debugDrawBoundsGroups = BuildCollisionMaskFromString( col_dbgDrawShapesGroups.GetString() );
    m_debugDrawCelGroups = BuildCollisionMaskFromString( col_dbgDrawCellGroups.GetString() );
    m_debugDrawPairs = BuildCollisionMaskFromString( col_dbgDrawPairGroups.GetString() );
    
    for(Collider::ListNode* shapeNode = m_shapeList.Next();
        shapeNode->IsRoot() == false;
        shapeNode = shapeNode->Next()) {
        
        Collider* obj = *shapeNode;
        int32_t colIndex = (int32_t) obj->m_group % (int32_t) DEBUG_COLOUR_COUNT;
        
        Shape* shape = obj->GetShape();
        
        if ( col_dbgDraw.GetBool() == true && (obj->m_group & m_debugDrawBoundsGroups) != 0) {
        
            switch(shape->m_type) {
                case Shape::TYPE_CIRCLE: {
                    ShapeCircle * circle = (ShapeCircle*) obj->GetShape();
                    DrawDebugCircle(obj->m_pos, std::sqrtf(circle->GetRadiusSqr()), m_debugColours[colIndex]);
                    break;
                }
                    
                case Shape::TYPE_AABOX: {
                    ShapeBox* box = (ShapeBox*) shape;
                    DrawAxisBox(box->GetMinWorld(), box->GetMaxWorld(), m_debugColours[colIndex]);
                    break;
                }
                    
                default:
                    break;
            }
        }
        
        if ( col_dbgDrawCells.GetBool() == true ) {
            if (((obj->m_group & m_debugDrawCelGroups) != 0) && (obj->m_cell != nullptr)) {
                DrawCell(obj->m_cell);
            }
        }
    }
    
    if ( col_dbgDrawPairs.GetBool() == true ) {
        
        for ( auto i : m_pairs ) {
            
            Pair & p = i.second;
            bool drawPair = ((p.m_shape0->m_group && m_debugDrawPairs) ||
                             (p.m_shape1->m_group && m_debugDrawPairs));
            
            if (drawPair == true) {
                DrawPair( &p );
            }
        }
    }
}

//======================================================================================================================
void CollideSys::DrawCell(Cell* cell) {
    i3d::Vector3 verts[4];
    float x0 = cell->m_boundsMin.X();
    float y0 = cell->m_boundsMin.Y();
    float x1 = cell->m_boundsMax.X();
    float y1 = cell->m_boundsMax.Y();

    
    verts[0].Set(x0, 0, y1 );
    verts[1].Set(x1, 0, y1 );
    verts[2].Set(x1, 0, y0 );
    verts[3].Set(x0, 0, y0 );
    
    render->DrawClosedLineList(verts, 4, i3d::Vector4(0, 1, 0, 1));
}

//======================================================================================================================
void CollideSys::DrawAxisBox(const i3d::Vector2& bmin, const i3d::Vector2& bmax, const i3d::Vector4& colour) {
    i3d::Vector3 verts[4];
    float x0 = bmin.X();
    float y0 = bmin.Y();
    float x1 = bmax.X();
    float y1 = bmax.Y();

    
    verts[0].Set(x0, 0, y1 );
    verts[1].Set(x1, 0, y1 );
    verts[2].Set(x1, 0, y0 );
    verts[3].Set(x0, 0, y0 );
    
    render->DrawClosedLineList(verts, 4, i3d::Vector4(0, 1, 0, 1));
}

//======================================================================================================================
void CollideSys::DrawPair(Pair* pair) {
    i3d::Vector2 bmin, bmax;
    pair->GetBounds(bmin, bmax);
    
    i3d::Vector3 verts[4];
    float x0 = bmin.X();
    float y0 = bmin.Y();
    float x1 = bmax.X();
    float y1 = bmax.Y();

    
    verts[0].Set(x0, 0, y1 );
    verts[1].Set(x1, 0, y1 );
    verts[2].Set(x1, 0, y0 );
    verts[3].Set(x0, 0, y0 );
    
    render->DrawClosedLineList(verts, 4, i3d::Vector4(1, 0, 1, 1));
}
