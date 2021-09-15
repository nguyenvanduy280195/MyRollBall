#pragma once

#include "2d/CCSprite.h"
#include "base/CCValue.h"

namespace cocos2d
{
class ActionInterval;
}

class Crossbow : public cocos2d::Sprite
{
public:
	bool init(cocos2d::ValueMap& value, const cocos2d::Size& levelSize);
	void update(float);
	virtual ~Crossbow();

private:
	cocos2d::Sprite* MakeArrow(const std::string imagePath) const;
	cocos2d::PhysicsBody* MakeArrowPhysicsBody(const cocos2d::Size& size) const;
	cocos2d::ActionInterval* MakeArrowAction(const cocos2d::Vec2& destination, float movementDuration);

	cocos2d::ActionInterval* MakeCrossbowAction(int nFrames, float animationDelay);

	cocos2d::Sprite* _arrow;

	cocos2d::ActionInterval* _crossbowAction;
	cocos2d::ActionInterval* _arrowAction;
};