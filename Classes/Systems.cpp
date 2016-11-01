#include "Systems.h"

//////////////////////////////////////////////////////
/// Render
//////////////////////////////////////////////////////
void RenderSystem::tick(float dt)
{
	auto all = Entity::getAll<RenderCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& render = e.render;
		auto& position = e.position;

		if (render.empty()) continue;
		if (position.empty()) continue;

		render.sprite->setPosition(position.pos);
	}
}

//////////////////////////////////////////////////////
/// Move
//////////////////////////////////////////////////////
void MoveSystem::tick(float dt)
{
	auto all = Entity::getAll<MoveCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& move = e.move;
		auto& position = e.position;

		if (move.empty()) continue;
		if (position.empty()) continue;

		position.lastPost.set(position.pos);
		position.pos.add(move.speed * dt);
	}
}

//////////////////////////////////////////////////////
/// Input
//////////////////////////////////////////////////////
void InputSystem::tick(float dt)
{
	auto all = Entity::getAll<InputCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& input = e.input;
		auto& move = e.move;

		if (input.empty()) continue;

		int speed = 80;

		if (InputSystem::upKeyDown) move.speed.y = speed;
		else if (InputSystem::downKeyDown) move.speed.y = -speed;
		else move.speed.y = 0;

		if (InputSystem::leftKeyDown) move.speed.x = -speed;
		else if (InputSystem::rightKeyDown) move.speed.x = speed;
		else move.speed.x = 0;
	}
}

bool InputSystem::rightKeyDown = false;
bool InputSystem::leftKeyDown = false;
bool InputSystem::downKeyDown = false;
bool InputSystem::upKeyDown = false;

//////////////////////////////////////////////////////
/// Collision
//////////////////////////////////////////////////////
void CollisionSystem::tick(float dt)
{
	auto all = Entity::getAll<CollisionCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& collision = e.collision;
		auto& position = e.position;
		auto& render = e.render;

		if (collision.empty()) continue;
		if (position.empty()) continue;

		if (render.full())//debug draw
		{
			static int DEBUG_TAG = 0x0025;
			if (render.sprite->getChildByTag(DEBUG_TAG) == nullptr)
			{
				Sprite* debugNode = Sprite::create("texture.png");
				debugNode->setPosition(collision.offset + Vec2(render.sprite->getContentSize().width/2, 0));
				debugNode->setAnchorPoint(Vec2(0.5, 0));
				debugNode->setTag(DEBUG_TAG);
				debugNode->setTextureRect(Rect(0, 0, collision.size.width, collision.size.height));
				debugNode->setColor(Color3B::WHITE);
				debugNode->setOpacity(50);
				render.sprite->addChild(debugNode);
			}
		}
        
        /// check collision
        Eid collId = getCollisionEntity(id);
        if(collId == id)
        {
            continue;
        }
        
        collisionHandler(id, collId);
	}
}

Eid CollisionSystem::getCollisionEntity(Eid _id)
{
    Ent _e(_id);
    auto& _collision = _e.collision;
    auto& _position = _e.position;
    
    if(_collision.empty()) return _id;
    if(_position.empty()) return _id;
    
    Rect _rect(_position.pos + _collision.offset, _collision.size);
    
    auto all = Entity::getAll<CollisionCom>();
    for(Eid id : all)
    {
        Ent e(id);
        auto& collision = e.collision;
        auto& position = e.position;
        
        if(id == _id) continue;
        if(collision.empty()) continue;
        if(position.empty()) continue;
        
        Rect rect(position.pos + collision.offset, collision.size);
        if(_rect.intersectsRect(rect))
        {
            return id;
        }
    }
    return _id;
}

void CollisionSystem::collisionHandler(Eid idA, Eid idB)
{
    Ent entityA(idA);
    Ent entityB(idB);
    
    log("发生碰撞 %d %d", idA, idB);
}
