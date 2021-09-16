#pragma once

#include "MyLayer.h"

class ChoosingLevelLayer : public MyLayer
{
public:
	bool init(const cocos2d::Vec2& boxPosition);
	bool init(const cocos2d::Vec2& boxPosition, const std::string& tmxPath);

private:
	void StartLevel(int level);

};