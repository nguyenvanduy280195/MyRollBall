#include "ScreenLog.h"
#include "2d/CCDrawNode.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "ui/UIListView.h"
#include "ui/UIText.h"
#include "2d/CCLabel.h"
#include "ui/UIVBox.h"

#include "Infos/GameInfo.h"

using Vec2 = cocos2d::Vec2;

bool ScreenLog::init(class GameInfo* gameInfo)
{
	if (!cocos2d::Node::init())
	{
		return false;
	}
	_gameInfo = gameInfo;
	

	_listView = MakeListView();
	addChild(_listView);

	
	scheduleUpdate();
	
	return true;
}

cocos2d::ui::ListView* ScreenLog::MakeListView() const
{
	auto winSize = cocos2d::Director::getInstance()->getWinSize();

	auto listView = cocos2d::ui::ListView::create();
	listView->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	listView->setContentSize(winSize);
	listView->setDirection(cocos2d::ui::ListView::Direction::VERTICAL);
	listView->setScrollBarEnabled(false);

	return listView;
}

void ScreenLog::AddLog(const std::string& msg)
{
	auto text = MakeText(msg);
	_listView->pushBackCustomItem(text);

	if (_listView->getChildrenCount() >= _gameInfo->GetScreenLogInfo().logCountLimit)
	{
		_listView->removeItem(0);
	}
}

cocos2d::ui::Text* ScreenLog::MakeText(const std::string& msg) const
{
	auto text = cocos2d::ui::Text::create(msg, _gameInfo->GetScreenLogInfo().fontFamily, _gameInfo->GetScreenLogInfo().fontSize);
	text->setColor(_gameInfo->GetScreenLogInfo().textColor);
	text->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	return text;
}

void ScreenLog::update(float dt)
{
	UpdatePosition();
	RemoveFirstItemAfterTime(dt);
}

void ScreenLog::RemoveFirstItemAfterTime(float dt)
{
	_removingTimer += dt;
	if (_removingTimer >= _gameInfo->GetScreenLogInfo().removingTimerLimit)
	{
		_removingTimer = 0;

		const auto childrenCount = _listView->getChildrenCount();
		if (childrenCount > 0)
		{
			_listView->removeItem(0);
		}
	}

}

void ScreenLog::UpdatePosition()
{
	const auto cameraPosition = cocos2d::Camera::getDefaultCamera()->getPosition();
	const auto winSize = cocos2d::Director::getInstance()->getWinSize();
	const auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	const auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto x = cameraPosition.x - 0.5f * winSize.width;
	auto y = cameraPosition.y + 0.5f * winSize.height - 1.5f*visibleOrigin.y;
	setPosition(x, y);
}