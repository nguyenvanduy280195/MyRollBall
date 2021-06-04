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

using Super = cocos2d::Sprite;
using KeyCode = cocos2d::EventKeyboard::KeyCode;



bool Player::init(const cocos2d::Vec2& position, HandlerManager* handlerManager)
{
	if (!Super::initWithFile("levels/assets/ball_blue_large.png"))
	{
		return false;
	}
	_handlerManager = handlerManager;
	_handlerManager->accelerationHandler->onAcceleration = [this](cocos2d::Acceleration* acceleration)
	{
		auto x = 0;
		auto y = 0;
		auto abs = [](double x) { return x > 0 ? x : -x; };

		if (abs(_physicsBody->getVelocity().x) < _info.maxMoveSpeed)
		{
			x = _info.moveSpeed * acceleration->x;
		}
		if (abs(_physicsBody->getVelocity().y) < _info.maxMoveSpeed)
		{
			y = _info.moveSpeed * acceleration->y;
		}

		_physicsBody->applyImpulse(Vec2(x, y));
	};

	InitInfo();

	setPosition(position);

	auto body = MakeBody();
	setPhysicsBody(body);

	return true;
}

void Player::update(float deltaTime)
{
	if (_preventer == nullptr)
	{
		//_preventer = Cocos2dCreator::CreateNode<PlayerPreventer>(this, _handlerManager->keyboardHandler);
		//_parent->addChild(_preventer);
	}

	int deltaX = 0;

	if (_onGround && _handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_RIGHT_ARROW])
	{
		deltaX = 1;

	}
	else if (_onGround && _handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_LEFT_ARROW])
	{
		deltaX = -1;
	}


	int deltaY = 0;
	if (_onGround && _handlerManager->keyboardHandler->pressingKeys[KeyCode::KEY_SPACE])
	{
		deltaY++;
		_onGround = false;
	}



	// limit velocity
	if (_physicsBody->getVelocity().x > _info.maxMoveSpeed || _physicsBody->getVelocity().x < -_info.maxMoveSpeed)
	{
		deltaX = 0;
	}

	auto force = Vec2(deltaX * _info.moveSpeed, deltaY * _info.jumpSpeed);
	_physicsBody->applyImpulse(force);

}

cocos2d::PhysicsBody* Player::MakeBody()
{
	auto& contentSize = getContentSize();
	const auto destiny = 2 / _info.moveSpeed;
	const auto restitution = .5f;
	const auto friction = .5f;

	auto body = cocos2d::PhysicsBody::createCircle(0.5f * contentSize.width, cocos2d::PhysicsMaterial(destiny, restitution, friction));
	body->setCategoryBitmask(0x1);
	body->setCollisionBitmask(0xFFFFFFFF);
	body->setContactTestBitmask(0xFFFFFFFF);

	return body;
}

void Player::InitInfo()
{
	_info.maxMoveSpeed = 400.f;
	_info.moveSpeed = 500.f;
	_info.jumpSpeed = 5000.f;
}

//========================================================================

bool PlayerPreventer::init(class Player* player, class KeyboardHandler* keyboardHandler)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	_player = player;
	_keyboardHandler = keyboardHandler;

	auto& playerContentSize = _player->getContentSize();

	// left wall
	auto leftPreventer = cocos2d::Sprite::create();
	addChild(leftPreventer);
	if (auto body = MakeWallBody())
	{
		leftPreventer->setPhysicsBody(body);
	}
	leftPreventer->setPositionX(-2 * playerContentSize.width);

	// right wall
	auto rightPreventer = cocos2d::Sprite::create();
	addChild(rightPreventer);
	if (auto body = MakeWallBody())
	{
		rightPreventer->setPhysicsBody(body);
	}
	rightPreventer->setPositionX(2 * playerContentSize.width);

	setPosition(_player->getPosition());

	scheduleUpdate();
	return true;
}

void PlayerPreventer::update(float dt)
{
	if (_keyboardHandler->pressingKeys[KeyCode::KEY_RIGHT_ARROW] || _keyboardHandler->pressingKeys[KeyCode::KEY_LEFT_ARROW])
	{
		setPositionX(_player->getPositionX());
	}
}

cocos2d::PhysicsBody* PlayerPreventer::MakeWallBody() const
{
	auto body = cocos2d::PhysicsBody::createBox(cocos2d::Size(_player->getContentSize().width, 500.f), cocos2d::PhysicsMaterial(.5f, .0f, .0f));

	body->setDynamic(false);

	return body;
}