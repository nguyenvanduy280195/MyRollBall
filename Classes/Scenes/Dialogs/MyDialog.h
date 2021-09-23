#pragma once

#include "../MyCustomGUI.h"
#include "../TouchSwallower.h"
#include "2d/CCLayer.h"

namespace cocos2d
{
class EventListenerTouchOneByOne;
}

class MyDialog : public MyCustomGUI<TouchSwallower<cocos2d::Layer>>
{
	using Super = MyCustomGUI<TouchSwallower<cocos2d::Layer>>;
public:
	cocos2d::TMXTiledMap* GetTiledMap() { return _tiledMap; }
	virtual void Show();
	virtual void Hide();

	std::function<void()> OnHidden;


protected:
	bool init(const std::string& tmxPath, bool backgroundEnabled = false);
};