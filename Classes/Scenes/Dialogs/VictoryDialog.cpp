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
#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"
#include "../MyCustomGUI.inl"

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


	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "positions", [this](cocos2d::TMXObjectGroup* objectGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objectGroup, "victory", [this](cocos2d::ValueMap& value)
		{
			auto x = value["x"].asFloat();
			auto y = value["y"].asFloat();
			auto src = value["src"].asString();
			auto scaleFactor0 = value["scale-factor-0"].asFloat();
			auto scaleFactor1 = value["scale-factor-1"].asFloat();
			auto duration = value["duration"].asFloat();

			auto sprite = cocos2d::Sprite::create(src);
			sprite->setPosition(x, y);
			sprite->setScale(scaleFactor0);

			auto action = cocos2d::ScaleTo::create(duration, scaleFactor1);
			sprite->runAction(action);

			_tiledMap->addChild(sprite);
		});
	});

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
