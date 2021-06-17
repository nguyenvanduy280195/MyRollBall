#pragma once

#include "2d/CCSprite.h"
#include "2d/CCNode.h"

namespace cocos2d
{
class PhysicsBody;
}

class Player : public cocos2d::Sprite
{
	using Vec2 = cocos2d::Vec2;
public:
	bool init(class IScene* owner, const Vec2& position);
	void update(float) override;
	void Break();

private:
	cocos2d::PhysicsBody* MakeBody();
	void MoveByAcceleration(const Vec2& accelerationVec2);
	void MoveByKeyboard(float deltaTime);
	
	class IScene* _owner;
};
