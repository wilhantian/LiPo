//
//  enums.h
//  LiPo
//
//  Created by WilhanTian on 2016/11/1.
//
//

#ifndef enums_h
#define enums_h

enum CollisionType
{
    Null = 0,//0x00000
    Hero = 1<<1,//0x00001
    Monster = 1<<2,//0x00010
    Wall = 1<<3//0x00100
};

/// usge: var types = CollisionType.Hero | CollisionType.Wall
///       if(types & CollisionType.Hero) --> true
///       if(types & CollisionType.Monster) --> false
///       if(types & CollisionType.Wall) --> true


#endif /* enums_h */
