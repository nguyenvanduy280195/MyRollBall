#pragma once

#include "LevelComponentTemplate.h"

class CoinLevelComponent : public LevelComponentTemplate
{
private:
	void SetBitmask(cocos2d::PhysicsBody*) const override;
	cocos2d::Action* CreateAction() const override;
};