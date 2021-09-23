#include "MyDialog.h"

#include "../MyCustomGUI.inl"
#include "../TouchSwallower.inl"

#include "base/CCDirector.h"

#include "ui/UILayout.h"

// TODO convert actions to tmx
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"

bool MyDialog::init(const std::string& tmxPath, bool backgroundEnabled)
{
	if (!Super::init(tmxPath))
	{
		return false;
	}

	setLocalZOrder(INT16_MAX);

	if (backgroundEnabled)
	{
		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		setContentSize(winSize);

		auto background = cocos2d::ui::Layout::create();
		background->setContentSize(winSize);
		background->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
		background->setBackGroundColor(cocos2d::Color3B::BLACK);
		background->setOpacity(100);
		background->setSwallowTouches(true);
		addChild(background, -1);
	}
	


	_tiledMap->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_tiledMap->setScale(0.1f);
	setVisible(false);

	return true;
}



void MyDialog::Show()
{
	setVisible(true);
	SetSwallowTouches(true);
	_tiledMap->runAction(cocos2d::ScaleTo::create(.1f, 1.f));
}

void MyDialog::Hide()
{
	auto scaleTo = cocos2d::ScaleTo::create(.1f, .1f);
	auto onHide = cocos2d::CallFunc::create([this]() 
	{ 
		setVisible(false);
	});

	if(OnHidden)
	{
		auto action = cocos2d::Sequence::create(scaleTo, onHide, cocos2d::CallFunc::create(OnHidden), nullptr);
		_tiledMap->runAction(action);
	}
	else
	{
		auto action = cocos2d::Sequence::createWithTwoActions(scaleTo, onHide);
		_tiledMap->runAction(action);
	}
	SetSwallowTouches(false);
}