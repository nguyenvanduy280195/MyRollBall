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
#include "Dialogs/LevelDialog.h"

#include "ScreenLog/ScreenLog.h"
#include "Dialogs/MyDialog.h"

using Vec2 = cocos2d::Vec2;

bool MainMenuScene::init()
{
	if (!MyCustomGUI<cocos2d::Scene>::init("ui/scene-main-menu.tmx"))
	{
		return false;
	}

	_tiledMap->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	AddCallbackToButton("play", MakePlayButtonClicked());
	AddCallbackToButton("options", MakeOptionsButtonClicked());
	AddCallbackToButton("credits", MakeCreditsButtonClicked());
	AddCallbackToButton("exit", MakeExitButtonClicked());

	//ScreenLog::GetInstance()->Debug(__FUNCTION__);
	//duy::ScreenLog::GetInstance()->SetOwner(this);
	//duy::ScreenLog::GetInstance()->Info("test1");

	return true;
}

void MainMenuScene::TakeLayerToBoxPosition(MyDialog* dialog)
{
	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "box", [dialog](cocos2d::TMXObjectGroup* objGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objGroup, "box", [dialog](cocos2d::ValueMap& value)
		{
			const auto x = value["x"].asFloat();
			const auto y = value["y"].asFloat();
			dialog->GetTiledMap()->setPosition(x, y);
		});
	});
}


void MainMenuScene::SetAllLayersVisible(bool visible)
{
	if (_optionsDialog)
	{
		_optionsDialog->setVisible(visible);
	}
	if (_creditsDialog)
	{
		_creditsDialog->setVisible(visible);
	}
	if (_exitGameDialog)
	{
		_exitGameDialog->setVisible(visible);
	}
}


std::function<void(cocos2d::Ref*)> MainMenuScene::MakePlayButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>();
		//auto sceneWithTransition = cocos2d::TransitionZoomFlipAngular::create(1, scene);
		cocos2d::Director::getInstance()->pushScene(scene);
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeOptionsButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);

		if (_optionsDialog)
		{
			_optionsDialog->setVisible(true);
		}
		else
		{
			//_optionsLayer = Cocos2dCreator::CreateNode<LevelDialog>();
			//if (_optionsLayer)
			//{
			//	TakeLayerToBoxPosition(_optionsLayer);
			//	addChild(_optionsLayer);
			//}
		}
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeCreditsButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);


		if (_creditsDialog)
		{
			_creditsDialog->setVisible(true);
		}
		else
		{
			_creditsDialog = Cocos2dCreator::CreateNode<CreditsDialog>();
			if (_creditsDialog)
			{
				TakeLayerToBoxPosition(_creditsDialog);
				addChild(_creditsDialog);
			}
		};
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeExitButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);


		if (_exitGameDialog)
		{
			_exitGameDialog->setVisible(true);
		}
		else
		{
			_exitGameDialog = Cocos2dCreator::CreateNode<ExitGameDialog>();
			if (_exitGameDialog)
			{
				TakeLayerToBoxPosition(_exitGameDialog);
				addChild(_exitGameDialog);
			}
		}
	};
}