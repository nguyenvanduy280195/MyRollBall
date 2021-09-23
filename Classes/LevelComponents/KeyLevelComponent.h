#pragma once
#include "LevelComponentTemplate.h"
class KeyLevelComponent : public LevelComponentTemplate
{
private:
	void SetBitmask(cocos2d::PhysicsBody*) const override;
};