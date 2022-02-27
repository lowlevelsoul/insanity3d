//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#import <Cocoa/Cocoa.h>

namespace i3d {
    void EngineInitialise( int argc, const char ** argv );
    void EngineFinalise();
}

//======================================================================================================================
int main(int argc, const char * argv[] ) {
    
    i3d::EngineInitialise( argc, argv );
    
    int retCode = NSApplicationMain(argc, argv);
    
    i3d::EngineFinalise();
    
    return retCode;
}
