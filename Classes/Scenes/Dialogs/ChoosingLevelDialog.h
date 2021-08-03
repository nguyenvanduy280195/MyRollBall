#pragma once

#include "MyDialog.h"

class ChoosingLevelDialog : public MyDialog
{
public:
	bool init(const cocos2d::Vec2& boxPosition);

private:
	void StartLevel(int level);

	std::function<void(cocos2d::Ref*)> MakePageNumberButtonCallback(int pageNumber);

	const int NUMBER_OF_LEVEL_IN_PAGE = 10;
	const int NUMBER_OF_PAGE_IN_DIALOG = 5;
};