#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include "cocos2d.h"
#include "EntityFu.h"

USING_NS_CC;

//////////////////////////////////////////////////////
/// Render
//////////////////////////////////////////////////////
struct RenderCom : Entity::Component
{
	static Cid cid;

	Sprite* sprite;

	RenderCom()
		: sprite(nullptr)
	{}
	
	RenderCom(std::string filename, Node* node)
		: RenderCom()
	{
		sprite = Sprite::create(filename);
		sprite->setAnchorPoint(Vec2(0.5, 0));
		node->addChild(sprite);
	}

	RenderCom(Size debugSize, Node* node)
		:RenderCom()
	{
		sprite = Sprite::create("texture.png");
		sprite->setAnchorPoint(Vec2(0.5, 0));
		sprite->setTextureRect(Rect(0, 0, debugSize.width, debugSize.height));
		sprite->setColor(Color3B::RED);
		sprite->setOpacity(125);
		node->addChild(sprite);
	}

	virtual bool empty() const
	{
		return sprite == nullptr || sprite->getParent() == nullptr;
	}
};

//////////////////////////////////////////////////////
/// Position
//////////////////////////////////////////////////////
struct PositionCom : Entity::Component
{
	static Cid cid;

	Vec2 pos;
	Vec2 lastPost;

	PositionCom(){}

	PositionCom(Vec2 _pos)
		: PositionCom()
	{
		pos.set(_pos);
	}

	virtual bool empty() const
	{
		return false;
	}
};

//////////////////////////////////////////////////////
/// Move
//////////////////////////////////////////////////////
struct MoveCom : Entity::Component
{
	static Cid cid;

	Vec2 speed;

	MoveCom(){}

	MoveCom(Vec2 _speed)
		: MoveCom()
	{
		speed = _speed;
	}

	virtual bool empty() const
	{
		return speed == Vec2::ZERO;
	}
};

//////////////////////////////////////////////////////
/// Collision
//////////////////////////////////////////////////////
struct CollisionCom : Entity::Component
{
	static Cid cid;
    
    unsigned type;//collision type 自己的碰撞类型
    unsigned mask;//filter mask 能跟哪些人发生碰撞
    
	Vec2 offset;
	Size size;

	CollisionCom(){}

	CollisionCom(unsigned _type, unsigned _mask, Size _size, Vec2 _offset)
		: CollisionCom()
	{
        type = _type;
        mask = _mask;
		size.setSize(_size.width, _size.height);
		offset.set(_offset);
	}

	virtual bool empty() const
	{
		return size.equals(Size::ZERO);
	}
};

//////////////////////////////////////////////////////
/// Input
//////////////////////////////////////////////////////
struct InputCom : Entity::Component
{
	static Cid cid;

	InputCom(){}

	virtual bool empty() const
	{
		return false;
	}
};

//////////////////////////////////////////////////////
/// please register in .cpp
//////////////////////////////////////////////////////
struct System::Ent
{
	Eid id;

	// BEGIN
	RenderCom& render;
	PositionCom& position;
	MoveCom& move;
	CollisionCom& collision;
	InputCom& input;
	// END

	Ent(Eid _id);
};

#endif //_COMPONENTS_H_
