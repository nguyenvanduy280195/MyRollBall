#include "PausingGameDialog.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"

bool PausingGameDialog::init()
{
	if (!MyDialog::init("ui/dialog-pausing-game.tmx"))
	{
		return false;
	}
	const auto winSize = cocos2d::Director::getInstance()->getWinSize();

	auto background = cocos2d::ui::Layout::create();
	background->setContentSize(winSize);
	background->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	background->setBackGroundColor(cocos2d::Color3B::BLACK);
	background->setOpacity(100);
	addChild(background,-1);

	_tiledMap->setPosition(0.5f * winSize);

	AddCallbackToButton("back", [this](cocos2d::Ref*)
	{
		cocos2d::Director::getInstance()->popScene();

		if (OnEventPaused)
		{
			OnEventPaused();
		}
	});

	AddCallbackToButton("option", [this](cocos2d::Ref*)
	{
		
	});

	AddCallbackToButton("resume", [this](cocos2d::Ref*)
	{
		Hide();

		if (OnEventUnpaused)
		{
			OnEventUnpaused();
		}
	});

	return true;
}