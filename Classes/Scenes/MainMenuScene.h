#pragma once

#include "2d/CCScene.h"
#include "base/CCValue.h"
#include "MyCustomGUI.h"

namespace cocos2d
{
class FiniteTimeAction;
}

class MainMenuScene : public MyCustomGUI<cocos2d::Scene>
{
public:
	bool init();
private:
	void InitChoosingLevelLayer();
	void InitMenuDialog();

	void SetupPreviousPageButton();
	void SetupNextPageButton();
	void SetupMenuButton();

	void OnNextPageButtonClicked(cocos2d::Ref*);
	void OnPreviousPageButtonClicked(cocos2d::Ref*);

	void PutMenuButtonInTopRightCorner();
	void PutVersionTextInBottomLeftCorner();

	cocos2d::FiniteTimeAction* MakeCurrentLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onNewLayerMoving) const;
	cocos2d::FiniteTimeAction* MakeNewLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onMovingDone) const;

	std::vector<class ChoosingLevelLayer*> _choosingLevelLayers;

	const std::string CURRENT_LAYER = "current-layer";
	
	const int nPages = 3;
	int _iPage = 0;
	float MOVE_DURATION = .5f;

	class MyButton* _menuButton;
	class MyButton* _previousPageButton;
	class MyButton* _nextPageButton;
	class MenuDialog* _menuDialog;
};
