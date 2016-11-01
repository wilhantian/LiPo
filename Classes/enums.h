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
    Null = 0,
    Hero = 1<<1,
    Monster = 1<<2,
    Wall = 1<<3,
	Sword = 1 << 4
};

/// usge: var types = CollisionType.Hero | CollisionType.Wall
///       if(types & CollisionType.Hero) --> true
///       if(types & CollisionType.Monster) --> false
///       if(types & CollisionType.Wall) --> true


#endif /* enums_h */
