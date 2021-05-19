#pragma once

#include "2d/CCSprite.h"
#include "2d/CCNode.h"

namespace cocos2d
{
class PhysicsBody;
}

class HandlerManager;

class Player : public cocos2d::Sprite
{
	using Vec2 = cocos2d::Vec2;
public:

	bool init(const Vec2& position, HandlerManager* handlerManager);
	void update(float) override;

	void SetOnGround(bool onGround) { _onGround = onGround; }

	const float& GetMoveSpeed() const { return _info.moveSpeed; }
private:
	 cocos2d::PhysicsBody* MakeBody();
	 void InitInfo();

	struct Info
	{
		float maxMoveSpeed = 0.f;
		float moveSpeed = 0.f;
		float jumpSpeed = 0.f;

	} _info;

	HandlerManager* _handlerManager;

	bool _onGround = false;

	class PlayerPreventer* _preventer;
};

class PlayerPreventer : public cocos2d::Node
{
public:
	bool init(class Player*, class KeyboardHandler*);
	void update(float) override;

private:
	cocos2d::PhysicsBody* MakeWallBody() const;

	class Player* _player;
	class KeyboardHandler* _keyboardHandler;
};