#pragma once

#include "2d/CCScene.h"
#include "2d/CCLayer.h"
#include "base/CCValue.h"
#include "MyCustomGUI.h"

namespace cocos2d
{
class TMXTiledMap;

namespace ui
{
class Button;
}
}

class MainMenuScene : public MyCustomGUI<cocos2d::Scene>
{
public:
	bool init();

private:
	const cocos2d::Vec2 GetBoxPosition() const;

	void HideAllDialogs();


	std::function<void(cocos2d::Ref*)> MakeMenuButtonClicked(class MyDialog* dialog);

	class MyDialog* _choosingLevelDialog;
	class MyDialog* _optionsDialog;
	class MyDialog* _creditsDialog;
	class MyDialog* _exitGameDialog;
};
