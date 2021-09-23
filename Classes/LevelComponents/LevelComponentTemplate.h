#pragma once

#include "2d/CCSprite.h"
#include "base/CCValue.h"
#include "json/document.h"

namespace cocos2d
{
class Action;
class Animate;
class Animation;
}

class LevelComponentTemplate : public cocos2d::Sprite
{
public:
	bool init(cocos2d::ValueMap& value);


protected:
	virtual void SetBitmask(cocos2d::PhysicsBody*) const = 0;
	virtual cocos2d::Action* CreateAction() const { return nullptr; };
	virtual cocos2d::Animate* CreateAnimate() const;

	rapidjson::Document _document;

private:
	bool InitWithSpriteSource();
	bool InitWithAnimation();

	cocos2d::PhysicsBody* CreateBody();
	cocos2d::PhysicsBody* CreateBoxBody();
	cocos2d::PhysicsBody* CreateCircleBody();

	cocos2d::Animation* CreateAnimation() const;
};