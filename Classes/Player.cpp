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
#include "ScreenLog/ScopeLog.h"

using Super = cocos2d::Sprite;
using KeyCode = cocos2d::EventKeyboard::KeyCode;
using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

bool Player::init(IScene* owner, const Vec2& position)
{
	_owner = owner;
	const auto& playerInfo = _owner->GetGameInfo()->GetPlayerInfo();

	if (!Super::initWithFile(playerInfo.imagePath))
	{
		return false;
	}

	_owner->GetHandlerManager()->accelerationHandler->onAcceleration = [this](cocos2d::Acceleration* acceleration)
	{
		const auto accelerationVec2 = Vec2((float)acceleration->x, (float)acceleration->y);
		MoveByAcceleration(accelerationVec2);
	};


	setAnchorPoint(Vec2(playerInfo.anchorPointX, playerInfo.anchorPointY));

	// draw anchor point
	auto anchorPoint = cocos2d::DrawNode::create();
	anchorPoint->drawDot(Vec2(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y), 5, cocos2d::Color4F(0.0, 1.0, 0.0, 1.0));
	//addChild(anchorPoint);


	setPosition(position);

	auto body = MakeBody();
	setPhysicsBody(body);

	return true;
}

void Player::update(float deltaTime)
{
	MoveByKeyboard(deltaTime);
}

void Player::MoveByAcceleration(const Vec2& accelerationVec2)
{
	auto f = [this](float t)
	{
		auto x = _owner->GetGameInfo()->GetPlayerInfo().moveSpeed * t * 2;
		return x;
	};
	const auto deltaX = (int)(accelerationVec2.x * 100);
	const auto x = (float)deltaX / 100;
	const auto fx = f(x);

	const auto deltaY = (int)(accelerationVec2.y * 100);
	const auto y = (float)deltaY / 100;
	const auto fy = f(y);

	_physicsBody->applyImpulse(Vec2(fx, fy));

	// debug
	//{
	//	const auto& velocity = _physicsBody->getVelocity();
	//	CCLOG("P(%.0f, %.0f) V(%f, %f) A(%f, %f) F(%f, %f)", _position.x, _position.y,
	//		  velocity.x, velocity.y,
	//		  accelerationVec2.x, accelerationVec2.y,
	//		  fx, fy);
	//}
}

void Player::MoveByKeyboard(float deltaTime)
{
	// x
	auto x = 0.f;
	if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_RIGHT_ARROW])
	{
		x = _owner->GetGameInfo()->GetPlayerInfo().moveSpeed;
	}
	else if (_owner->GetHandlerManager()->keyboardHandler->pressingKeys[KeyCode::KEY_LEFT_ARROW])
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
	const auto radius = 0.5f * _contentSize.width;
	const auto size = Size(radius, radius);
	const auto destiny = _owner->GetGameInfo()->GetPlayerInfo().destiny;
	const auto restitution = _owner->GetGameInfo()->GetPlayerInfo().restitution;
	const auto friction = _owner->GetGameInfo()->GetPlayerInfo().friction;
	const auto offset = Vec2(0, -_contentSize.height * (0.5f - _anchorPoint.y));

	auto body = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PhysicsMaterial(destiny, restitution, friction), offset);

	body->setCategoryBitmask(PLAYER_CATEGORY_BITMASK);
	body->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
	body->setContactTestBitmask(PLAYER_CONTACT_TEST_BITMASK);

	body->setRotationEnable(false);
	body->setAngularVelocityLimit(0);
	body->setLinearDamping(1.0f);

	return body;
}


void Player::Break()
{
	// run broken animations
}

void Player::Dash()
{
	auto velocity = _physicsBody->getVelocity();
	auto newVelocity = (1 + _owner->GetGameInfo()->GetPlayerInfo().dashPower) * velocity.getNormalized() * 500;
	_physicsBody->setVelocity(newVelocity);
}


void Player::MoveToCenterGoal(const cocos2d::Vec2& goalPosition, const std::function<void()>& onMovingDone)
{
	auto moveToGoalAction = cocos2d::MoveTo::create(1, goalPosition);
	auto onMoveToGoalActionFinished = cocos2d::CallFunc::create(onMovingDone);
	runAction(cocos2d::Sequence::createWithTwoActions(moveToGoalAction, onMoveToGoalActionFinished));
}

void Player::RemoveBodyFromWorld()
{
	_physicsBody->removeFromWorld();
}

cocos2d::Node* Player::AsNode() const
{
	return (cocos2d::Node*)this;
}