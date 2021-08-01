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

	cocos2d::Sprite* MakeStart(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeGoal(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeKey(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeLaserShooter(cocos2d::ValueMap& value);
	cocos2d::Sprite* MakeRotatingBlock(cocos2d::ValueMap& value);

	cocos2d::PhysicsShape* MakeSpikeBodyShape(cocos2d::ValueMap& value);

	cocos2d::Sprite* MakeLaserBeam(cocos2d::ValueMap& value, const cocos2d::Vec2& position);
	cocos2d::PhysicsBody* MakeLaserBeamBody(const cocos2d::Size&);
	cocos2d::Action* MakeLaserBeamAction(cocos2d::PhysicsBody* body,
										 float durationOfFadeIn,
										 float durationOfDelay,
										 float durationOfFadeOut,
										 float durationOfDelayFadeOut);

	
	int _currentLevel = 1;
	cocos2d::Vec2 _startPosition;
};