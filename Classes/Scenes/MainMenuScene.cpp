#include "MainMenuScene.h"

#include "2d/CCTMXTiledMap.h"
#include "ui/UIButton.h"
#include "2d/CCTransition.h"

#include "Utils/StaticMethods.h"
#include "Scenes/IntroLevelScene.h"
#include "Utils/Cocos2dCreator.h"
#include "Utils/TMXUtil.h"

#include "MyCustomGUI.inl"

#include "Dialogs/CreditsDialog.h"
#include "Dialogs/ExitGameDialog.h"
#include "Dialogs/OptionsDialog.h"
#include "Dialogs/ChoosingLevelDialog.h"


#include "ScreenLog/ScreenLog.h"


using Vec2 = cocos2d::Vec2;

bool MainMenuScene::init()
{
	if (!MyCustomGUI<cocos2d::Scene>::init("ui/scene-main-menu.tmx"))
	{
		return false;
	}

	_tiledMap->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto boxPosition = GetBoxPosition();

	_choosingLevelDialog = Cocos2dCreator::CreateNode<ChoosingLevelDialog>(boxPosition);
	AddCallbackToButton("play", MakeMenuButtonClicked(_choosingLevelDialog));
	addChild(_choosingLevelDialog);

	_optionsDialog = Cocos2dCreator::CreateNode<OptionsDialog>(boxPosition);
	AddCallbackToButton("options", MakeMenuButtonClicked(_optionsDialog));
	addChild(_optionsDialog);

	_creditsDialog = Cocos2dCreator::CreateNode<CreditsDialog>(boxPosition);
	AddCallbackToButton("credits", MakeMenuButtonClicked(_creditsDialog));
	addChild(_creditsDialog);

	_exitGameDialog = Cocos2dCreator::CreateNode<ExitGameDialog>(boxPosition);
	AddCallbackToButton("exit", MakeMenuButtonClicked(_exitGameDialog));
	addChild(_exitGameDialog);

	return true;
}

const cocos2d::Vec2 MainMenuScene::GetBoxPosition() const
{
	cocos2d::Vec2 boxPosition;

	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "box", [&boxPosition](cocos2d::TMXObjectGroup* objGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objGroup, "box", [&boxPosition](cocos2d::ValueMap& value)
		{
			const auto x = value["x"].asFloat();
			const auto y = value["y"].asFloat();
			boxPosition = Vec2(x, y);
		});
	});

	return boxPosition;
}

void MainMenuScene::HideAllDialogs()
{
	if (_choosingLevelDialog->isVisible())
	{
		_choosingLevelDialog->Hide();
	}

	if (_optionsDialog->isVisible())
	{
		_optionsDialog->Hide();
	}

	if (_creditsDialog->isVisible())
	{
		_creditsDialog->Hide();
	}

	if (_exitGameDialog->isVisible())
	{
		_exitGameDialog->Hide();
	}
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeMenuButtonClicked(class MyDialog* dialog)
{
	return [dialog, this](cocos2d::Ref*)
	{
		HideAllDialogs();
		dialog->Show();
	};
}