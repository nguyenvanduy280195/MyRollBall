#include "ExitGameDialog.h"
#include "../MyCustomGUI.inl"
#include "base/CCDirector.h"

bool ExitGameDialog::init()
{
	if (!MyDialog::init("ui/layer-exit-game.tmx"))
	{
		return false;
	}

	AddCallbackToButton("yes", [this](cocos2d::Ref*)
	{
		cocos2d::Director::getInstance()->end();
		exit(0);
	});

	AddCallbackToButton("no", [this](cocos2d::Ref*)
	{
		setVisible(false);
	});


	return true;
}
