#include "PausingGameDialog.h"
#include "base/CCDirector.h"
#include "2d/CCTMXTiledMap.h"

bool PausingGameDialog::init(const cocos2d::Vec2& position)
{
	if (!MyDialog::init("ui/dialog-pausing-game.tmx", true))
	{
		return false;
	}

	setPosition(position);

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