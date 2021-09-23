#pragma once

#include "2d/CCTMXTiledMap.h"

namespace cocos2d
{
class PhysicsBody;
class PhysicsShape;
class Sprite;
class Action;
}

class Level : public cocos2d::TMXTiledMap
{
	
public:
	bool init(const std::string& filename);

	cocos2d::Vec2 GetStartPosition() const;
	cocos2d::Vec2 GetGoalPosition() const;

	bool Locked = false;

private:
	cocos2d::PhysicsBody* MakeBody();

	std::vector<cocos2d::PhysicsShape*> MakeWalls();

	cocos2d::Sprite* MakeSpriteWithBoxBodyShape(cocos2d::ValueMap&);
	cocos2d::Sprite* MakeSpriteWithCircleBodyShape(cocos2d::ValueMap&);
	cocos2d::Sprite* MakeGoal(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeRotatingBlock(cocos2d::ValueMap& value);

	cocos2d::PhysicsShape* MakeSpikeBodyShape(cocos2d::ValueMap& value);
	
	int _currentLevel = 1;
	cocos2d::Vec2 _startPosition;
};