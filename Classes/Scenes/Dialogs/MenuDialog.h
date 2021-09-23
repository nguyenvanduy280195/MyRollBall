#pragma once
#include "MyDialog.h"

class MenuDialog : public MyDialog
{
public:
	bool init(const cocos2d::Vec2& position);
	void Show() override;
	void Hide() override;

	std::function<void()> OnActionBegan;
	std::function<void()> OnActionEnded;

private:
	void RunButtonsAction(bool showing);

	class MyButton* _optionButton;
	class MyButton* _creditButton;
	class MyButton* _exitButton;
	class MyButton* _closeButton;
};
