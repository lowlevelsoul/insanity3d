//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================


#import <UIKit/UIKit.h>
#import "AppDelegate.h"

namespace i3d {
    void EngineInitialise( int argc, const char ** argv );
    void EngineFinalise();
}

//======================================================================================================================
int main(int argc, char * argv[]) {
    
    NSString * appDelegateClassName;    
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    
    i3d::EngineInitialise( argc, argv );
    
    int retCode = UIApplicationMain(argc, argv, nil, appDelegateClassName);
    
    i3d::EngineFinalise();
    
    return retCode;
}
