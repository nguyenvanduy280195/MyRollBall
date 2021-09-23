#include "ScreenInfoLayer.h"
#include "../MyCustomGUI.inl"

bool ScreenInfoLayer::init() { return MyLayer::init("ui/ingamescene-screen-info-horizontal.tmx"); }

float ScreenInfoLayer::GetHeight() const { return _tiledMap->getContentSize().height; }

void ScreenInfoLayer::SetCoinText(const std::string& text) { SetTextContent("coin", text); }

void ScreenInfoLayer::AddPauseButtonCallback(const std::function<void(cocos2d::Ref*)>& callback) { AddCallbackToButton("pause", callback); }

void ScreenInfoLayer::ShowKey()
{
	if (auto child = _tiledMap->getChildByName("key"))
	{
		child->setVisible(true);
	}
}
