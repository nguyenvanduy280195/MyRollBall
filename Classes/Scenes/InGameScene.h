#pragma once

#include "2d/CCLayer.h"
#include "Utils/Cocos2dCreator.h"
#include "IScene.h"

namespace cocos2d
{
class TMXTiledMap;
class Sprite;
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

	// Inherited via IScene
	virtual ScreenLog* GetScreenLog() const override { return _screenLog; }
	virtual HandlerManager* GetHandlerManager() const override { return _handlerManager; }
	virtual GameInfo* GetGameInfo() const override { return _gameInfo; }

	void NextLevel();

private:
	void UpdateCameraPosition();


	class Level* _level;
	class Player* _player;
	
	class HandlerManager* _handlerManager;
	class GameInfo* _gameInfo;
	class ScreenLog* _screenLog;

	int _currentLevel;

	const std::string FORMAT_LEVEL = "levels/%03d.tmx";
};
