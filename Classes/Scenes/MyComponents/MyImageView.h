#pragma once
#include "ActionHolder.h"
#include "ui/UIImageView.h"
#include "base/CCValue.h"
#include "ActionHolder.inl"


class MyImageView : public ActionHolder<cocos2d::ui::ImageView>
{
	using Super = ActionHolder<cocos2d::ui::ImageView>;
public:
	bool init(cocos2d::ValueMap& value);
};