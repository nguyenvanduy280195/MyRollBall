#include "KeyboardHandler.h"
#include "base/CCEventListenerKeyboard.h"
#include "2d/CCNode.h"
#include "base/CCEventDispatcher.h"

KeyboardHandler::KeyboardHandler(cocos2d::Node* owner)
{
	auto keyListener = cocos2d::EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event*)
	{
		pressingKeys[keyCode] = true;
	};
	keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event*)
	{
		pressingKeys[keyCode] = false;
	};


	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, owner);
}