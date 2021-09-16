#pragma once

#include "2d/CCSprite.h"
#include "IPlayer.h"

namespace cocos2d
{
class PhysicsBody;
}

class Player : public cocos2d::Sprite, public IPlayer
{
public:
	// Inherited via cocos2d::Sprite
	bool init(class IInGameScene* owner, const cocos2d::Vec2& position);
	void update(float) override;

	// Inherited via IPlayer
	void Break() override;
	void Dash() override;
	void MoveToCenterGoal(const cocos2d::Vec2& goalPosition, const std::function<void()>& onMovingDone) override;
	void RemoveBodyFromWorld() override;
	cocos2d::Node* AsNode() const override;

private:
	// creating body always successes
	// if _inGameScene->GetHandlerManager()->GetGameInfo() is nullptr
	// then this uses default value
	cocos2d::PhysicsBody* MakeBody();

	// moving by acceleration successes if
	// and _inGameScene->GetHandlerManager() is not nullptr
	// and _inGameScene->GetHandlerManager()->GetGameInfo() is not nullptr	
	void MoveByAcceleration(const cocos2d::Vec2& accelerationVec2);

	// moving by keyboard successes if
	// and _inGameScene->GetHandlerManager() is not nullptr
	// and _inGameScene->GetHandlerManager()->GetGameInfo() is not nullptr	
	void MoveByKeyboard(float deltaTime);


	class IInGameScene* _inGameScene;
};
