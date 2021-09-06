#pragma once

#include "2d/CCLayer.h"
#include "Utils/Cocos2dCreator.h"
#include "IScene.h"
#include "2d/CCTMXTiledMap.h"
#include "MyCustomGUI.h"

namespace cocos2d
{
class TMXTiledMap;
class Sprite;
class Follow;
}

namespace cocos2d::ui
{
class Button;
}

class InGameScene : public cocos2d::Layer, public IScene
{
	using Super = cocos2d::Layer;
	using Point = cocos2d::Point;

	friend class ContactHandler;
public:
	static cocos2d::Scene* CreateScene();
	static cocos2d::Scene* CreateScene(int level);

	~InGameScene();

	bool init(int level);
	void update(float) override;
	void onEnterTransitionDidFinish() override;

	void ShowVictory();
	void ShowGameOver();

	void ShowVictoryDialog();
	void ShowGameOverDialog();
	void StopGame();

	void IncreaseNumberOfCarrots();
	void ShowKeyInScreenInfo();

	// Inherited via IScene
	virtual HandlerManager* GetHandlerManager() const override { return _handlerManager; }
	virtual GameInfo* GetGameInfo() const override { return _gameInfo; }

private: 
	void TakeCameraAfterPlayer();
	cocos2d::Vec2 GetVectorToPlayer() const;
	
	

	class Level* _level;
	class IPlayer* _player;
	
	class HandlerManager* _handlerManager;
	class GameInfo* _gameInfo;
	class ScreenInfo* _screenInfo;

	cocos2d::ui::Button* _dashButton;

	int _currentLevel;
	int _nCarrots;

	const std::string FORMAT_LEVEL = "levels/%03d.tmx";

	const std::string time_temp = "5:43";
	const std::string bestTime_temp = "1:23";

	cocos2d::Follow* _followingPlayerAction;

	
};

class ScreenInfo : public MyCustomGUI<cocos2d::Layer>
{
public:
	bool init();
	float GetHeight() const { return _tiledMap->getContentSize().height; }
	void SetCarrotText(const std::string& text);
	void ShowKey();
};