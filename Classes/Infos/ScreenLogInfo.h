#pragma once

#include "base/ccTypes.h"

struct ScreenLogInfo
{
	std::string fontFamily;
	float fontSize;
	cocos2d::Color3B textColor;
	float removingTimerLimit;
	int logCountLimit;
};