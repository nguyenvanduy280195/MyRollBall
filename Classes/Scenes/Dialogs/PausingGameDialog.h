#pragma once

#include "MyDialog.h"

class PausingGameDialog : public MyDialog
{
public:
	bool init(const cocos2d::Vec2& position = cocos2d::Vec2::ZERO);
	std::function<void()> OnEventPaused;
	std::function<void()> OnEventUnpaused;
};