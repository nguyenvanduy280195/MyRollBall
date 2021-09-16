#include "OptionsDialog.h"
#include "../MyCustomGUI.inl"

#include "ui/UILayout.h"

bool OptionsDialog::init(const cocos2d::Vec2& position)
{
	if (!MyDialog::init("ui/dialog-option.tmx", true))
	{
		return false;
	}

	_tiledMap->setPosition(position);

	AddCallbackToButton("close", [this](cocos2d::Ref*) { setVisible(false); });

	return true;
}
