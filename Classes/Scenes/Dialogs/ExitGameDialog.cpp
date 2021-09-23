#include "ExitGameDialog.h"

#include "base/CCDirector.h"

#include "../MyCustomGUI.inl"

bool ExitGameDialog::init(const cocos2d::Vec2& boxPosition)
{
	if (!MyDialog::init("ui/dialog-exit-game.tmx", true))
	{
		return false;
	}

	_tiledMap->setPosition(boxPosition);

	AddCallbackToButton("yes", [this](cocos2d::Ref*)
	{
		cocos2d::Director::getInstance()->end();
		exit(0);
	});

	AddCallbackToButton("no", [this](cocos2d::Ref*)
	{
		Hide();
	});


	return true;
}
