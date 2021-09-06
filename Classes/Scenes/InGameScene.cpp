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
#include "ui/UIButton.h"

#include "Dialogs/VictoryDialog.h"
#include "Dialogs/GameOverDialog.h"

#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCAction.h"
#include "ScreenLog/ScreenLog.h"

#include "MyCustomGUI.inl"
#include "Player.h"

using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

cocos2d::Scene* InGameScene::CreateScene()
{
	return CreateScene(1);
}

cocos2d::Scene* InGameScene::CreateScene(int level)
{
	auto scene = cocos2d::Scene::createWithPhysics();

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
#endif
	scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0.0f));

	if (auto inGame = Cocos2dCreator::CreateNode<InGameScene>(level))
	{
		scene->addChild(inGame);
		return scene;
	}
	else
	{
		return nullptr;
	}
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

	_screenInfo = Cocos2dCreator::CreateNode<ScreenInfo>();
	addChild(_screenInfo, 1000000);

	// level
	const auto path = cocos2d::StringUtils::format(FORMAT_LEVEL.c_str(), _currentLevel);
	if (_level = Cocos2dCreator::CreateNode<Level>(path))
	{
		addChild(_level);
	}
	else
	{
		CCLOG("Creating Level Object fails");
		return false;
	}


	// player
	if (_player = Cocos2dCreator::CreateNode<Player>(this, _level->GetStartPosition()))
	{
		addChild(_player->AsNode());
	}
	else
	{
		CCLOG("Creating Player Object fails");
		return false;
	}


	return true;
}

void InGameScene::onEnterTransitionDidFinish()
{
	Super::onEnterTransitionDidFinish();
	scheduleUpdate();
	TakeCameraAfterPlayer();
}

void InGameScene::update(float)
{
	if (_screenInfo)
	{
		const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
		_screenInfo->setPosition(visibleOrigin - _position);
	}
}

void InGameScene::TakeCameraAfterPlayer()
{
	auto screenInfoHeight = 0.0f;
	if (_screenInfo)
	{
		screenInfoHeight = _screenInfo->GetHeight();
	}

	const auto& levelPosition = _level->getPosition();
	const auto& levelContentSize = _level->getContentSize();
	const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	const auto rect = cocos2d::Rect(-visibleOrigin - Vec2(0, screenInfoHeight), Size(levelContentSize.width + 2 * visibleOrigin.x, levelContentSize.height + 2 * visibleOrigin.y + screenInfoHeight));
	_followingPlayerAction = cocos2d::Follow::create(_player->AsNode(), rect);
	runAction(_followingPlayerAction);
}

cocos2d::Vec2 InGameScene::GetVectorToPlayer() const
{
	const auto& winSize = cocos2d::Director::getInstance()->getWinSize();
	auto& playerPosition = _player->AsNode()->getPosition();
	Vec2 position = 0.5f * winSize - playerPosition;

	const auto& levelPosition = _level->getPosition();
	const auto& levelSize = _level->getContentSize();

	const auto cameraXMin = -((levelPosition.x + levelSize.width) - winSize.width);
	const auto cameraXMax = -levelPosition.x;
	const auto cameraYMin = -((levelPosition.y + levelSize.height) - winSize.height);
	const auto cameraYMax = -levelPosition.y;

	position.x = cocos2d::clampf(position.x, cameraXMin, cameraXMax);
	position.y = cocos2d::clampf(position.y, cameraYMin, cameraYMax);

	return position;
}

void InGameScene::IncreaseNumberOfCarrots()
{
	_nCarrots++;
	if (_screenInfo)
	{
		_screenInfo->SetCarrotText(std::to_string(_nCarrots));
	}
}

void InGameScene::ShowKeyInScreenInfo()
{
	_screenInfo->ShowKey();
}

void InGameScene::StopGame()
{
	_player->RemoveBodyFromWorld();
	stopAction(_followingPlayerAction);
	unscheduleUpdate();
	_eventDispatcher->pauseEventListenersForTarget(this);
}

void InGameScene::ShowVictory()
{
	StopGame();
	_player->MoveToCenterGoal(_level->GetGoalPosition(), [this]() { ShowVictoryDialog(); });
}

void InGameScene::ShowGameOver()
{
	StopGame();
	_player->Break();
	ShowGameOverDialog();
}

void InGameScene::ShowVictoryDialog()
{
	auto dialog = Cocos2dCreator::CreateNode<VictoryDialog>();
	dialog->SetLevelTextContent(std::to_string(_currentLevel));
	dialog->SetTimeTextContent(time_temp);
	dialog->SetBestTimeTextContent(bestTime_temp);
	dialog->SetOnMainMenuButtonPressed([](cocos2d::Ref*)
	{
		cocos2d::Director::getInstance()->popToRootScene();
	});
	dialog->SetOnNextButtonPressed([this](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel + 1);
		cocos2d::Director::getInstance()->replaceScene(scene);
	});
	getParent()->addChild(dialog, INT16_MAX);
	dialog->Show();
}

void InGameScene::ShowGameOverDialog()
{
	auto dialog = Cocos2dCreator::CreateNode<GameOverDialog>();
	dialog->SetOnMainMenuButtonPressed([](cocos2d::Ref*)
	{
		cocos2d::Director::getInstance()->popToRootScene();
	});
	dialog->SetOnReplayButtonPressed([this](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel);
		cocos2d::Director::getInstance()->replaceScene(scene);
	});
	getParent()->addChild(dialog, INT16_MAX);
	dialog->Show();
}


//////////////////////

bool ScreenInfo::init()
{
	if (!MyCustomGUI<cocos2d::Layer>::init("ui/ingamescene-screen-info-horizontal.tmx"))
	{
		return false;
	}

	//setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	return true;
}

void ScreenInfo::SetCarrotText(const std::string& text)
{
	SetTextContent("carrot", text);
}

void ScreenInfo::ShowKey()
{
	if (auto child = _tiledMap->getChildByName("key"))
	{
		child->setVisible(true);
	}
}