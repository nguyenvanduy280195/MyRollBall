#pragma once

#include "base/CCValue.h"

template<class Base>
class MyWidgetTemplate : public Base
{
};

namespace cocos2d
{
class FiniteTimeAction;
class TMXTiledMap;
}

class ActionCreator
{
public:
	ActionCreator(cocos2d::TMXTiledMap* tiledMap);

	cocos2d::FiniteTimeAction* Create(const std::string& actionName);
	cocos2d::FiniteTimeAction* CreateScaleToAction(cocos2d::ValueMap& value) const;
	cocos2d::FiniteTimeAction* CreateSequenceAction(cocos2d::ValueMap& value);
	cocos2d::FiniteTimeAction* CreateFadeInAction(cocos2d::ValueMap& value) const;
	cocos2d::FiniteTimeAction* CreateFadeOutAction(cocos2d::ValueMap& value) const;

	cocos2d::TMXTiledMap* _tiledMap;
};
