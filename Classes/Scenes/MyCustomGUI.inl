#pragma once
#include "MyCustomGUI.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "Utils/StaticMethods.h"
#include "Utils/TMXUtil.h"
#include "ScreenLog/ScreenLog.h"
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
		gScreenLog->Log(LL_WARNING, "Loading `%s` file failed. Please check the path again", tmxPath.c_str());
		return false;
	}

	StaticMethods::RequireObjectNotNull<cocos2d::TMXTiledMap>(_tiledMap, [this](cocos2d::TMXTiledMap* tiledMap)
	{
		TMXUtil::RequireTMXObjectGroupNotFound(tiledMap, "buttons", [this](cocos2d::TMXObjectGroup* objGroup)
		{
			auto objects = objGroup->getObjects();
			for (auto& object : objects)
			{
				auto value = object.asValueMap();
				auto button = MakeButton(value);
				_tiledMap->addChild(button);
			}
		});
	});

	StaticMethods::RequireObjectNotNull<cocos2d::TMXTiledMap>(_tiledMap, [this](cocos2d::TMXTiledMap* tiledMap)
	{
		TMXUtil::RequireTMXObjectGroupNotFound(tiledMap, "texts", [this](cocos2d::TMXObjectGroup* objGroup)
		{
			auto objects = objGroup->getObjects();
			for (auto& object : objects)
			{
				auto value = object.asValueMap();
				auto text = MakeText(value);
				_tiledMap->addChild(text);
			}
		});
	});
	

	return true;
}

template<class TGUI>
inline void MyCustomGUI<TGUI>::AddCallbackToButton(const std::string& name, const std::function<void(cocos2d::Ref*)>& callback)
{
	if (callback)
	{
		if (auto node = _tiledMap->getChildByName(name))
		{
			if (auto button = dynamic_cast<cocos2d::ui::Button*>(node))
			{
				button->addClickEventListener(callback);
			}
			else
			{
				gScreenLog->Log(LL_WARNING, "%s is not Button type", name.c_str());
			}
		}
		else
		{
			gScreenLog->Log(LL_WARNING, "%s not found", name.c_str());
		}
	}
}

template<class TGUI>
inline void MyCustomGUI<TGUI>::SetTextContent(const std::string& name, const std::string& content)
{
	if (auto node = _tiledMap->getChildByName(name))
	{
		if (auto text = dynamic_cast<cocos2d::ui::Text*>(node))
		{
			text->setString(content);
		}
		else
		{
			gScreenLog->Log(LL_WARNING, "%s is not Text type", name.c_str());
		}
	}
	else
	{
		gScreenLog->Log(LL_WARNING, "%s not found", name.c_str());
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

	auto text = cocos2d::ui::Text::create("<text>", fontName, fontSize);
	text->setName(name);
	text->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	text->setColor(cocos2d::Color3B::BLACK);
	text->setPosition(Vec2(x, y));

	return text;
}

template<class TGUI>
inline cocos2d::ui::Button* MyCustomGUI<TGUI>::MakeButtonFromTiledMap(const std::string& objectGroupName, const std::string& objectName)
{
	cocos2d::ui::Button* button = nullptr;
	StaticMethods::RequireObjectNotNull<cocos2d::TMXTiledMap>(_tiledMap, [this, objectGroupName, objectName, &button](cocos2d::TMXTiledMap* tiledMap)
	{
		TMXUtil::RequireTMXObjectGroupNotFound(tiledMap, objectGroupName, [this, objectName, &button](cocos2d::TMXObjectGroup* objGroup)
		{
			TMXUtil::RequireTMXObjectNotFound(objGroup, objectName, [this, &button](cocos2d::ValueMap& value)
			{
				button = MakeButton(value);
			});
		});
	});

	return button;
}
