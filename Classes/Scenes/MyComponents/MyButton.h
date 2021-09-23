#pragma once
#include "ui/UIButton.h"
#include "base/CCValue.h"
#include "ActionHolder.h"

class MyButton : public ActionHolder<cocos2d::ui::Button>
{
	using Super = ActionHolder<cocos2d::ui::Button>;
public:
	bool init(cocos2d::ValueMap& value);
};