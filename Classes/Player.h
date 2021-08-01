#pragma once

#include "2d/CCSprite.h"
#include "2d/CCNode.h"
#include "IPlayer.h"

namespace cocos2d
{
class PhysicsBody;
}

class Player : public cocos2d::Sprite, public IPlayer
{
public:
	// Inherited via cocos2d::Sprite
	bool init(class IScene* owner, const cocos2d::Vec2& position);
	void update(float) override;


	// Inherited via IPlayer
	void Break() override;
	void Dash() override;
	void MoveToCenterGoal(const cocos2d::Vec2& goalPosition, const std::function<void()>& onMovingDone) override;
	void RemoveBodyFromWorld() override;
	cocos2d::Node* AsNode() const override;
private:
	cocos2d::PhysicsBody* MakeBody();
	void MoveByAcceleration(const cocos2d::Vec2& accelerationVec2);
	void MoveByKeyboard(float deltaTime);
	
	class IScene* _owner;

};
