#pragma once

#include "../MyCustomGUI.h"
#include "2d/CCLayer.h"

class MyDialog : public MyCustomGUI<cocos2d::Layer>
{
public:
	cocos2d::TMXTiledMap* GetTiledMap() { return _tiledMap; }

protected:
	bool init(const std::string& tmxPath);
	
};