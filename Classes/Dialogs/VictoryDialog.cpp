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


using Size = cocos2d::Size;
using Vec2 = cocos2d::Vec2;
using RelativeLayoutParameter = cocos2d::ui::RelativeLayoutParameter;
using RelativeAlign = cocos2d::ui::RelativeLayoutParameter::RelativeAlign;
using LinearGravity = cocos2d::ui::LinearLayoutParameter::LinearGravity;
using LinearLayoutParameter = cocos2d::ui::LinearLayoutParameter;
using RelativeBox = cocos2d::ui::RelativeBox;
using VBox = cocos2d::ui::VBox;
using Margin = cocos2d::ui::Margin;

bool VictoryDialog::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	auto cameraPostion = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPostion);

	auto tiledMap = cocos2d::TMXTiledMap::create("levels/dialog-victory.tmx");
	tiledMap->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addChild(tiledMap);

	RequireObjectGroupNotFound(tiledMap, "buttons", [this, tiledMap](cocos2d::TMXObjectGroup* objectGroup)
	{
		RequireObjectNotFound(objectGroup, "next", [this, tiledMap](cocos2d::ValueMap& value)
		{
			_nextButton = MakeButton(value);
			RequireNotNull(_nextButton, [tiledMap](cocos2d::Node* button)
			{
				tiledMap->addChild(button);
			});
		});

		RequireObjectNotFound(objectGroup, "home", [this, tiledMap](cocos2d::ValueMap& value)
		{
			_homeButton = MakeButton(value);
			RequireNotNull(_homeButton, [tiledMap](cocos2d::Node* button)
			{
				tiledMap->addChild(button);
			});
		});
	});

	RequireObjectGroupNotFound(tiledMap, "texts", [this, tiledMap](cocos2d::TMXObjectGroup* objectGroup)
	{
		RequireObjectNotFound(objectGroup, "level", [this, tiledMap](cocos2d::ValueMap& value)
		{
			_levelText = MakeText(value);
			RequireNotNull(_levelText, [tiledMap](cocos2d::Node* text)
			{
				tiledMap->addChild(text);
			});
		});

		RequireObjectNotFound(objectGroup, "time", [this, tiledMap](cocos2d::ValueMap& value)
		{
			_timeText = MakeText(value);
			RequireNotNull(_timeText, [tiledMap](cocos2d::Node* text)
			{
				tiledMap->addChild(text);
			});
		});

		RequireObjectNotFound(objectGroup, "best-time", [this, tiledMap](cocos2d::ValueMap& value)
		{
			_bestTimeText = MakeText(value);
			RequireNotNull(_bestTimeText, [tiledMap](cocos2d::Node* text)
			{
				tiledMap->addChild(text);
			});
		});
	});

	return true;
}

void VictoryDialog::SetLevelTextContent(const std::string& content)
{
	_levelText->setString(content);
}

void VictoryDialog::SetTimeTextContent(const std::string& content)
{
	_timeText->setString(content);
}

void VictoryDialog::SetBestTimeTextContent(const std::string& content)
{
	_bestTimeText->setString(content);
}

void VictoryDialog::SetOnNextButtonPressed(const std::function<void(cocos2d::Ref*)>& onButtonPressed)
{
	_nextButton->addClickEventListener(onButtonPressed);
}

void VictoryDialog::SetOnHomeButtonPressed(const std::function<void(cocos2d::Ref*)>& onButtonPressed)
{
	_homeButton->addClickEventListener(onButtonPressed);
}

cocos2d::ui::Button* VictoryDialog::MakeButton(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto title = value["title"].asString();
	const auto titleFontName = value["font-name"].asString();
	const auto titleFontSize = value["font-size"].asFloat();
	const auto normalImage = value["src-normal"].asString();
	const auto pressedImage = value["src-pressed"].asString();
	const auto disabledImage = value["src-disabled"].asString();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();

	auto button = cocos2d::ui::Button::create(normalImage, pressedImage, disabledImage);
	button->setPosition(Vec2(x, y));
	button->setTitleText(title);
	button->setTitleFontName(titleFontName);
	button->setTitleFontSize(titleFontSize);

	return button;
}

cocos2d::ui::Text* VictoryDialog::MakeText(cocos2d::ValueMap& value)
{
	const auto fontName = value["font-name"].asString();
	const auto fontSize = value["font-size"].asFloat();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();

	auto text = cocos2d::ui::Text::create("<text>", fontName, fontSize);
	text->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	text->setColor(cocos2d::Color3B::BLACK);
	text->setPosition(Vec2(x, y));

	return text;
}

void VictoryDialog::RequireObjectGroupNotFound(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::TMXObjectGroup*)>& action)
{
	auto textsObjectGroup = tiledMap->getObjectGroup(name);
	if (textsObjectGroup)
	{
		action(textsObjectGroup);
	}
	else
	{
		CCLOG("`%s` not found", name.c_str());
	}
}

void VictoryDialog::RequireObjectNotFound(cocos2d::TMXObjectGroup* objectGroup, const std::string& name, const std::function<void(cocos2d::ValueMap& value)>& action)
{
	auto valueMap = objectGroup->getObject(name);
	if (valueMap != cocos2d::ValueMap())
	{
		action(valueMap);
	}
	else
	{
		CCLOG("`%s` not found", name.c_str());
	}
}

void VictoryDialog::RequireNotNull(cocos2d::Node* node, const std::function<void(cocos2d::Node*)>& action)
{
	if (node)
	{
		action(node);
	}
	else
	{
		CCLOG("node is nullptr");
	}
}