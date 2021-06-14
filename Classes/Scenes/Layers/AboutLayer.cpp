#include "AboutLayer.h"
#include "../MyCustomGUI.inl"

bool AboutLayer::init()
{
	if (!MyCustomLayer::init("ui/layer-about.tmx"))
	{
		return false;
	}

	AddCallbackToButton("close", [this](cocos2d::Ref*)
	{
		setVisible(false);
	});

	return true;
}
