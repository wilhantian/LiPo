#ifndef _SYSTEMS_H_
#define _SYSTEMS_H_

#include "cocos2d.h"
#include "EntityFu.h"
#include "Components.h"

USING_NS_CC;

//////////////////////////////////////////////////////
/// Render
//////////////////////////////////////////////////////
class RenderSystem : System
{
public:
	static void tick(float dt);
    
private:
    RenderSystem(){}
    ~RenderSystem(){}
};

//////////////////////////////////////////////////////
/// Move
//////////////////////////////////////////////////////
class MoveSystem : System
{
public:
	static void tick(float dt);
    
private:
    MoveSystem(){}
    ~MoveSystem(){}
};

//////////////////////////////////////////////////////
/// Input
//////////////////////////////////////////////////////
class InputSystem : System
{
public:
	static void tick(float dt);
    
public:
	static bool upKeyDown;
	static bool downKeyDown;
	static bool leftKeyDown;
	static bool rightKeyDown;
	static bool attackKeyDown;

private:
    InputSystem(){}
    ~InputSystem(){}
};

//////////////////////////////////////////////////////
/// Collision
//////////////////////////////////////////////////////
class CollisionSystem : System
{
public:
	static void tick(float dt);
    
private:
    static Eid getCollisionEntity(Eid id);
    static void collisionHandler(Eid idA, Eid idB);
    static bool isCanCollision(unsigned selfMast, unsigned otherType);
    
private:
    CollisionSystem(){}
    ~CollisionSystem(){}
};

//////////////////////////////////////////////////////
/// Melee
//////////////////////////////////////////////////////
class MeleeSystem : System
{
public:
	static void tick(float dt);
};

#endif //_SYSTEMS_H_
