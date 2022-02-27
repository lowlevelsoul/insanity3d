//
//  TextureArguments.h
//  r3d
//
//  Created by James Steele on 26.09.21.
//

#ifndef TextureArguments_h
#define TextureArguments_h

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

#define MAX_ARGUMENT_TEXTUES 5000000

typedef struct {
    array<texture2d<float>, MAX_ARGUMENT_TEXTUES> textures  [[ id(0) ]];
} TextureArguments;



#endif /* TextureArguments_h */
