//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __COLLIDE_OVERLAPTEST_H__
#define __COLLIDE_OVERLAPTEST_H__

class Shape;

struct OverlapResult {
    bool                m_overlap;
    float               m_distance;
    i3d::Vector2        m_axis;
};

void TestShape(OverlapResult& result, Shape* shape0, Shape* shape1);

#endif
