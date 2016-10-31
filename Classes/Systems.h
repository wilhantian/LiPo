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
	static void tick(double dt);
};

//////////////////////////////////////////////////////
/// Move
//////////////////////////////////////////////////////
class MoveSystem : System
{
public:
	static void tick(double dt);
};

//////////////////////////////////////////////////////
/// Input
//////////////////////////////////////////////////////
class InputSystem : System
{
public:
	static void tick(double dt);
public:
	static boolean upKeyDown;
	static boolean downKeyDown;
	static boolean leftKeyDown;
	static boolean rightKeyDown;
};

//////////////////////////////////////////////////////
/// Collision
//////////////////////////////////////////////////////
class CollisionSystem : System
{
public:
	static void tick(double dt);
};