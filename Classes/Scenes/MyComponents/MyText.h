#pragma once
#include "ui/UIText.h"
#include "base/CCValue.h"
#include "ActionHolder.h"

class MyText : public ActionHolder<cocos2d::ui::Text>
{
	using Super = ActionHolder<cocos2d::ui::Text>;
public:
	bool init(cocos2d::ValueMap& value);
};