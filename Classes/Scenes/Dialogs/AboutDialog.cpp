#include "AboutDialog.h"

#include "../MyCustomGUI.inl"

#include "ScreenLog/ScreenLog.h"

bool AboutDialog::init()
{
	if (!MyDialog::init("ui/layer-about.tmx"))
	{
		return false;
	}

	AddCallbackToButton("close", [this](cocos2d::Ref*)
	{
		setVisible(false);
	});

	//_screenLog->Info(__FUNCTION__);


	return true;
}
