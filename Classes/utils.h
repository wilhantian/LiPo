//
//  utils.h
//  LiPo
//
//  Created by WilhanTian on 2016/11/4.
//
//

#ifndef utils_h
#define utils_h

#include "cocos2d.h"

USING_NS_CC;

class utils {
public:
    static Texture2D* getRenderFrame(std::string frame)
    {
        auto cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile("animate.plist");
        
        return NULL;
    }
};

#endif /* utils_h */
