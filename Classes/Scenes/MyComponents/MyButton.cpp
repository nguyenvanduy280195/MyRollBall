#include "MyButton.h"
#include "Utils/StaticMethods.h"

using Vec2 = cocos2d::Vec2;

bool MyButton::init(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	const auto src_normal = value["src-normal"].asString();
	const auto src_pressed = value["src-pressed"].asString();
	const auto src_disabled = value["src-disabled"].asString();
	const auto title = value["title"].asString();
	const auto fontName = value["font-name"].asString();
	const auto fontSize = value["font-size"].asFloat();
	const auto titleColor = StaticMethods::MakeColor3BFromHex(value["title-color"].asString());

	if (!Super::init(src_normal, src_pressed, src_disabled))
	{
		return false;
	}
	
	setName(name);
	setTitleColor(titleColor);
	setTitleText(title);
	setTitleFontName(fontName);
	setTitleFontSize(fontSize);
	setPosition(Vec2(x, y));

	return true;
}