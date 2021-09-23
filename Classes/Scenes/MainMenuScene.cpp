#include "MainMenuScene.h"

#include "MyCustomGUI.inl"

#include "base/CCDirector.h"

#include "2d/CCActionInstant.h"

#include "Utils/StaticMethods.h"
#include "Utils/Cocos2dCreator.h"

#include "Dialogs/MenuDialog.h"

#include "Layers/ChoosingLevelLayer.h"

#include "MyComponents/MyButton.h"

using Vec2 = cocos2d::Vec2;

bool MainMenuScene::init()
{
	if (!MyCustomGUI<cocos2d::Scene>::init("ui/scene-main-menu.tmx"))
	{
		return false;
	}

	if (!LoadMyWidgetByName<MyButton>(_menuButton, "menu") ||
		!LoadMyWidgetByName<MyButton>(_previousPageButton, "previous-page") ||
		!LoadMyWidgetByName<MyButton>(_nextPageButton, "next-page"))
	{
		return false;
	}

	PutVersionTextInBottomLeftCorner();


	SetupPreviousPageButton();
	SetupNextPageButton();
	SetupMenuButton();


	InitChoosingLevelLayer();
	InitMenuDialog();

	return true;
}

void MainMenuScene::OnNextPageButtonClicked(cocos2d::Ref* ref)
{
	if (++_iPage >= nPages)
	{
		return;
	}

	if (auto child = getChildByName(CURRENT_LAYER))
	{
		auto button = (cocos2d::ui::Button*)ref;
		button->setEnabled(false);

		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		auto leftDeltaPosition = Vec2(-winSize.width, 0);
		auto currentLayerAction = MakeCurrentLayerAction(leftDeltaPosition, [this, leftDeltaPosition, winSize]()
		{
			auto newLayer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", _iPage));
			newLayer->setPositionX(winSize.width);
			addChild(newLayer);

			auto newLayerAction = MakeNewLayerAction(leftDeltaPosition, [newLayer, this]()
			{
				newLayer->setName(CURRENT_LAYER);
			});
			newLayer->runAction(newLayerAction);
		});

		auto action = cocos2d::Sequence::createWithTwoActions(currentLayerAction,
															  cocos2d::CallFunc::create([button]() { button->setEnabled(true); }));
		child->runAction(action);
	}


	_previousPageButton->setVisible(true);
	if (_iPage + 1 >= nPages)
	{
		_nextPageButton->setVisible(false);
	}
}

void MainMenuScene::OnPreviousPageButtonClicked(cocos2d::Ref* ref)
{
	if (--_iPage < 0)
	{
		return;
	}

	if (auto child = getChildByName(CURRENT_LAYER))
	{
		auto button = (cocos2d::ui::Button*)ref;
		button->setEnabled(false);

		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		auto leftDeltaPosition = Vec2(winSize.width, 0);
		auto currentLayerAction = MakeCurrentLayerAction(leftDeltaPosition, [this, leftDeltaPosition, winSize]()
		{
			auto newLayer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", _iPage));
			newLayer->setPositionX(-winSize.width);
			addChild(newLayer);

			auto newLayerAction = MakeNewLayerAction(leftDeltaPosition, [newLayer, this]()
			{
				newLayer->setName(CURRENT_LAYER);
			});
			newLayer->runAction(newLayerAction);
		});

		auto action = cocos2d::Sequence::createWithTwoActions(currentLayerAction,
															  cocos2d::CallFunc::create([button]() { button->setEnabled(true); }));
		child->runAction(action);
	}

	_nextPageButton->setVisible(true);
	if (_iPage - 1 < 0)
	{
		_previousPageButton->setVisible(false);
	}
}

cocos2d::FiniteTimeAction* MainMenuScene::MakeCurrentLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onNewLayerMoving) const
{
	auto moveByAction = cocos2d::MoveBy::create(MOVE_DURATION, deltaPosition);
	auto onNewLayerMovingCallback = cocos2d::CallFunc::create([onNewLayerMoving]() { if (onNewLayerMoving) onNewLayerMoving(); });
	auto removeSelfAction = cocos2d::RemoveSelf::create();
	auto moveByTogether = cocos2d::Spawn::createWithTwoActions(moveByAction, onNewLayerMovingCallback);
	return cocos2d::Sequence::createWithTwoActions(moveByTogether, removeSelfAction);
}

cocos2d::FiniteTimeAction* MainMenuScene::MakeNewLayerAction(const cocos2d::Vec2& deltaPosition, const std::function<void()>& onMovingDone) const
{
	auto moveByAction = cocos2d::MoveBy::create(MOVE_DURATION, deltaPosition);
	auto onMovingDoneCallback = cocos2d::CallFunc::create([onMovingDone]() { if (onMovingDone) onMovingDone(); });
	return cocos2d::Sequence::createWithTwoActions(moveByAction, onMovingDoneCallback);
}

void MainMenuScene::PutMenuButtonInTopRightCorner()
{
	auto menuButtonSize = _menuButton->getContentSize();
	auto visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto menuButtonPosition = Vec2(visibleOrigin.x + visibleSize.width - menuButtonSize.width,
								   visibleOrigin.y + visibleSize.height - menuButtonSize.height);
	_menuButton->setPosition(menuButtonPosition);
}

void MainMenuScene::PutVersionTextInBottomLeftCorner()
{
	if (auto child = _tiledMap->getChildByName("version"))
	{
		child->setPosition(cocos2d::Director::getInstance()->getVisibleOrigin());
	}
}

void MainMenuScene::InitChoosingLevelLayer()
{
	auto layer = Cocos2dCreator::CreateNode<ChoosingLevelLayer>(Vec2::ZERO, cocos2d::StringUtils::format("ui/dialog-choosing-level-%d.tmx", 0));
	layer->setName(CURRENT_LAYER);
	addChild(layer);
}

void MainMenuScene::InitMenuDialog()
{
	_menuDialog = Cocos2dCreator::CreateNode<MenuDialog>(_menuButton->getPosition());
	_menuDialog->OnActionBegan = [this]() { _menuButton->setEnabled(false); };
	_menuDialog->OnActionEnded = [this]() { _menuButton->setEnabled(true); };
	addChild(_menuDialog);
}

void MainMenuScene::SetupPreviousPageButton()
{
	_previousPageButton->setVisible(false);
	_previousPageButton->addClickEventListener(CC_CALLBACK_1(MainMenuScene::OnPreviousPageButtonClicked, this));
}

void MainMenuScene::SetupNextPageButton()
{
	_nextPageButton->addClickEventListener(CC_CALLBACK_1(MainMenuScene::OnNextPageButtonClicked, this));
}

void MainMenuScene::SetupMenuButton()
{
	PutMenuButtonInTopRightCorner();
	_menuButton->addClickEventListener([this](cocos2d::Ref*) { _menuDialog->Show(); });
}