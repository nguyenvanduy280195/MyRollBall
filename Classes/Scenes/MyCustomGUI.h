#pragma once
#include "base/CCValue.h"
#include "2d/CCNode.h"
#include "2d/CCLayer.h"
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
protected:
	virtual bool init(const std::string& tmxPath);

	void AddCallbackToButton(const std::string& name, const std::function<void(cocos2d::Ref*)>& callback);
	void SetTextContent(const std::string& name, const std::string& content);

	cocos2d::ui::Button* GetButtonByName(const std::string& name) const;

	cocos2d::ui::Button* MakeButton(cocos2d::ValueMap& value);
	cocos2d::ui::Text* MakeText(cocos2d::ValueMap& value);
	

	cocos2d::TMXTiledMap* _tiledMap;

};
