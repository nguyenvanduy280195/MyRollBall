#include "OptionsDialog.h"
#include "../MyCustomGUI.inl"

bool OptionsDialog::init(const cocos2d::Vec2& boxPosition)
{
	if (!MyDialog::init("ui/dialog-option.tmx"))
	{
		return false;
	}

	_tiledMap->setPosition(boxPosition);

	return true;
}
