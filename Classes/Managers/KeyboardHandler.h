#pragma once
#include "base/CCRef.h"
#include "base/CCEventKeyboard.h"
#include <unordered_map>

class KeyboardHandler : public cocos2d::Ref
{
	using KeyCode = cocos2d::EventKeyboard::KeyCode;
public:
	std::unordered_map<KeyCode, bool> pressingKeys;

	void Setup(cocos2d::Node*);
};
