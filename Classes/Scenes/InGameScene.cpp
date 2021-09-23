#include "InGameScene.h"

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

#include "2d/CCScene.h"
#include "physics/CCPhysicsWorld.h"

#include "2d/CCCamera.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"

#include "Managers/HandlerManager.h"
#include "Managers/KeyboardHandler.h"

#include "Infos/GameInfo.h"
#include "IntroLevelScene.h"

#include "Dialogs/VictoryDialog.h"
#include "Dialogs/GameOverDialog.h"
#include "Dialogs/PausingGameDialog.h"

#include "MyComponents/MyButton.h"

#include "Player.h"
#include "Level.h"

#include "Layers/ScreenInfoLayer.h"

#include "Utils/StaticMethods.h"
#include "Utils/Cocos2dCreator.h"

#include "json/document.h"

#include "platform/CCFileUtils.h"

//#include "ScreenLog/ScreenLog.h"

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
//#if 1
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

	return InitProfile() && InitScreenInfoLayer() && InitLevel() && InitPlayer();
}

bool InGameScene::InitPlayer() { 
	if (_player = Cocos2dCreator::CreateNode<Player>(this, _level->GetStartPosition()))
	{
		addChild(_player->AsNode());
		return true;
	}
	return false;
}

bool InGameScene::InitLevel()
{
	const auto path = cocos2d::StringUtils::format(FORMAT_LEVEL.c_str(), _currentLevel);
	if (_level = Cocos2dCreator::CreateNode<Level>(path))
	{
		addChild(_level);
		CCLOG("Creating Level Object successes");
		return true;
	}

	CCLOG("Creating Level Object fails");
	return false;
}

bool InGameScene::InitScreenInfoLayer()
{
	if (_screenInfoLayer = Cocos2dCreator::CreateNode<ScreenInfoLayer>())
	{
		addChild(_screenInfoLayer, 1000000);


		_screenInfoLayer->AddPauseButtonCallback([this](cocos2d::Ref* ref)
		{
			auto button = (MyButton*)ref;
			button->setEnabled(false);
			auto dialog = Cocos2dCreator::CreateNode<PausingGameDialog>(-_position);
			dialog->OnEventPaused = [this, button]() { 
				
				_eventDispatcher->pauseEventListenersForTarget(this); 
				//button->setEnabled(false);
			};
			dialog->OnEventUnpaused = [this, button]() {
				_eventDispatcher->resumeEventListenersForTarget(this);
				button->setEnabled(true);
			};
			addChild(dialog);
			dialog->Show();
		});
		_screenInfoLayer->SetCoinText(std::to_string(_nCoins));

		CCLOG("Creating ScreenInfoLayer Object successes");
		return true;
	}

	CCLOG("Creating ScreenInfoLayer Object fails");
	return false;
}

bool InGameScene::InitProfile()
{
	rapidjson::Document document;
	auto profilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile.json";
	if (!cocos2d::FileUtils::getInstance()->isFileExist(profilePath))
	{
		CCLOG("profile.json doesn't exist. The file will be created");
		document.SetObject();
		document.AddMember("coin", _nCoins, document.GetAllocator());

		StaticMethods::WriteJSONOnFile(document, profilePath);
	}
	else
	{
		CCLOG("profile.json exists");
		auto jsonFileContent = cocos2d::FileUtils::getInstance()->getStringFromFile(profilePath);
		document.Parse(jsonFileContent.c_str());
		_nCoins = document["coin"].GetInt();
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
	if (_screenInfoLayer)
	{
		const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
		_screenInfoLayer->setPosition(visibleOrigin - _position);
	}
}

void InGameScene::TakeCameraAfterPlayer()
{
	auto screenInfoHeight = 0.0f;
	if (_screenInfoLayer)
	{
		screenInfoHeight = _screenInfoLayer->GetHeight();
	}

	const auto& levelPosition = _level->getPosition();
	const auto& levelContentSize = _level->getContentSize();
	const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	const auto rect = cocos2d::Rect(-visibleOrigin - Vec2(0, screenInfoHeight), Size(levelContentSize.width + 2 * visibleOrigin.x, levelContentSize.height + 2 * visibleOrigin.y + screenInfoHeight));
	_followingPlayerAction = cocos2d::Follow::create(_player->AsNode(), rect);
	runAction(_followingPlayerAction);
}

void InGameScene::IncreaseNumberOfCoin()
{
	_nCoins++;
	if (_screenInfoLayer)
	{
		_screenInfoLayer->SetCoinText(std::to_string(_nCoins));
	}
}

void InGameScene::ShowKeyInScreenInfo()
{
	if (_screenInfoLayer)
	{
		_screenInfoLayer->ShowKey();
	}
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
	auto profilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "profile.json";
	auto document = StaticMethods::GetJSONFromFile(profilePath);
	document["coin"] = _nCoins;
	StaticMethods::WriteJSONOnFile(document, profilePath);

	if (_player)
	{
		_player->MoveToCenterGoal(_level->GetGoalPosition(), [this]() { ShowVictoryDialog(); });
	}

	StopGame();
}

void InGameScene::ShowGameOver()
{
	if (_player)
	{
		_player->Break();
	}

	
	ShowGameOverDialog();

	StopGame();
}

void InGameScene::ShowVictoryDialog()
{
	auto dialog = Cocos2dCreator::CreateNode<VictoryDialog>();
	dialog->SetLevelTextContent(std::to_string(_currentLevel));
	dialog->SetTimeTextContent(time_temp);
	dialog->SetBestTimeTextContent(bestTime_temp);
	dialog->SetOnMainMenuButtonPressed([](cocos2d::Ref*){ cocos2d::Director::getInstance()->popToRootScene(); });
	dialog->SetOnNextButtonPressed([this](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel + 1);
		cocos2d::Director::getInstance()->replaceScene(scene);
	});
	_parent->addChild(dialog, INT16_MAX);
	dialog->Show();
}

void InGameScene::ShowGameOverDialog()
{
	auto dialog = Cocos2dCreator::CreateNode<GameOverDialog>();
	dialog->SetOnMainMenuButtonPressed([](cocos2d::Ref*){ cocos2d::Director::getInstance()->popToRootScene(); });
	dialog->SetOnReplayButtonPressed([this](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(_currentLevel);
		cocos2d::Director::getInstance()->replaceScene(scene);
	});
	_parent->addChild(dialog, INT16_MAX);
	dialog->Show();
}

HandlerManager* InGameScene::GetHandlerManager() const
{
	if (_handlerManager == nullptr)
	{
		CCLOG("InGameScene::_handlerManager is nullptr");
	}
	
	return _handlerManager;
}

GameInfo* InGameScene::GetGameInfo() const
{
	if (_gameInfo == nullptr)
	{
		CCLOG("InGameScene::_gameInfo is nullptr");
	}

	return _gameInfo;
}