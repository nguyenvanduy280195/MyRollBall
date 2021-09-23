#include "MyText.h"
#include "Utils/StaticMethods.h"

using Vec2 = cocos2d::Vec2;

bool MyText::init(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto fontName = value["font-name"].asString();
	const auto fontSize = value["font-size"].asFloat();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	const auto anchorPointX = value["anchor-point-x"].asFloat();
	const auto anchorPointY = value["anchor-point-y"].asFloat();
	const auto color = StaticMethods::MakeColor3BFromHex(value["color"].asString());
	const auto shadowColor = StaticMethods::MakeColor3BFromHex(value["shadow.color"].asString());
	const auto shadowWidth = value["shadow.width"].asFloat();
	const auto shadowHeight = value["shadow.height"].asFloat();
	auto str = value["text"].asString();

	if (str.empty())
	{
		str = "<text>";
	}

	if(!Super::init(str, fontName, fontSize))
	{
		return false;
	}


	setAnchorPoint(Vec2(anchorPointX, anchorPointY));
	setName(name);
	setColor(color);
	setPosition(Vec2(x, y));

	if (value["enableShadow"].asBool())
	{
		enableShadow(cocos2d::Color4B(shadowColor), cocos2d::Size(shadowWidth, shadowHeight));
	}

	return true;
}