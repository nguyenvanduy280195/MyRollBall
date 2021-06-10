#pragma once

#include "2d/CCLayer.h"
#include "ui/UILayoutParameter.h"

namespace cocos2d
{
namespace ui
{
class Button;
class Layout;
class Text;
}
}

class VictoryDialog : public cocos2d::Layer
{
public:
	bool init() override;
	std::function<void(cocos2d::Ref*)> OnNextButtonPressed;
	std::function<void(cocos2d::Ref*)> OnHomeButtonPressed;

private:
	cocos2d::Node* MakeBackground() const;
	cocos2d::Node* MakeGround()const;
	cocos2d::Node* MakeTitle() const;
	cocos2d::Node* MakeContent() const;
	cocos2d::ui::Layout* MakeButtons() const;

	cocos2d::ui::Button* MakeHomeButton() const;
	cocos2d::ui::Button* MakeNextButton() const;

	cocos2d::ui::Text* MakeText(const std::string& content, float fontSize = 50) const;
	cocos2d::ui::Text* MakeTextWithRelativeLayoutParameter(const std::string& content,
														   float fontSize,
														   const cocos2d::ui::RelativeLayoutParameter::RelativeAlign& align,
														   const cocos2d::ui::Margin& margin,
														   const std::string& relativeName = "",
														   const std::string& relativeToWidgetName = "") const;
	cocos2d::ui::Button* MakeButton(const std::string& title) const;
};