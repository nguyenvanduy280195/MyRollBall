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
#include "Infos/GameInfo.h"
#include "ScreenLog/ScreenLog.h"
#include "IntroLevelScene.h"
#include "base/ccUTF8.h"
#include "2d/CCTransition.h"

using Vec2 = cocos2d::Vec2;
using KeyCode = cocos2d::EventKeyboard::KeyCode;


cocos2d::Scene* InGameScene::CreateScene()
{
	return CreateScene(1);
}

cocos2d::Scene* InGameScene::CreateScene(int level)
{
	auto scene = cocos2d::Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
	auto inGame = Cocos2dCreator::CreateNode<InGameScene>(level);
	scene->addChild(inGame);
	return scene;
}


InGameScene::~InGameScene()
{
	_handlerManager->release();
	_gameInfo->release();
}

bool InGameScene::init(int level)
{
	if (!Super::init())
	{
		return false;
	}
	_currentLevel = level;
	
	_handlerManager = new(std::nothrow) HandlerManager(this);

	_gameInfo = new(std::nothrow) GameInfo();

	// screen log
	_screenLog = Cocos2dCreator::CreateNode<ScreenLog>(_gameInfo);
	addChild(_screenLog, INT32_MAX);

	// level
	const auto path = cocos2d::StringUtils::format(FORMAT_LEVEL.c_str(), _currentLevel);
	_level = Cocos2dCreator::CreateNode<Level>(path);
	addChild(_level);

	auto startPosition = _level->GetStartPosition();

	// player
	_player = Cocos2dCreator::CreateNode<Player>(this, startPosition);
	addChild(_player);

	return true;
}

void InGameScene::onEnterTransitionDidFinish()
{
	Super::onEnterTransitionDidFinish();
	scheduleUpdate();
}

void InGameScene::update(float)
{
	UpdateCameraPosition();
}

void InGameScene::UpdateCameraPosition()
{
	auto& playerPosition = _player->getPosition();
	auto& viewport = cocos2d::Camera::getDefaultViewport();

//	if (0.5f * viewport.w <= playerPosition.x && playerPosition.x <= _level->getContentSize().width - 0.5f * viewport.w)
//	{
//		cocos2d::Camera::getDefaultCamera()->setPositionX(_player->getPositionX());
//	}
//	if (0.5f * viewport.h <= playerPosition.y && playerPosition.y <= _level->getContentSize().height - 0.5f * viewport.h)
//	{
//		cocos2d::Camera::getDefaultCamera()->setPositionY(_player->getPositionY());
//	}

	cocos2d::Camera::getDefaultCamera()->setPosition(_player->getPosition());
}

void InGameScene::NextLevel()
{
	unscheduleUpdate();
	auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel + 1);
	auto sceneWithTransition = cocos2d::TransitionMoveInR::create(1, scene);
	cocos2d::Director::getInstance()->replaceScene(sceneWithTransition);
}