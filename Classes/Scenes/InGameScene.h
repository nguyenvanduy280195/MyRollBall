#pragma once

#include "2d/CCLayer.h"
#include "IInGameScene.h"

namespace cocos2d
{
class TMXTiledMap;
class Sprite;
class Follow;
}

class InGameScene : public cocos2d::Layer, public IInGameScene
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

	void IncreaseNumberOfCoin();
	void ShowKeyInScreenInfo();

	// Inherited via IScene
	virtual HandlerManager* GetHandlerManager() const override;
	virtual GameInfo* GetGameInfo() const override;

private: 
	void TakeCameraAfterPlayer();

	bool InitPlayer();
	bool InitLevel();
	bool InitScreenInfoLayer();
	bool InitProfile();
	
	class Level* _level;
	class IPlayer* _player;
	
	class HandlerManager* _handlerManager;
	class GameInfo* _gameInfo;
	class ScreenInfoLayer* _screenInfoLayer;

	int _currentLevel;
	int _nCoins;

	const std::string FORMAT_LEVEL = "levels/%03d.tmx";
	
	const std::string time_temp = "5:43";
	const std::string bestTime_temp = "1:23";

	cocos2d::Follow* _followingPlayerAction;
};
