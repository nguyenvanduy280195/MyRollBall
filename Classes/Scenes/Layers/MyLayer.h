#pragma once

#include "../MyCustomGUI.h"
#include "2d/CCLayer.h"

class MyLayer : public MyCustomGUI<cocos2d::Layer>
{
	using Super = MyCustomGUI<cocos2d::Layer>;

protected:
	bool init(const std::string& tmxPath);
};