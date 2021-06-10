#include "IntroLevelScene.h"
#include "ui/UIText.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"
#include "2d/CCTransition.h"
#include "InGameScene.h"
#include "Dialogs/VictoryDialog.h"

bool IntroLevelScene::init(int level)
{
	if (!cocos2d::Scene::init())
	{
		return false;
	}
	_level = level;
	auto winSize = cocos2d::Director::getInstance()->getWinSize();

	auto background = cocos2d::ui::Layout::create();
	background->setContentSize(winSize);
	background->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	background->setBackGroundColor(cocos2d::Color3B::GREEN);
	addChild(background);
	
	auto text = cocos2d::ui::Text::create(cocos2d::StringUtils::format("Level %d", _level),
										  "fonts/Marker Felt.ttf", 
										  50);
	text->setPosition(0.5f * winSize);
	addChild(text);

	return true;
}

void IntroLevelScene::onEnterTransitionDidFinish()
{
	cocos2d::Scene::onEnterTransitionDidFinish();

	scheduleUpdate();
}

void IntroLevelScene::update(float dt)
{
	if (_timer > _timerLimit)
	{
		unscheduleUpdate();
		auto scene = InGameScene::CreateScene(_level);
		auto sceneWithTransition = cocos2d::TransitionFadeDown::create(1, scene);
		cocos2d::Director::getInstance()->replaceScene(sceneWithTransition);
	}
	_timer += dt;
}
