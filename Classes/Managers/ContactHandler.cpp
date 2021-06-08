#include "ContactHandler.h"

#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsShape.h"
#include "base/CCEventDispatcher.h"

#include "Scenes/InGameScene.h"
#include "ScreenLog/ScreenLog.h"
#include "Level.h"
#include "Player.h"
#include "Bitmask.h"

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
	const auto categoryA = contact.getShapeA()->getCategoryBitmask();
	const auto categoryB = contact.getShapeB()->getCategoryBitmask();

	const auto nodeA = contact.getShapeA()->getBody()->getNode();
	const auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (categoryA == GOAL_CATEGORY_BITMASK || categoryB == GOAL_CATEGORY_BITMASK)
	{
		if (!_inGameScene->_level->Locked)
		{
			_inGameScene->GetScreenLog()->AddLog("Win");
			_inGameScene->NextLevel();
		}
		else
		{
			_inGameScene->GetScreenLog()->AddLog("The door is locked. Please find the key");
		}
	}

	if (categoryA == KEY_CATEGORY_BITMASK)
	{
		_inGameScene->GetScreenLog()->AddLog("Congratulations!!! You found the key. Please go to the door");
		_inGameScene->_level->Locked = false;
		nodeB->removeFromParent();
	}
	else if(categoryB == KEY_CATEGORY_BITMASK)
	{
		_inGameScene->GetScreenLog()->AddLog("Congratulations!!! You found the key. Please go to the door");
		_inGameScene->_level->Locked = false;
		nodeB->removeFromParent();
	}

	return true;
}

bool ContactHandler::OnContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve)
{
	//auto tagA = contact.getShapeA()->getTag();
	//auto tagB = contact.getShapeB()->getTag();
	//if (tagA == _inGameScene->_level->nameTagMap["goal"] && tagB == _inGameScene->_level->nameTagMap["player"] ||
	//	tagB == _inGameScene->_level->nameTagMap["goal"] && tagA == _inGameScene->_level->nameTagMap["player"])
	//{
	//	return false;
	//}


	return true;
}

void ContactHandler::OnContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve)
{
}

void ContactHandler::OnContactSeparate(cocos2d::PhysicsContact& contact)
{
}