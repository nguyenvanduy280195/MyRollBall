#include "VictoryDialog.h"

#include "../MyCustomGUI.inl"

#include "base/CCDirector.h"

#include "2d/CCCamera.h"

using Size = cocos2d::Size;
using Vec2 = cocos2d::Vec2;

bool VictoryDialog::init()
{
	if (!MyDialog::init("ui/dialog-victory.tmx"))
	{
		return false;
	}

	auto cameraPosition = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPosition);

	if (auto victory = GetMyWidgetByName<MyImageView>("victory"))
	{
		if (auto action = victory->GetAction())
		{
			victory->runAction(action);
		}
	}

	return true;
}

void VictoryDialog::SetLevelTextContent(const std::string& content)
{
	SetTextContent("level", content);
}

void VictoryDialog::SetTimeTextContent(const std::string& content)
{
	SetTextContent("time", content);
}

void VictoryDialog::SetBestTimeTextContent(const std::string& content)
{
	SetTextContent("best-time", content);
}

void VictoryDialog::SetOnNextButtonPressed(const std::function<void(cocos2d::Ref*)>& onButtonPressed)
{
	AddCallbackToButton("next", onButtonPressed);
}

void VictoryDialog::SetOnMainMenuButtonPressed(const std::function<void(cocos2d::Ref*)>& onButtonPressed)
{
	AddCallbackToButton("main-menu", onButtonPressed);
}
