#pragma once

#include "2d/CCScene.h"
#include "MyCustomGUI.h"

class IntroLevelScene : public MyCustomGUI<cocos2d::Scene>
{
public:
	bool init();
	bool init(int level);
	void onEnterTransitionDidFinish() override;
private:
	void update(float) override;

	float _timeout = 2.f;
	float _timer = 0;
	int _level = 1;
};
