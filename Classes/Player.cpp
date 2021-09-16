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
#include "Scenes/IInGameScene.h"
#include "ScreenLog/ScreenLog.h"
#include "Bitmask.h"
#include "Infos/PlayerInfo.h"

#include "ScreenLog/ScreenLog.h"
#include "ScreenLog/ScopeLog.h"

using Super = cocos2d::Sprite;
using KeyCode = cocos2d::EventKeyboard::KeyCode;
using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

#define DRAWING_PLAYER_ANCHOR_POINT 0

bool Player::init(class IInGameScene* owner, const Vec2& position)
{
	if (owner == nullptr)
	{
		CCLOG("Player::init::owner is nullptr");
		return false;
	}
	_inGameScene = owner;

	PlayerInfo playerInfo;
	if (auto gameInfo = _inGameScene->GetGameInfo())
	{
		playerInfo = gameInfo->GetPlayerInfo();
	}
	
	if (!Super::initWithFile(playerInfo.imagePath))
	{
		CCLOG("Creating Player Object fails");
		return false;
	}

#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM:
	_inGameScene->GetHandlerManager()->accelerationHandler->onAcceleration = [this](cocos2d::Acceleration* acceleration)
	{
		const Vec2 accelerationVec2((float)acceleration->x, (float)acceleration->y);
		MoveByAcceleration(accelerationVec2);
	};
#endif

	setPosition(position);
	setPhysicsBody(MakeBody());
	setAnchorPoint(Vec2(playerInfo.anchorPointX, playerInfo.anchorPointY));

#if DRAWING_PLAYER_ANCHOR_POINT
	auto anchorPoint = cocos2d::DrawNode::create();
	anchorPoint->drawDot(Vec2(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y), 5, cocos2d::Color4F(0.0, 1.0, 0.0, 1.0));
	addChild(anchorPoint);
#endif

	CCLOG("Creating Player Object successes");
	return true;
}

void Player::update(float deltaTime)
{
#if CC_PLATFORM_ANDROID != CC_TARGET_PLATFORM:
	MoveByKeyboard(deltaTime);
#endif
}

void Player::Break()
{
	// run broken animations
}

void Player::Dash()
{
	auto velocity = _physicsBody->getVelocity();
	auto newVelocity = (1 + _inGameScene->GetGameInfo()->GetPlayerInfo().dashPower) * velocity.getNormalized() * 500;
	_physicsBody->setVelocity(newVelocity);
}

void Player::MoveToCenterGoal(const cocos2d::Vec2& goalPosition, const std::function<void()>& onMovingDone)
{
	auto moveToGoalAction = cocos2d::MoveTo::create(1, goalPosition);
	auto onMoveToGoalActionFinished = cocos2d::CallFunc::create(onMovingDone);
	runAction(cocos2d::Sequence::createWithTwoActions(moveToGoalAction, onMoveToGoalActionFinished));
}

void Player::RemoveBodyFromWorld() { _physicsBody->removeFromWorld(); }

cocos2d::Node* Player::AsNode() const { return (cocos2d::Node*)this; }

void Player::MoveByAcceleration(const Vec2& accelerationVec2)
{
	if (auto handlerManager = _inGameScene->GetHandlerManager())
	{
		if (auto gameInfo = _inGameScene->GetGameInfo())
		{
			const auto moveSpeed = gameInfo->GetPlayerInfo().moveSpeed;
			auto f = [moveSpeed](float t)
			{
				const auto deltaT = (int)(t * 100);
				const auto tt = (float)deltaT / 100;
				return moveSpeed * tt * 2;
			};

			const auto fx = f(accelerationVec2.x);
			const auto fy = f(accelerationVec2.y);

			_physicsBody->applyImpulse(Vec2(fx, fy));
		}
	}
}

void Player::MoveByKeyboard(float deltaTime)
{
	if (auto handlerManager = _inGameScene->GetHandlerManager())
	{
		auto x = 0.f;
		auto y = 0.f;
		auto moveSpeed = 10000.0f;

		if (auto gameInfo = _inGameScene->GetGameInfo())
		{
			moveSpeed = gameInfo->GetPlayerInfo().moveSpeed;
		}

		if (handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_RIGHT_ARROW])
		{
			x = moveSpeed;
		}
		else if (handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_LEFT_ARROW])
		{
			x = -moveSpeed;
		}

		if (handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_UP_ARROW])
		{
			y = moveSpeed;
		}
		else if (handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_DOWN_ARROW])
		{
			y = -moveSpeed;
		}

		_physicsBody->applyImpulse(Vec2(x, y));
	}
}

cocos2d::PhysicsBody* Player::MakeBody()
{
	auto radius = 0.5f * _contentSize.width;
	auto size = Size(radius, radius);
	auto offset = Vec2(0, -_contentSize.height * (0.5f - _anchorPoint.y));
	auto destiny = 0.1f; // PHYSICSSHAPE_MATERIAL_DEFAULT
	auto restitution = 0.5f; // PHYSICSSHAPE_MATERIAL_DEFAULT
	auto friction = 0.5f; // PHYSICSSHAPE_MATERIAL_DEFAULT

	if (auto gameInfo = _inGameScene->GetGameInfo())
	{
		destiny = gameInfo->GetPlayerInfo().destiny;
		restitution = gameInfo->GetPlayerInfo().restitution;
		friction = gameInfo->GetPlayerInfo().friction;
	}

	auto body = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PhysicsMaterial(destiny, restitution, friction), offset);

	body->setCategoryBitmask(PLAYER_CATEGORY_BITMASK);
	body->setCollisionBitmask(PLAYER_COLLISION_BITMASK);
	body->setContactTestBitmask(PLAYER_CONTACT_TEST_BITMASK);

	body->setRotationEnable(false);
	body->setAngularVelocityLimit(0);
	body->setLinearDamping(1.0f);

	return body;
}
