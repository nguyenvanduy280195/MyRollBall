#pragma once

#include "2d/CCScene.h"

class IntroLevelScene : public cocos2d::Scene
{
public:
	bool init() override { return init(1); }
	bool init(int level);
	void onEnterTransitionDidFinish() override;
private:
	void update(float) override;

	float _timerLimit = 2.f;
	float _timer = 0;
	int _level = 1;
};
