#include "ExitGameLayer.h"


bool ExitGameLayer::init()
{
	if (!MyCustomLayer::init("ui/layer-exit-game.tmx"))
	{
		return false;
	}

	AddCallbackToButton("yes", [this](cocos2d::Ref*)
	{
		exit(0);
	});

	AddCallbackToButton("no", [this](cocos2d::Ref*)
	{
		setVisible(false);
	});


	return true;
}
