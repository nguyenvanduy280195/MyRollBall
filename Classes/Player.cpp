#include "Player.h"

#include "base/CCDirector.h"
#include "physics/CCPhysicsBody.h"

#include "Managers/HandlerManager.h"
#include "Managers/AccelerationHandler.h"
#include "Managers/KeyboardHandler.h"

#include "2d/CCActionInterval.h"
#include "base/CCEventAcceleration.h"

#include "Utils/Cocos2dCreator.h"

#include "math/CCMathBase.h"

#include "Infos/GameInfo.h"
#include "Scenes/IScene.h"
#include "ScreenLog/ScreenLog.h"
#include "Bitmask.h"
#include "Infos/PlayerInfo.h"

#include "ScreenLog/ScreenLog.h"

using Super = cocos2d::Sprite;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

bool Player::init(IScene* owner, const Vec2& position)
{
	//ScopeLog scopeLog("Player::init(...)");
	if (!Super::initWithFile("levels/assets/ball_blue_large.png"))
	{
		return false;
	}
	_owner = owner;
	_owner->GetHandlerManager()->accelerationHandler->onAcceleration = [this](cocos2d::Acceleration* acceleration)
	{
		const auto accelerationVec2 = Vec2((float)acceleration->x, (float)acceleration->y);
		MoveByAcceleration(accelerationVec2);
	};


	setPosition(position);

	auto body = MakeBody();
	setPhysicsBody(body);

	return true;
}

void Player::update(float deltaTime)
{
	MoveByKeyboard(deltaTime);
}

void Player::Break()
{
}

void Player::MoveByAcceleration(const Vec2& accelerationVec2)
{
	auto f = [this](float t) {
		auto x = _owner->GetGameInfo()->GetPlayerInfo().moveSpeed * t;
		return x;
	};
	const auto deltaX = (int)(accelerationVec2.x * 100);
	const auto x = (float) deltaX / 100;
	const auto fx = f(x);

    const auto deltaY = (int)(accelerationVec2.y * 100);
	const auto y = (float) deltaY / 100;
	const auto fy = f(y);

	_physicsBody->applyImpulse(Vec2(fx, fy));

	// debug
	{
		const auto& velocity = _physicsBody->getVelocity();
		CCLOG("P(%.0f, %.0f) V(%f, %f) A(%f, %f) F(%f, %f)",_position.x, _position.y,
                                                            velocity.x, velocity.y,
                                                            accelerationVec2.x, accelerationVec2.y,
                                                            fx, fy);
	}
}

void Player::MoveByKeyboard(float deltaTime)
{
	// x
	auto x = 0.f;
	if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_RIGHT_ARROW] && _physicsBody->getVelocity().x < _owner->GetGameInfo()->GetPlayerInfo().maxMoveSpeed)
	{
		x = _owner->GetGameInfo()->GetPlayerInfo().moveSpeed;
	}
	else if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_LEFT_ARROW] && -_owner->GetGameInfo()->GetPlayerInfo().maxMoveSpeed < _physicsBody->getVelocity().x)
	{
		x = -_owner->GetGameInfo()->GetPlayerInfo().moveSpeed;
	}

	// y
	auto y = 0.f;
	if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_UP_ARROW])
	{
		y = _owner->GetGameInfo()->GetPlayerInfo().moveSpeed;
	}
	if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_DOWN_ARROW])
	{
		y = -_owner->GetGameInfo()->GetPlayerInfo().moveSpeed;
	}

	_physicsBody->applyImpulse(Vec2(x, y));
}

cocos2d::PhysicsBody* Player::MakeBody()
{
	auto& contentSize = getContentSize();
	const auto destiny = _owner->GetGameInfo()->GetPlayerInfo().destiny;
	const auto restitution = _owner->GetGameInfo()->GetPlayerInfo().restitution;
	const auto friction = _owner->GetGameInfo()->GetPlayerInfo().friction;

	auto body = cocos2d::PhysicsBody::createCircle(0.5f * contentSize.width, cocos2d::PhysicsMaterial(destiny, restitution, friction));
	body->setCategoryBitmask(PLAYER_CATEGORY_BITMASK);
	body->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
	body->setContactTestBitmask(PLAYER_CONTACT_TEST_BITMASK);

	return body;
}
