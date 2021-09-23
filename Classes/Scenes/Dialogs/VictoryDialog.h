#pragma once

#include "MyDialog.h"

class VictoryDialog : public MyDialog
{
public:
	bool init();
	void SetLevelTextContent(const std::string&);
	void SetTimeTextContent(const std::string&);
	void SetBestTimeTextContent(const std::string&);
	void SetOnNextButtonPressed(const std::function<void(cocos2d::Ref*)>&);
	void SetOnMainMenuButtonPressed(const std::function<void(cocos2d::Ref*)>&);
};
