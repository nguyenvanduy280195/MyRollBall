#include "KeyboardHandler.h"
#include "base/CCEventListenerKeyboard.h"
#include "2d/CCNode.h"
#include "base/CCEventDispatcher.h"

void KeyboardHandler::Setup(cocos2d::Node* node)
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


	node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, node);
}