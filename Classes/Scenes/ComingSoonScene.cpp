#include "ComingSoonScene.h"
#include "base/CCDirector.h"
#include "MyCustomGUI.inl"


bool ComingSoonScene::init()
{
	if (!MyCustomGUI<cocos2d::Scene>::init("ui/scene-coming-soon.tmx"))
	{
		return false;
	}

	AddCallbackToButton("back", [](cocos2d::Ref*) { cocos2d::Director::getInstance()->popScene(); });

	if (auto child = _tiledMap->getChildByName("back"))
	{
		if (auto button = dynamic_cast<cocos2d::ui::Button*>(child))
		{
			const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
			const auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			const auto buttonSize = button->getContentSize();
			const auto x = visibleOrigin.x + buttonSize.width;
			const auto y = visibleOrigin.y + visibleSize.height - buttonSize.height;
			button->setPosition(cocos2d::Vec2(x, y));
		}
	}

	return true;
}