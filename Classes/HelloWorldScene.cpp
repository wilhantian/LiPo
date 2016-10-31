#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Components.h"
#include "Systems.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	/// Hero
	Eid e = Entity::create();
	Entity::addComponent(e, new RenderCom(Size(40, 70), this));
	Entity::addComponent(e, new PositionCom());
	Entity::addComponent(e, new MoveCom(Vec2(60, 60)));
	Entity::addComponent(e, new InputCom());
	Entity::addComponent(e, new CollisionCom(Size(50, 20), Vec2(0, 0)));

	/// Monster
	e = Entity::create();
	Entity::addComponent(e, new RenderCom(Size(40, 40), this));
	Entity::addComponent(e, new PositionCom(Vec2(240, 200)));
	Entity::addComponent(e, new MoveCom());
	Entity::addComponent(e, new CollisionCom(Size(50, 20), Vec2(0, 0)));

	this->scheduleUpdate();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* eve)
	{
		if (EventKeyboard::KeyCode::KEY_W == code) InputSystem::upKeyDown = true;
		if (EventKeyboard::KeyCode::KEY_S == code) InputSystem::downKeyDown = true;
		if (EventKeyboard::KeyCode::KEY_A == code) InputSystem::leftKeyDown = true;
		if (EventKeyboard::KeyCode::KEY_D == code) InputSystem::rightKeyDown = true;
	};
	listener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* eve)
	{
		if (EventKeyboard::KeyCode::KEY_W == code) InputSystem::upKeyDown = false;
		if (EventKeyboard::KeyCode::KEY_S == code) InputSystem::downKeyDown = false;
		if (EventKeyboard::KeyCode::KEY_A == code) InputSystem::leftKeyDown = false;
		if (EventKeyboard::KeyCode::KEY_D == code) InputSystem::rightKeyDown = false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void HelloWorld::update(float dt)
{
	InputSystem::tick(dt);
	CollisionSystem::tick(dt);
	MoveSystem::tick(dt);
	RenderSystem::tick(dt);
}
