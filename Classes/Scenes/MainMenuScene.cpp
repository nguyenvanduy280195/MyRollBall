#include "MainMenuScene.h"

#include "2d/CCTMXTiledMap.h"
#include "ui/UIButton.h"
#include "2d/CCTransition.h"
#include "2d/CCActionInterval.h"

#include "Utils/StaticMethods.h"
#include "Scenes/IntroLevelScene.h"
#include "Utils/Cocos2dCreator.h"
#include "Utils/TMXUtil.h"

#include "MyCustomGUI.inl"

#include "Dialogs/CreditsDialog.h"
#include "Dialogs/ExitGameDialog.h"
#include "Dialogs/OptionsDialog.h"
#include "Layers/ChoosingLevelLayer.h"


#include "ScreenLog/ScreenLog.h"


using Vec2 = cocos2d::Vec2;

bool MainMenuScene::init()
{
	if (!MyCustomGUI<cocos2d::Scene>::init("ui/scene-main-menu.tmx"))
	{
		return false;
	}

	_tiledMap->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto layer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", 0));
	layer->setName(CURRENT_LAYER);
	addChild(layer);

	auto centerWindow = 0.5f * cocos2d::Director::getInstance()->getWinSize();

	auto optionsDialog = Cocos2dCreator::CreateNode<OptionsDialog>(centerWindow);
	addChild(optionsDialog);

	auto creditsDialog = Cocos2dCreator::CreateNode<CreditsDialog>(centerWindow);
	addChild(creditsDialog);

	AddCallbackToButton("options", [optionsDialog](cocos2d::Ref*) { optionsDialog->Show(); });
	AddCallbackToButton("credits", [creditsDialog](cocos2d::Ref*) { creditsDialog->Show(); });
	AddCallbackToButton("next-page", [this](cocos2d::Ref*) { OnNextPageButtonClicked(); });
	AddCallbackToButton("previous-page", [this](cocos2d::Ref*) { OnPreviousPageButtonClicked(); });

	SetPreviousPageButtonVisible(false);

	if (auto child = _tiledMap->getChildByName("version"))
	{
		child->setPosition(cocos2d::Director::getInstance()->getVisibleOrigin());
	}

	return true;
}

void MainMenuScene::SetPreviousPageButtonVisible(bool visible)
{
	if (auto pageButton = GetButtonByName("previous-page"))
	{
		pageButton->setVisible(visible);
	}
}

void MainMenuScene::SetNextPageButtonVisible(bool visible)
{
	if (auto pageButton = GetButtonByName("next-page"))
	{
		pageButton->setVisible(visible);
	}
}

void MainMenuScene::OnNextPageButtonClicked()
{
	if (++_iPage >= nPages)
	{
		return;
	}
	
	if (auto child = getChildByName(CURRENT_LAYER))
	{
		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		auto leftDeltaPosition = Vec2(-winSize.width, 0);
		auto currentLayerAction = MakeCurrentLayerAction(leftDeltaPosition, [this, leftDeltaPosition, winSize]()
		{
			auto newLayer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", _iPage));
			addChild(newLayer);
			
			newLayer->setPositionX(winSize.width);

			auto newLayerAction = MakeNewLayerAction(leftDeltaPosition, [newLayer, this]()
			{
				newLayer->setName(CURRENT_LAYER);
			});
			newLayer->runAction(newLayerAction);
		});
		child->runAction(currentLayerAction);
	}
	

	SetPreviousPageButtonVisible(true);
	if (_iPage + 1 >= nPages)
	{
		SetNextPageButtonVisible(false);
	}
}

void MainMenuScene::OnPreviousPageButtonClicked()
{
	if (--_iPage < 0)
	{
		return;
	}

	if (auto child = getChildByName(CURRENT_LAYER))
	{
		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		auto leftDeltaPosition = Vec2(winSize.width, 0);
		auto currentLayerAction = MakeCurrentLayerAction(leftDeltaPosition, [this, leftDeltaPosition, winSize]()
		{
			auto newLayer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", _iPage));
			addChild(newLayer);

			newLayer->setPositionX(-winSize.width);

			auto newLayerAction = MakeNewLayerAction(leftDeltaPosition, [newLayer, this]()
			{
				newLayer->setName(CURRENT_LAYER);
			});
			newLayer->runAction(newLayerAction);
		});
		child->runAction(currentLayerAction);
	}

	SetNextPageButtonVisible(true);
	if (_iPage - 1 < 0)
	{
		SetPreviousPageButtonVisible(false);
	}
}

cocos2d::Action* MainMenuScene::MakeCurrentLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onNewLayerMoving) const
{
	auto moveByAction = cocos2d::MoveBy::create(MOVE_DURATION, deltaPosition);
	auto onNewLayerMovingCallback = cocos2d::CallFunc::create([onNewLayerMoving]() { if (onNewLayerMoving) onNewLayerMoving(); });
	auto removeSelfAction = cocos2d::RemoveSelf::create();

	auto moveByTogether = cocos2d::Spawn::createWithTwoActions(moveByAction, onNewLayerMovingCallback);

	return cocos2d::Sequence::createWithTwoActions(moveByTogether, removeSelfAction);
}

cocos2d::Action* MainMenuScene::MakeNewLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onMovingDone) const
{
	auto moveByAction = cocos2d::MoveBy::create(MOVE_DURATION, deltaPosition);
	auto onMovingDoneCallback = cocos2d::CallFunc::create([onMovingDone]() { if (onMovingDone) onMovingDone(); });

	return cocos2d::Sequence::create(moveByAction,
									 onMovingDoneCallback,
									 nullptr);
}