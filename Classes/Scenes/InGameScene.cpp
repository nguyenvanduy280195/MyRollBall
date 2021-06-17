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
#include "IntroLevelScene.h"
#include "base/ccUTF8.h"
#include "2d/CCTransition.h"

#include "Dialogs/VictoryDialog.h"

#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCAction.h"
#include "ScreenLog/ScreenLog.h"

#include "MyCustomGUI.inl"

using Vec2 = cocos2d::Vec2;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

cocos2d::Scene *InGameScene::CreateScene()
{
	return CreateScene(1);
}

cocos2d::Scene *InGameScene::CreateScene(int level)
{
	auto scene = cocos2d::Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
	auto inGame = Cocos2dCreator::CreateNode<InGameScene>(level);
	inGame->setName("InGameScene");
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

	_handlerManager = new (std::nothrow) HandlerManager(this);

	_gameInfo = new (std::nothrow) GameInfo();

	// level
	const auto path = cocos2d::StringUtils::format(FORMAT_LEVEL.c_str(), _currentLevel);
	_level = Cocos2dCreator::CreateNode<Level>(path);
	addChild(_level);

	auto startPosition = _level->GetStartPosition();

	// player
	_player = Cocos2dCreator::CreateNode<Player>(this, startPosition);
	addChild(_player);

	ScreenLog::GetInstance()->AttachToScene(this);
	ScreenLog::GetInstance()->Debug(__FUNCTION__);

	return true;
}

void InGameScene::onEnterTransitionDidFinish()
{
	Super::onEnterTransitionDidFinish();

	//_eventDispatcher->pauseEventListenersForTarget(this);
	//auto targetPosition = GetVectorToPlayer();
	//auto movingCameraToPlayer = cocos2d::MoveTo::create(2, targetPosition);
	//auto startGame = cocos2d::CallFunc::create([this]()
	//{
	//	_eventDispatcher->resumeEventListenersForTarget(this);
	//	scheduleUpdate();
	//	TakeCameraAfterPlayer();
	//});
	//auto sequence = cocos2d::Sequence::createWithTwoActions(movingCameraToPlayer, startGame);
	//runAction(sequence);

	scheduleUpdate();
	TakeCameraAfterPlayer();
}

void InGameScene::update(float)
{
	auto originalTargetPosition = _followingPlayer->getOriginalTarget()->getPosition();
	ScreenLog::GetInstance()->Debug("%f %f", originalTargetPosition.x, originalTargetPosition.y);
	//ScreenLog::GetInstance()->SetPosition(originalTargetPosition);
}

void InGameScene::TakeCameraAfterPlayer()
{
	const auto &levelPosition = _level->getPosition();
	const auto &levelContentSize = _level->getContentSize();
	const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	const auto rect = cocos2d::Rect(levelPosition - visibleOrigin, cocos2d::Size(levelContentSize.width + 2 * visibleOrigin.x, levelContentSize.height + 2 * visibleOrigin.y));
	_followingPlayer = cocos2d::Follow::create(_player, rect);
	runAction(_followingPlayer);
}

void InGameScene::Victory()
{

	ScreenLog::GetInstance()->AttachToScene(this);
	ScreenLog::GetInstance()->Debug(__FUNCTION__);

	StopGame();
	ShowVictoryDialog();
}

cocos2d::Vec2 InGameScene::GetVectorToPlayer() const
{
	const auto &winSize = cocos2d::Director::getInstance()->getWinSize();
	auto &playerPosition = _player->getPosition();
	Vec2 position = 0.5f * winSize - playerPosition;

	const auto &levelPosition = _level->getPosition();
	const auto &levelSize = _level->getContentSize();

	const auto cameraXMin = -((levelPosition.x + levelSize.width) - winSize.width);
	const auto cameraXMax = -levelPosition.x;
	const auto cameraYMin = -((levelPosition.y + levelSize.height) - winSize.height);
	const auto cameraYMax = -levelPosition.y;

	position.x = cocos2d::clampf(position.x, cameraXMin, cameraXMax);
	position.y = cocos2d::clampf(position.y, cameraYMin, cameraYMax);

	return position;
}

void InGameScene::StopGame()
{
	_player->getPhysicsBody()->setVelocity(Vec2::ZERO);
	stopAction(_followingPlayer);
	unscheduleUpdate();
	_eventDispatcher->pauseEventListenersForTarget(this);
}

void InGameScene::ShowVictoryDialog()
{
	auto dialog = Cocos2dCreator::CreateNode<VictoryDialog>();
	dialog->SetLevelTextContent(std::to_string(_currentLevel));
	dialog->SetTimeTextContent(time_temp);
	dialog->SetBestTimeTextContent(bestTime_temp);
	dialog->SetOnHomeButtonPressed([](cocos2d::Ref *) {});
	dialog->SetOnNextButtonPressed([this](cocos2d::Ref *)
								   {
									   auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel + 1);
									   auto sceneWithTransition = cocos2d::TransitionMoveInR::create(1, scene);
									   cocos2d::Director::getInstance()->replaceScene(sceneWithTransition);
								   });
	getParent()->addChild(dialog, INT16_MAX);
}