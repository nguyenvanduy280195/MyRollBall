#include "MyDialog.h"

#include "../MyCustomGUI.inl"

//#include "ScreenLog/ScreenLog.h"

bool MyDialog::init(const std::string& tmxPath)
{
	if (!MyCustomGUI<cocos2d::Layer>::init(tmxPath))
	{
		return false;
	}

	_tiledMap->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	return true;
}