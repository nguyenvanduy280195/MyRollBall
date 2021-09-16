#pragma once

#include "MyLayer.h"

class ScreenInfoLayer : public MyLayer
{
public:
	bool init();

	float GetHeight() const; 

	void SetCoinText(const std::string& text);
	void ShowKey();
	void AddPauseButtonCallback(const std::function<void(cocos2d::Ref*)>& callback); 
};