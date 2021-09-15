#include "ContactHandler.h"

#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsShape.h"
#include "base/CCEventDispatcher.h"

#include "Scenes/InGameScene.h"
#include "ScreenLog/ScreenLog.h"
#include "Level.h"
#include "Player.h"
#include "Bitmask.h"

#include "2d/CCActionInterval.h"


#define USE_AUDIO_ENGINE CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

//#include "editor-support/cocostudio/SimpleAudioEngine.h"

ContactHandler::ContactHandler(InGameScene* owner)
{
	_inGameScene = owner;

	auto contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(ContactHandler::OnContactBegan, this);
	contactListener->onContactPreSolve = CC_CALLBACK_2(ContactHandler::OnContactPreSolve, this);
	contactListener->onContactPostSolve = CC_CALLBACK_2(ContactHandler::OnContactPostSolve, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(ContactHandler::OnContactSeparate, this);
	_inGameScene->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, _inGameScene);
}

bool ContactHandler::OnContactBegan(cocos2d::PhysicsContact& contact)
{
	CCASSERT(_inGameScene != nullptr, "Please assign InGameScene");
	if (_inGameScene == nullptr)
	{
		return false;
	}

	CheckVictory(contact);
	CheckGameOver(contact);
	CheckGettingKey(contact);
	CheckGettingCoin(contact);

	return true;
}

bool ContactHandler::OnContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve)
{
	return true;
}

void ContactHandler::OnContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve)
{
}

void ContactHandler::OnContactSeparate(cocos2d::PhysicsContact& contact)
{
}

void ContactHandler::CheckVictory(cocos2d::PhysicsContact& contact)
{
	const auto categoryA = contact.getShapeA()->getCategoryBitmask();
	const auto categoryB = contact.getShapeB()->getCategoryBitmask();

	if (categoryA == GOAL_CATEGORY_BITMASK || categoryB == GOAL_CATEGORY_BITMASK)
	{
		if (!_inGameScene->_level->Locked)
		{
			_inGameScene->ShowVictory();
		}
	}
}

void ContactHandler::CheckGameOver(cocos2d::PhysicsContact& contact)
{
	const auto categoryA = contact.getShapeA()->getCategoryBitmask();
	const auto categoryB = contact.getShapeB()->getCategoryBitmask();

	if ((categoryA == OBSTACLE_DANGER && categoryB == PLAYER_CATEGORY_BITMASK)
		|| (categoryB == OBSTACLE_DANGER && categoryA == PLAYER_CATEGORY_BITMASK))
	{
		_inGameScene->ShowGameOver();
	}
}

void ContactHandler::CheckGettingKey(cocos2d::PhysicsContact& contact)
{
	const auto nodeA = contact.getShapeA()->getBody()->getNode();
	const auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA == nullptr || nodeB == nullptr)
	{
		return;
	}

	if (nodeA->getName().compare("key") == 0)
	{
		nodeA->removeFromParent();
	}
	else if (nodeB->getName().compare("key") == 0)
	{
		nodeB->removeFromParent();
	}
	else
	{
		return;
	}

	_inGameScene->_level->Locked = false;
	_inGameScene->ShowKeyInScreenInfo();
#if USE_AUDIO_ENGINE
	cocos2d::AudioEngine::play2d("sfx/key.wav");
#endif
}

void ContactHandler::CheckGettingCoin(cocos2d::PhysicsContact& contact)
{
	const auto nodeA = contact.getShapeA()->getBody()->getNode();
	const auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA == nullptr || nodeB == nullptr)
	{
		return;
	}

	if (nodeA->getName().compare("coin") == 0)
	{
		nodeA->removeFromParent();
	}
	else if (nodeB->getName().compare("coin") == 0)
	{

		nodeB->removeFromParent();
	}
	else
	{
		return;
	}

	_inGameScene->IncreaseNumberOfCoin();
#if USE_AUDIO_ENGINE
	cocos2d::AudioEngine::play2d("sfx/coin.wav");
#endif
}