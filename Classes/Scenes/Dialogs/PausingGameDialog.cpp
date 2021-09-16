#include "PausingGameDialog.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"

bool PausingGameDialog::init()
{
	if (!MyDialog::init("ui/dialog-pausing-game.tmx", true))
	{
		return false;
	}
	const auto winSize = cocos2d::Director::getInstance()->getWinSize();

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