#include "VictoryDialog.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "ui/UIText.h"
#include "ui/UIVBox.h"
#include "ui/UIRelativeBox.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXObjectGroup.h"
#include "Utils/TMXUtil.h"
#include "Utils/StaticMethods.h"


#include "../MyCustomGUI.inl"


using Size = cocos2d::Size;
using Vec2 = cocos2d::Vec2;

bool VictoryDialog::init()
{
	if (!MyCustomLayer::init("ui/dialog-victory.tmx"))
	{
		return false;
	}

	auto cameraPosition = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPosition);
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

void VictoryDialog::SetOnHomeButtonPressed(const std::function<void(cocos2d::Ref*)>& onButtonPressed)
{
	AddCallbackToButton("home", onButtonPressed);
}
