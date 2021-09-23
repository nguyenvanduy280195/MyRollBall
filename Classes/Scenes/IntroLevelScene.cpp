#include "IntroLevelScene.h"
#include "ui/UIText.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"
#include "2d/CCTransition.h"
#include "InGameScene.h"
#include "Dialogs/VictoryDialog.h"
#include "Utils/StaticMethods.h"
#include "ScreenLog/ScreenLog.h"
#include "ComingSoonScene.h"

#include "MyCustomGUI.inl"

// TODO convert this tmx file

bool IntroLevelScene::init() { return init(1); }

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
	if (_timer > _timeout)
	{
		unscheduleUpdate();
		if (auto scene = InGameScene::CreateScene(_level))
		{
			cocos2d::Director::getInstance()->replaceScene(scene);
		}
		else
		{
			auto comingSoonScene = Cocos2dCreator::CreateNode<ComingSoonScene>();
			cocos2d::Director::getInstance()->replaceScene(comingSoonScene);
		}

		
	}
	_timer += dt;
}
