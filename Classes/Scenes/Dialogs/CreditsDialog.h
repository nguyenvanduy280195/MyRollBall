#pragma once

#include "MyDialog.h"

namespace cocos2d::ui
{
class LayoutParameter;
}

namespace cocos2d
{
class Sprite;
}

class CreditsDialog : public MyDialog
{
public:
	bool init(const cocos2d::Vec2& position);
private:
	void AddSpriteFromTMXObject(const std::string& objectGroupName, const std::string& objectName);
};