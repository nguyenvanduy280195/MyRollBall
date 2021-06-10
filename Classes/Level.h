#pragma once

#include "2d/CCTMXTiledMap.h"

namespace cocos2d
{
class PhysicsBody;
class PhysicsShape;
class Sprite;
}

class Level : public cocos2d::TMXTiledMap
{
	
public:
	bool init(const std::string& filename);

	cocos2d::Vec2 GetStartPosition() const;

	bool Locked = false;

private:
	cocos2d::PhysicsBody* MakeBody();

	std::vector<cocos2d::PhysicsShape*> MakeWalls();

	cocos2d::Sprite* MakeStart(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeGoal(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeKey(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeLaserShooter(cocos2d::ValueMap& value);
	int _currentLevel = 1;
};