#pragma once
#include "MyCustomGUI.h"

#include "ActionCreator.h"

#include "2d/CCTMXTiledMap.h"
#include "2d/CCActionInterval.h"

#include "Utils/StaticMethods.h"
#include "Utils/TMXUtil.h"
#include "Utils/Cocos2dCreator.h"

#include "MyComponents/MyButton.h"
#include "MyComponents/MyImageView.h"
#include "MyComponents/MyText.h"

#define USE_AUDIO_ENGINE CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

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
			auto button = Cocos2dCreator::CreateNode<MyButton>(value);
			button->SetAction(ActionCreator(_tiledMap).Create(value["action.name"].asString()));
			_tiledMap->addChild(button);


		});

		TMXUtil::ForeachAllObjectsInObjectGroup(_tiledMap, "texts", [this](cocos2d::ValueMap& value)
		{
			auto text = Cocos2dCreator::CreateNode<MyText>(value);
			text->SetAction(ActionCreator(_tiledMap).Create(value["action.name"].asString()));
			_tiledMap->addChild(text);
		});

		TMXUtil::ForeachAllObjectsInObjectGroup(_tiledMap, "images", [this](cocos2d::ValueMap& value)
		{
			auto image = Cocos2dCreator::CreateNode<MyImageView>(value);
			image->SetAction(ActionCreator(_tiledMap).Create(value["action.name"].asString()));
			_tiledMap->addChild(image);
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
				button->addClickEventListener([callback](cocos2d::Ref* ref)
				{
					if (callback)
					{
						callback(ref);
					}

				#if USE_AUDIO_ENGINE
					cocos2d::AudioEngine::play2d("sfx/select.wav");
				#endif
					});
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
template<class TMyWidget>
inline TMyWidget* MyCustomGUI<TGUI>::GetMyWidgetByName(const std::string& name) const
{
	if (auto child = _tiledMap->getChildByName(name))
	{
		return dynamic_cast<TMyWidget*>(child);
	}
	return nullptr;
}

template<class TGUI>
template<class TMyWidget>
inline bool MyCustomGUI<TGUI>::LoadMyWidgetByName(TMyWidget*& o_myWidget, const std::string& name)
{
	if (o_myWidget = GetMyWidgetByName<TMyWidget>(name))
	{
		return true;
	}

	CCLOG(cocos2d::StringUtils::format("the TMyWidget's name is %s that is not found", name.c_str()).c_str());
	return false;
}
