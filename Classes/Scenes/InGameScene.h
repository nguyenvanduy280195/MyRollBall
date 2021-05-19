#pragma once

#include "2d/CCLayer.h"
#include "Utils/Cocos2dCreator.h"

namespace cocos2d
{
class TMXTiledMap;
class Sprite;
}

class InGameScene : public cocos2d::Layer
{
	using Super = cocos2d::Layer;
	using Point = cocos2d::Point;
public:
	static cocos2d::Scene* CreateScene();

	~InGameScene();

	bool init() override;
	void update(float) override;



private:
	class Level* _level;
	class Player* _player;
	
	class HandlerManager* _handlerManager;
};
