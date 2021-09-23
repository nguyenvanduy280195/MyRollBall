#include "ActionCreator.h"
#include "Utils/TMXUtil.h"
#include "2d/CCActionInterval.h"

ActionCreator::ActionCreator(cocos2d::TMXTiledMap* tiledMap)
{
	_tiledMap = tiledMap;
}

cocos2d::FiniteTimeAction* ActionCreator::Create(const std::string& name)
{
	cocos2d::FiniteTimeAction* action = nullptr;

	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "actions", [this, &action, name](cocos2d::TMXObjectGroup* objectGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objectGroup, name, [this, &action, name](cocos2d::ValueMap& value)
		{
			if (cocos2d::StringUtils::contain(name, "Sequence"))
			{
				action = CreateSequenceAction(value);
			}
			else if (cocos2d::StringUtils::contain(name, "FadeIn"))
			{
				action = CreateFadeInAction(value);
			}
			else if (cocos2d::StringUtils::contain(name, "FadeOut"))
			{
				action = CreateFadeOutAction(value);
			}
			else if (cocos2d::StringUtils::contain(name, "ScaleTo"))
			{
				action = CreateScaleToAction(value);
			}
		});
	});
	return action;
}

cocos2d::FiniteTimeAction* ActionCreator::CreateScaleToAction(cocos2d::ValueMap& value) const
{
	auto duration = value["duration"].asFloat();
	auto scaleFactorX = value["scale-factor-x"].asFloat();
	auto scaleFactorY = value["scale-factor-y"].asFloat();

	return cocos2d::ScaleTo::create(duration, scaleFactorX, scaleFactorY);
}

cocos2d::FiniteTimeAction* ActionCreator::CreateSequenceAction(cocos2d::ValueMap& value)
{
	int nActions = value["nActions"].asInt();
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
	for (int i = 0; i < nActions; i++)
	{
		auto actionName = cocos2d::StringUtils::format("action%d", i);
		auto actioni = value[actionName].asString();
		auto action = Create(actioni);
		actions.pushBack(action);
	}

	return cocos2d::Sequence::create(actions);
}

cocos2d::FiniteTimeAction* ActionCreator::CreateFadeInAction(cocos2d::ValueMap& value) const
{
	auto duration = value["duration"].asFloat();
	return cocos2d::FadeIn::create(duration);
}

cocos2d::FiniteTimeAction* ActionCreator::CreateFadeOutAction(cocos2d::ValueMap& value) const
{
	auto duration = value["duration"].asFloat();
	return cocos2d::FadeOut::create(duration);
}