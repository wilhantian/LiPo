#include "Systems.h"

//////////////////////////////////////////////////////
/// Render
//////////////////////////////////////////////////////
void RenderSystem::tick(double dt)
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
void MoveSystem::tick(double dt)
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
void InputSystem::tick(double dt)
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

boolean InputSystem::rightKeyDown = false;
boolean InputSystem::leftKeyDown = false;
boolean InputSystem::downKeyDown = false;
boolean InputSystem::upKeyDown = false;

//////////////////////////////////////////////////////
/// Collision
//////////////////////////////////////////////////////
void CollisionSystem::tick(double dt)
{
	auto all = Entity::getAll<CollisionCom>();
	for (Eid id : all)
	{
		Ent e(id);
		auto& collision = e.collision;
		auto& position = e.position;
		auto& move = e.move;
		auto& render = e.render;

		if (collision.empty()) continue;
		if (position.empty()) continue;
		if (move.empty()) continue;

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

		/// check
		Rect rect = Rect(position.pos + collision.offset, collision.size);
		auto others = Entity::getAll<CollisionCom>();
		for (Eid oId : others)
		{
			if (oId == id) continue;

			Ent oE(oId);
			auto& oCollision = oE.collision;
			auto& oPosition = oE.position;

			if (oCollision.empty()) continue;
			if (oPosition.empty()) continue;

			Rect oRect = Rect(oPosition.pos + oCollision.offset, oCollision.size);
			if (rect.intersectsRect(oRect))
			{
				/// TODO
				position.pos = position.lastPost;
				move.speed.setZero();
			}
		}
	}
}
