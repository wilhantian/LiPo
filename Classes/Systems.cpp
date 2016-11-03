#include "Systems.h"
#include <ctime>

#include "enums.h"

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
/// Health
//////////////////////////////////////////////////////
void HealthSystem::tick(float dt)
{
    auto all = Entity::getAll<HealthCom>();
    for(Eid id : all)
    {
        Ent e(id);
        auto& health = e.health;
        
        if(health.empty()) continue;
        
        if(health.hp <= 0)//is death
        {
            Entity::destroyNow(id);
        }
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
bool InputSystem::attackKeyDown = false;

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
			int DEBUG_TAG = id;
			Sprite* debugNode = (Sprite*)render.sprite->getParent()->getChildByTag(DEBUG_TAG);
			if (debugNode == nullptr)
			{
				debugNode = Sprite::create("texture.png");
				debugNode->setAnchorPoint(Vec2(0.5, 0));
				debugNode->setTag(DEBUG_TAG);
				debugNode->setTextureRect(Rect(0, 0, collision.size.width, collision.size.height));
				debugNode->setColor(Color3B::YELLOW);
				debugNode->setOpacity(50);
				render.sprite->getParent()->addChild(debugNode);
			}
			debugNode->setPosition(position.pos + collision.offset);
		}
        
        /// check collision
        Eid collId = getCollision(e);
        if(collId == -1)
        {
            continue;
        }
        
        collisionHandler(id, collId);
	}
}

Eid CollisionSystem::getCollision(Ent _e)
{
    auto& _collision = _e.collision;
    auto& _position = _e.position;
    
    if(_collision.empty()) return -1;
    if(_position.empty()) return -1;
    
	Rect _rect(_position.pos + _collision.offset - Vec2(_collision.size.width/2, 0), _collision.size);
    
    auto all = Entity::getAll<CollisionCom>();
    for(Eid id : all)
    {
        Ent e(id);
        auto& collision = e.collision;
        auto& position = e.position;
        
        if(id == _e.id) continue;
        if(collision.empty()) continue;
        if(position.empty()) continue;
        if(!isCanCollision(_collision.mask, collision.type)) continue;
        
		Rect rect(position.pos + collision.offset - Vec2(collision.size.width / 2, 0), collision.size);
        if(_rect.intersectsRect(rect))
        {
            return id;
        }
    }
    return -1;
}

std::vector<Eid> CollisionSystem::getCollision(unsigned mask, Rect rect)
{
    std::vector<Eid> ids;
    
    auto all = Entity::getAll<CollisionCom>();
    for (Eid id : all)
    {
        Ent e(id);
        auto& collision = e.collision;
        auto& position = e.position;
        
        if(collision.empty()) continue;
        if(position.empty()) continue;
        
        if(!isCanCollision(mask, collision.type)) continue;
        
        Rect eRect(position.pos + collision.offset - Vec2(collision.size.width / 2, 0), collision.size);
        if(rect.intersectsRect(eRect))
        {
            ids.push_back(id);
        }
    }
    
    return ids;
}

void CollisionSystem::collisionHandler(Eid idA, Eid idB)
{
    Ent entityA(idA);
    Ent entityB(idB);
    
    auto& collisionA = entityA.collision;
    auto& collisionB = entityB.collision;
    
    /// 英雄与墙发生碰撞
    if((collisionA.type == CollisionType::Hero
		|| collisionA.type == CollisionType::Monster)
		&& collisionB.type == CollisionType::Wall)
    {
        log("英雄发生墙体碰撞 %d %d", idA, idB);
        entityA.move.speed.setZero();
        entityA.position.pos = entityA.position.lastPost;
    }
}

bool CollisionSystem::isCanCollision(unsigned int selfMast, unsigned int otherType)
{
    return selfMast & otherType;
}

//////////////////////////////////////////////////////
/// Melee
//////////////////////////////////////////////////////
void MeleeSystem::tick(float dt)
{
	auto all = Entity::getAll<MeleeCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& melee = e.melee;
        auto& position = e.position;
        
        if(melee.empty()) continue;
        if(position.empty()) continue;
        if(!InputSystem::attackKeyDown) continue;
        
		/// check time
		clock_t timep = clock();
		if (timep - melee.lastAttTime < 10000) continue;
		else melee.lastAttTime = timep;

        Size size(30, 40);
        Rect rect(position.pos - Vec2(size.width/2, 0), size);
        
        std::vector<Eid> ids = CollisionSystem::getCollision(CollisionType::Monster | CollisionType::Wall, rect);
        for(Eid collisionEid : ids)
        {
            if(id == collisionEid) continue;
            
        }
        
        auto& render = e.render;
        if(render.full())
        {
            auto debugSrpite = Sprite::create("texture.png");
            debugSrpite->setPosition(position.pos);
            debugSrpite->setAnchorPoint(Vec2(0.5, 0));
            debugSrpite->setTextureRect(Rect(Vec2::ZERO, size));
            render.sprite->getParent()->addChild(debugSrpite);
            
            debugSrpite->scheduleOnce([=](float dt)
            {
                debugSrpite->removeFromParent();
            }, 0.5f, "debugSrpite");
        }
	}
}
