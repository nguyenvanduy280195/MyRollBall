#pragma once

#include "2d/CCScene.h"
#include "2d/CCLayer.h"
#include "base/CCValue.h"
#include "MyCustomGUI.h"

namespace cocos2d
{
class TMXTiledMap;
class Action;

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
	void SetPreviousPageButtonVisible(bool visible);
	void SetNextPageButtonVisible(bool visible);

	void OnNextPageButtonClicked();
	void OnPreviousPageButtonClicked();

	cocos2d::Action* MakeCurrentLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onNewLayerMoving) const;
	cocos2d::Action* MakeNewLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onMovingDone) const;

	std::vector<class ChoosingLevelLayer*> _choosingLevelLayers;

	const std::string CURRENT_LAYER = "current-layer";
	
	const int nPages = 3;
	int _iPage = 0;
	float MOVE_DURATION = .5f;
};
