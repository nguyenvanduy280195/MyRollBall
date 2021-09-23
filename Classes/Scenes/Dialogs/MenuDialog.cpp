#include "MenuDialog.h"

#include "../MyCustomGUI.inl"

#include "base/CCDirector.h"

#include "Utils/Cocos2dCreator.h"
#include "Utils/TMXUtil.h"

#include "OptionsDialog.h"
#include "CreditsDialog.h"
#include "ExitGameDialog.h"

// TODO convert actions to tmx
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"


using Vec2 = cocos2d::Vec2;

bool MenuDialog::init(const cocos2d::Vec2& position)
{
	if (!MyDialog::init("ui/dialog-menu.tmx", true))
	{
		return false;
	}

	if (!LoadMyWidgetByName<MyButton>(_optionButton, "option") ||
		!LoadMyWidgetByName<MyButton>(_creditButton, "credits") ||
		!LoadMyWidgetByName<MyButton>(_exitButton, "exit") ||
		!LoadMyWidgetByName<MyButton>(_closeButton, "close"))
	{
		return false;
	}

	auto centerWindow = 0.5f * cocos2d::Director::getInstance()->getWinSize();

	auto optionsDialog = Cocos2dCreator::CreateNode<OptionsDialog>(centerWindow);
	addChild(optionsDialog);

	auto creditsDialog = Cocos2dCreator::CreateNode<CreditsDialog>(centerWindow);
	addChild(creditsDialog);

	auto exitDialog = Cocos2dCreator::CreateNode<ExitGameDialog>(centerWindow);
	addChild(exitDialog);

	_optionButton->addClickEventListener([optionsDialog](cocos2d::Ref*) { optionsDialog->Show(); });
	_creditButton->addClickEventListener([creditsDialog](cocos2d::Ref*) { creditsDialog->Show(); });
	_exitButton->addClickEventListener([exitDialog](cocos2d::Ref*) { exitDialog->Show(); });
	_closeButton->addClickEventListener([this](cocos2d::Ref*) { Hide(); });


	_tiledMap->setAnchorPoint(Vec2(0.82f, 0.5f));
	_tiledMap->setPosition(position);
	_tiledMap->setScaleX(.1f);
	_tiledMap->setScaleY(1.f);

	return true;
}

void MenuDialog::Show()
{
	setVisible(true);
	_tiledMap->runAction(cocos2d::ScaleBy::create(.1f, 10.f, 1.f));
	
	RunButtonsAction(true);
}

void MenuDialog::Hide()
{
	auto scaleTo = cocos2d::ScaleBy::create(.1f, .1f, 1.f);
	auto onHide = cocos2d::CallFunc::create([this]()
	{
		setVisible(false);
	});
	auto action = cocos2d::Sequence::createWithTwoActions(scaleTo, onHide);
	_tiledMap->runAction(action);

	RunButtonsAction(false);
}

void MenuDialog::RunButtonsAction(bool showing)
{
	TMXUtil::ForeachAllObjectsInObjectGroup(_tiledMap, "buttons", [this, showing](cocos2d::ValueMap& value)
	{
		auto name = value["name"].asString();
		auto duration = value["duration"].asFloat();
		auto x = value["hidden.x"].asFloat();
		auto y = value["hidden.y"].asFloat();
		if (showing)
		{
			x = value["shown.x"].asFloat();
			y = value["shown.y"].asFloat();
		}

		if (auto button = GetMyWidgetByName<MyButton>(name))
		{
			cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
			actions.pushBack(cocos2d::CallFunc::create([this, button]()
			{
				if (OnActionBegan)
				{
					OnActionBegan();
				}
				button->setEnabled(false);
			}));
			actions.pushBack(cocos2d::MoveTo::create(duration, Vec2(x, y)));
			actions.pushBack(cocos2d::CallFunc::create([this, button]()
			{
				if (OnActionEnded)
				{
					OnActionEnded();
				}

				button->setEnabled(true);
			}));

			button->runAction(cocos2d::Sequence::create(actions));
		}
	});

	SetSwallowTouches(showing);
}
