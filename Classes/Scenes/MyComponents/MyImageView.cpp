#include "MyImageView.h"

using Vec2 = cocos2d::Vec2;

bool MyImageView::init(cocos2d::ValueMap& value)
{
	const auto defaultScaleFactor = 1.f;
	const auto name = value["name"].asString();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	const auto src = value["src"].asString();
	const auto invisible = value["invisible"].asBool();
	auto scaleFactorX = value["scale-factor-x"].asFloat();
	auto scaleFactorY = value["scale-factor-y"].asFloat();

	if (!Super::init(src))
	{
		return false;
	}

	if (scaleFactorX == 0.f)
	{
		scaleFactorX = defaultScaleFactor;
	}

	if (scaleFactorY == 0.f)
	{
		scaleFactorY = defaultScaleFactor;
	}

	setScale(scaleFactorX, scaleFactorY);
	setVisible(!invisible);
	setName(name);
	setPosition(Vec2(x, y));

	return true;
}