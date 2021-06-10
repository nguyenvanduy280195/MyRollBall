#include "VictoryDialog.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "ui/UIText.h"
#include "ui/UIVBox.h"
#include "ui/UIRelativeBox.h"

using Size = cocos2d::Size;
using Vec2 = cocos2d::Vec2;
using RelativeLayoutParameter = cocos2d::ui::RelativeLayoutParameter;
using RelativeAlign = cocos2d::ui::RelativeLayoutParameter::RelativeAlign;
using LinearGravity = cocos2d::ui::LinearLayoutParameter::LinearGravity;
using LinearLayoutParameter = cocos2d::ui::LinearLayoutParameter;
using RelativeBox = cocos2d::ui::RelativeBox;
using VBox = cocos2d::ui::VBox;
using Margin = cocos2d::ui::Margin;

bool VictoryDialog::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	auto cameraPostion = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPostion);

	auto background = MakeBackground();
	addChild(background);

	auto ground = MakeGround();
	addChild(ground);

	auto title = MakeTitle();
	ground->addChild(title);

	auto content = MakeContent();
	ground->addChild(content);

	auto homeButton = MakeHomeButton();
	ground->addChild(homeButton);

	auto nextButton = MakeNextButton();
	ground->addChild(nextButton);

	return true;
}

cocos2d::Node* VictoryDialog::MakeBackground() const
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	auto background = cocos2d::ui::Layout::create();
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	background->setBackGroundColor(cocos2d::Color3B::BLACK);
	background->setOpacity(100.5f);
	background->setContentSize(winSize);
	return background;
}

cocos2d::Node* VictoryDialog::MakeGround() const
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	auto ground = cocos2d::ui::VBox::create(Size(300, 500));
	ground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ground->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	ground->setBackGroundColor(cocos2d::Color3B::GREEN);
	return ground;
}

cocos2d::Node* VictoryDialog::MakeTitle() const
{
	auto title = cocos2d::ui::Text::create("Victory", "fonts/Marker Felt.ttf", 50);
	auto param = LinearLayoutParameter::create();
	param->setMargin(cocos2d::ui::Margin(0, 10, 0, 0));
	param->setGravity(LinearGravity::CENTER_HORIZONTAL);
	title->setLayoutParameter(param);
	return title;
}

cocos2d::Node* VictoryDialog::MakeContent() const
{
	auto content = VBox::create(Size(300, 300));
	content->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	content->setBackGroundColor(cocos2d::Color3B::RED);
	content->addChild(MakeTextWithRelativeLayoutParameter("Level: ",
					  25,
					  RelativeAlign::PARENT_TOP_LEFT,
					  Margin(),
					  "levelLabel",
					  ""));
	content->addChild(MakeTextWithRelativeLayoutParameter("Time: ",
					  25,
					  RelativeAlign::LOCATION_BELOW_LEFTALIGN,
					  Margin(),
					  "timeLabel",
					  "levelLabel"));

	content->addChild(MakeTextWithRelativeLayoutParameter("Best time: ",
					  25,
					  RelativeAlign::LOCATION_BELOW_LEFTALIGN,
					  Margin(),
					  "bestTimeLabel",
					  "timeLabel"));


	auto contentParam = LinearLayoutParameter::create();
	content->setLayoutParameter(contentParam);

	return content;
}

cocos2d::ui::Layout* VictoryDialog::MakeButtons() const
{
	auto buttonsBox = cocos2d::ui::RelativeBox::create();

	auto homeButton = MakeHomeButton();
	buttonsBox->addChild(homeButton);

	auto nextButton = MakeNextButton();
	buttonsBox->addChild(nextButton);

	return buttonsBox;
}

cocos2d::ui::Button* VictoryDialog::MakeHomeButton() const
{
	auto homeButton = MakeButton("Home");
	
	auto param = LinearLayoutParameter::create();
	param->setGravity(LinearGravity::CENTER_HORIZONTAL);
	param->setMargin(Margin(0, 5, 0, 5));

	homeButton->setLayoutParameter(param);
	homeButton->addClickEventListener([this](cocos2d::Ref* ref)
	{
		if (OnHomeButtonPressed)
		{
			OnHomeButtonPressed(ref);
		}
	});

	return homeButton;
}

cocos2d::ui::Button* VictoryDialog::MakeNextButton() const
{
	auto nextButton = MakeButton("Next");
	
	auto param = LinearLayoutParameter::create();
	param->setGravity(LinearGravity::CENTER_HORIZONTAL);
	param->setMargin(Margin(0, 5, 0, 5));

	nextButton->setLayoutParameter(param);
	nextButton->addClickEventListener([this](cocos2d::Ref* ref)
	{
		if (OnNextButtonPressed)
		{
			OnNextButtonPressed(ref);
		}
	});

	return nextButton;
}

cocos2d::ui::Button* VictoryDialog::MakeButton(const std::string& title) const
{
	auto button = cocos2d::ui::Button::create("levels/assets/blue_button_normal.png",
											  "levels/assets/blue_button_pressed.png",
											  "levels/assets/blue_button_disabled.png");
	button->setTitleText(title);
	return button;
}

cocos2d::ui::Text* VictoryDialog::MakeText(const std::string& content, float fontSize) const
{
	return cocos2d::ui::Text::create(content, "fonts/Marker Felt.ttf", fontSize);;
}

cocos2d::ui::Text* VictoryDialog::MakeTextWithRelativeLayoutParameter(const std::string& content,
																	  float fontSize,
																	  const cocos2d::ui::RelativeLayoutParameter::RelativeAlign& align,
																	  const cocos2d::ui::Margin& margin,
																	  const std::string& relativeName,
																	  const std::string& relativeToWidgetName) const
{
	auto text = MakeText(content, 25);

	auto param = RelativeLayoutParameter::create();
	param->setAlign(align);
	param->setMargin(margin);
	param->setRelativeName(relativeName);
	param->setRelativeToWidgetName(relativeToWidgetName);

	text->setLayoutParameter(param);
	return text;
}