#include "MyDialog.h"

#include "../MyCustomGUI.inl"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
//#include "ScreenLog/ScreenLog.h"

bool MyDialog::init(const std::string& tmxPath)
{
	if (!Super::init(tmxPath))
	{
		return false;
	}

	_tiledMap->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	_tiledMap->setScale(0.1f);
	setVisible(false);

	return true;
}



void MyDialog::Show()
{
	setVisible(true);
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
}

void MyDialog::onEnter()
{
	Super::onEnter();

	//_tiledMap->setVisible(false);
}
