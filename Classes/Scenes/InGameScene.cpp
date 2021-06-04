#include "InGameScene.h"
#include "Player.h"
#include "base/CCDirector.h"
#include "Utils/Cocos2dCreator.h"
#include "2d/CCScene.h"
#include "physics/CCPhysicsWorld.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "Level.h"
#include "2d/CCCamera.h"
#include "Managers/HandlerManager.h"
#include "Managers/KeyboardHandler.h"
#include "physics/CCPhysicsContact.h"
#include "CCEventListenerAcceleration.h"
#include "2d/CCFastTMXTiledMap.h"
#include "platform/CCDevice.h"

using Vec2 = cocos2d::Vec2;
using KeyCode = cocos2d::EventKeyboard::KeyCode;


cocos2d::Scene* InGameScene::CreateScene()
{
	auto scene = cocos2d::Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
	auto inGame = Cocos2dCreator::CreateNode<InGameScene>();
	scene->addChild(inGame);
	return scene;
}


InGameScene::~InGameScene()
{
	_handlerManager->release();
}

bool InGameScene::init()
{
	if (!Super::init())
	{
		return false;
	}

	_handlerManager = new(std::nothrow) HandlerManager(this);

	// level
	_level = Cocos2dCreator::CreateNode<Level>("levels/001.tmx");
	addChild(_level);

	auto startPosition = _level->GetStartPosition();
	//auto startPosition = Vec2(100, 100);

	// player
	_player = Cocos2dCreator::CreateNode<Player>(startPosition, _handlerManager);
	addChild(_player);

	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](cocos2d::PhysicsContact& contact) -> bool
	{
		auto nodeA = contact.getShapeA()->getBody()->getNode();
		auto nodeB = contact.getShapeB()->getBody()->getNode();

		auto tagA = contact.getShapeA()->getTag();
		auto tagB = contact.getShapeB()->getTag();

		if (tagA == _level->nameTagMap["player"] ||
			tagB == _level->nameTagMap["wall-bottom"])
		{
			auto player = (Player*)nodeA;
			player->SetOnGround(true);
		}
		else if (tagA == _level->nameTagMap["wall-bottom"] ||
				 tagB == _level->nameTagMap["player"])
		{
			auto player = (Player*)nodeB;
			player->SetOnGround(true);
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();
	return true;
}

void InGameScene::update(float)
{
	auto& playerPosition = _player->getPosition();
	auto& viewport = cocos2d::Camera::getDefaultViewport();

	//if (playerPosition.x > 0.5f * viewport.w && playerPosition.y < _level->getContentSize().width - 0.5f * viewport.w)
	//{
	//	cocos2d::Camera::getDefaultCamera()->setPosition(_player->getPosition());
	//}
	cocos2d::Camera::getDefaultCamera()->setPosition(_player->getPosition());
}

