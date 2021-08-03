#pragma once
#include "MyCustomGUI.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "Utils/StaticMethods.h"
#include "Utils/TMXUtil.h"
#include "2d/CCTMXTiledMap.h"


using Vec2 = cocos2d::Vec2;

template<class TGUI>
inline bool MyCustomGUI<TGUI>::init(const std::string& tmxPath)
{
	if (!TGUI::init())
	{
		return false;
	}

	if (_tiledMap = cocos2d::TMXTiledMap::create(tmxPath))
	{
		TGUI::addChild(_tiledMap);
	}
	else
	{
		return false;
	}

	if (_tiledMap)
	{
		TMXUtil::ForeachAllObjectsInObjectGroup(_tiledMap, "buttons", [this](cocos2d::ValueMap& value)
		{
			auto button = MakeButton(value);
			_tiledMap->addChild(button);
		});

		TMXUtil::ForeachAllObjectsInObjectGroup(_tiledMap, "texts", [this](cocos2d::ValueMap& value)
		{
			auto text = MakeText(value);
			_tiledMap->addChild(text);
		});
	}

	return true;
}

template<class TGUI>
inline void MyCustomGUI<TGUI>::AddCallbackToButton(const std::string& name, const std::function<void(cocos2d::Ref*)>& callback)
{
	if (callback)
	{
		if (auto child = _tiledMap->getChildByName(name))
		{
			if (auto button = dynamic_cast<cocos2d::ui::Button*>(child))
			{
				button->addClickEventListener(callback);
			}
		}
	}
}

template<class TGUI>
inline void MyCustomGUI<TGUI>::SetTextContent(const std::string& name, const std::string& content)
{
	if (auto child = _tiledMap->getChildByName(name))
	{
		if (auto text = dynamic_cast<cocos2d::ui::Text*>(child))
		{
			text->setString(content);
		}
	}
}

template<class TGUI>
inline cocos2d::ui::Button* MyCustomGUI<TGUI>::MakeButton(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	const auto src_normal = value["src-normal"].asString();
	const auto src_pressed = value["src-pressed"].asString();
	const auto src_disabled = value["src-disabled"].asString();
	const auto title = value["title"].asString();
	const auto fontName = value["font-name"].asString();
	const auto fontSize = value["font-size"].asFloat();

	auto button = cocos2d::ui::Button::create(src_normal, src_pressed, src_disabled);
	button->setName(name);
	button->setTitleText(title);
	button->setTitleFontName(fontName);
	button->setTitleFontSize(fontSize);
	button->setPosition(Vec2(x, y));

	return button;
}

template<class TGUI>
inline cocos2d::ui::Text* MyCustomGUI<TGUI>::MakeText(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto fontName = value["font-name"].asString();
	const auto fontSize = value["font-size"].asFloat();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	const auto anchorPointX = value["anchor-point-x"].asFloat();
	const auto anchorPointY = value["anchor-point-y"].asFloat();
	const auto color = StaticMethods::MakeColor3BFromHex(value["color"].asString());
	auto str = value["text"].asString();

	if (str.empty())
	{
		str = "<text>";
	}

	auto text = cocos2d::ui::Text::create(str, fontName, fontSize);
	
	text->setAnchorPoint(Vec2(anchorPointX, anchorPointY));
	text->setName(name);
	text->setColor(color);
	text->setPosition(Vec2(x, y));

	if (value["enableShadow"].asBool())
	{
		text->enableShadow();
	}

	return text;
}
