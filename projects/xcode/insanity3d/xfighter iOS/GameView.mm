//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#import "GameView.h"
#include "i3d/input/Input.h"
#include "i3d/input/Input_ios.h"

@implementation GameView

//======================================================================================================================
-(BOOL) acceptsFirstResponder {
    return YES;
}


//======================================================================================================================
-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
    NSArray<UITouch *>* touchArray = [touches allObjects];
    for(uint32_t i = 0; i < [touches count]; ++i) {
        inputLocal->OnTouchBegin([touchArray objectAtIndex:i]);
    }
    
}

//======================================================================================================================
-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    NSArray<UITouch *>* touchArray = [touches allObjects];
    for(uint32_t i = 0; i < [touches count]; ++i) {
        inputLocal->OnTouchEnd([touchArray objectAtIndex:i]);
    }
}

//======================================================================================================================
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
    NSArray<UITouch *>* touchArray = [touches allObjects];
    for(uint32_t i = 0; i < [touches count]; ++i) {
        inputLocal->OnTouchMove([touchArray objectAtIndex:i]);
    }
}

@end
