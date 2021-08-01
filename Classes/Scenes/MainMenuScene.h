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
	void TakeLayerToBoxPosition(class MyDialog* dialog);
	void SetAllLayersVisible(bool visible);

	std::function<void(cocos2d::Ref*)> MakePlayButtonClicked();
	std::function<void(cocos2d::Ref*)> MakeOptionsButtonClicked();
	std::function<void(cocos2d::Ref*)> MakeCreditsButtonClicked();
	std::function<void(cocos2d::Ref*)> MakeExitButtonClicked();

	class MyDialog* _optionsDialog;
	class MyDialog* _creditsDialog;
	class MyDialog* _exitGameDialog;
};
