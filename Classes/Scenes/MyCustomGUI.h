#pragma once
#include "base/CCValue.h"

namespace cocos2d
{
class TMXTiledMap;
}

template<class TGUI>
class MyCustomGUI : public TGUI
{
protected:
	virtual bool init(const std::string& tmxPath);

	void AddCallbackToButton(const std::string& name, const std::function<void(cocos2d::Ref*)>& callback);
	void SetTextContent(const std::string& name, const std::string& content);

	template<class TMyWidget>
	TMyWidget* GetMyWidgetByName(const std::string& name) const;

	// true if loading successfully button
	template<class TMyWidget>
	bool LoadMyWidgetByName(TMyWidget*& o_myWidget, const std::string& name);

	cocos2d::TMXTiledMap* _tiledMap;
};
