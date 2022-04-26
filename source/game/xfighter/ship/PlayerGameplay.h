//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __PLAYERGAMEPLAY_H__
#define __PLAYERGAMEPLAY_H__

#include "ship/Ship.h"
#include "ship/ShipDef.h"

class ColShape;
class Collider;
class Projectile;
struct OverlapResult;

class PlayerGameplay : ShipComponent {
public:
    RTTI_CLASS_DECLARE( PlayerGameplay, ShipComponent )
    
    static void HitCallback( const OverlapResult & res, Collider * thisObj, Collider * otherObj, void * context );
    
    PlayerGameplay();
    
    virtual ~PlayerGameplay();
    
    virtual void Construct( ShipComponentDef * def ) override;
    
    virtual void Think( float timeStep ) override;
    
    void HandleEnemyBullet( Projectile * projectile );
};

#endif
