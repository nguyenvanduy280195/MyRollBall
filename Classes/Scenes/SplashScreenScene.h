#pragma once

#include "MyCustomGUI.h"
#include "2d/CCScene.h"

namespace cocos2d
{
class FiniteTimeAction;
}

class SplashScreenScene : public MyCustomGUI<cocos2d::Scene>
{
	using Super = MyCustomGUI<cocos2d::Scene>;
public:
	bool init(const std::function<void()>& onDone);

};