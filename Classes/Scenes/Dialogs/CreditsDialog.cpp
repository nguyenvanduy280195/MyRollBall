#include "CreditsDialog.h"
#include "../MyCustomGUI.inl"

bool CreditsDialog::init(const cocos2d::Vec2& position)
{
	if (!MyDialog::init("ui/dialog-credits.tmx", true))
	{
		return false;
	}

	_tiledMap->setPosition(position);

	AddCallbackToButton("close", [this](cocos2d::Ref*){ Hide(); });

	return true;
}
