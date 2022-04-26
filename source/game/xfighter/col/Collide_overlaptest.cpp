//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "col/ColShape.h"
#include "col/Collide_overlaptest.h"
#include "col/ColShapeCircle.h"
#include "col/ColShapeBox.h"
#include "col/Collider.h"

typedef void (*TestFunc)(OverlapResult& result, Shape* shape0, Shape* shape1);

void TestCircle_Circle(OverlapResult& result, Shape* shape0, Shape* shape1);
void TestAabox_Aabox(OverlapResult& result, Shape* shape0, Shape* shape1);

void TestCircle_Aabox(OverlapResult& result, Shape* shape0, Shape* shape1);
void TestAabox_Circle(OverlapResult& result, Shape* shape0, Shape* shape1);

static const TestFunc TEST_FUNC_TABLE[Shape::TYPE_COUNT][Shape::TYPE_COUNT] = {
    // NONE             TYPE_CIRCLE             TYPE_AABOX
    {nullptr,           nullptr,                nullptr},
    {nullptr,           TestCircle_Circle,      TestCircle_Aabox},
    {nullptr,           TestAabox_Circle,       TestAabox_Aabox},
};

//======================================================================================================================
void TestShape(OverlapResult& result, Shape* shape0, Shape* shape1) {
    TestFunc func = TEST_FUNC_TABLE[shape0->m_type][shape1->m_type];
    if (func != nullptr) {
        func(result, shape0, shape1);
    }
}

//======================================================================================================================
void TestCircle_Circle(OverlapResult& result, Shape* shape0, Shape* shape1) {
    
    ShapeCircle * circle0 = (ShapeCircle*) shape0;
    ShapeCircle * circle1 = (ShapeCircle*) shape1;
    
    float overlapDist = circle0->GetRadiusSqr() +  circle1->GetRadiusSqr();
    const i3d::Vector2 & c0 = circle0->GetCenterWorld();
    const i3d::Vector2 & c1 = circle1->GetCenterWorld();
    
    i3d::Vector2 sepDir = c1 - c0;
    float sepDistSqr = sepDir.Dot(sepDir);
    
    result.m_overlap = (sepDistSqr <= overlapDist);
}

//======================================================================================================================
void TestCircle_Aabox(OverlapResult& result, Shape* shape0, Shape* shape1) {
    
    ShapeCircle * circle = (ShapeCircle*) shape0;
    ShapeBox * box = (ShapeBox*) shape1;
    
    const i3d::Vector2 & bmin = box->GetMinWorld();
    const i3d::Vector2 & bmax = box->GetMaxWorld();
    const i3d::Vector2 & c = circle->GetCenterWorld();
    
    
    float closestX = (c.X() < bmin.X() ? bmin.X() : (c.X() > bmax.X() ? bmax.X() : c.X() ) );
    float closestY = (c.Y() < bmin.Y() ? bmin.Y() : (c.Y() > bmax.Y() ? bmax.Y() : c.Y() ) );
    i3d::Vector2 delta = i3d::Vector2(closestX, closestY) - c;
    float d = delta.Dot(delta);
    
    if ( d < circle->GetRadiusSqr() ) {
        result.m_overlap = true;
    }
    else {
        result.m_overlap = false;
    }
}

//======================================================================================================================
void TestAabox_Circle(OverlapResult& result, Shape* shape0, Shape* shape1) {
    TestCircle_Aabox(result, shape1, shape0);
}

//======================================================================================================================
void TestAabox_Aabox(OverlapResult& result, Shape* shape0, Shape* shape1) {
    
    ShapeBox * box0 = (ShapeBox *) shape0;
    ShapeBox * box1 = (ShapeBox *) shape1;
    
    const i3d::Vector2 & bmin0 = box0->GetMinWorld();
    const i3d::Vector2 & bmax0 = box0->GetMaxWorld();
    const i3d::Vector2 & bmin1 = box1->GetMinWorld();
    const i3d::Vector2 & bmax1 = box1->GetMinWorld();
    
    if (bmin0.X() > bmax1.X() || bmax0.X() < bmin1.X() ||
        bmin0.Y() > bmax1.Y() || bmax0.Y() < bmin1.Y()) {
        result.m_overlap = false;
    }
    else {
        result.m_overlap = true;
    }
}
