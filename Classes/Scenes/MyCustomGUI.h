#pragma once
#include "base/CCValue.h"
#include "2d/CCNode.h"
#include "2d/CCLayer.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "2d/CCTMXTiledMap.h"

namespace cocos2d
{
class TMXTiledMap;
}

namespace cocos2d::ui
{
class Button;
class Text;
}

template<class TGUI>
class MyCustomGUI : public TGUI
{
	//static_assert(std::is_base_of<cocos2d::Node, TGUI>::value, "T must inherit from SomeBase");
protected:
	virtual bool init(const std::string& tmxPath);

	void AddCallbackToButton(const std::string& name, const std::function<void(cocos2d::Ref*)>& callback);
	void SetTextContent(const std::string& name, const std::string& content);

	cocos2d::ui::Button* MakeButton(cocos2d::ValueMap& value);
	cocos2d::ui::Text* MakeText(cocos2d::ValueMap& value);

	cocos2d::ui::Button* MakeButtonFromTiledMap(const std::string& objectGroupName, const std::string& objectName);

	cocos2d::TMXTiledMap* _tiledMap;
};

class MyCustomLayer : public MyCustomGUI<cocos2d::ui::Layout>
{
public:
	cocos2d::TMXTiledMap* GetTiledMap() { return _tiledMap; }

protected:
	bool init(const std::string& tmxPath) override
	{
		if (!MyCustomGUI::init(tmxPath))
		{
			return false;
		}

		_tiledMap->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		
		return true;
	}
};