#include "MainMenuScene.h"

#include "2d/CCTMXTiledMap.h"
#include "ui/UIButton.h"
#include "2d/CCTransition.h"

#include "Utils/StaticMethods.h"
#include "Scenes/IntroLevelScene.h"
#include "Utils/Cocos2dCreator.h"
#include "Utils/TMXUtil.h"

#include "MyCustomGUI.inl"

#include "Layers/AboutLayer.h"
#include "Layers/ExitGameLayer.h"
#include "Layers/OptionsLayer.h"

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
	AddCallbackToButton("about", MakeAboutButtonClicked());
	AddCallbackToButton("exit", MakeExitButtonClicked());

	return true;
}

void MainMenuScene::TakeLayerToBoxPosition(MyCustomLayer* layer)
{
	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "box", [layer](cocos2d::TMXObjectGroup* objGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objGroup, "box", [layer](cocos2d::ValueMap& value)
		{
			const auto x = value["x"].asFloat();
			const auto y = value["y"].asFloat();
			layer->GetTiledMap()->setPosition(x, y);
		});
	});
}


void MainMenuScene::SetAllLayersVisible(bool visible)
{
	if (_optionsLayer)
	{
		_optionsLayer->setVisible(visible);
	}
	if (_aboutLayer)
	{
		_aboutLayer->setVisible(visible);
	}
	if (_exitGameLayer)
	{
		_exitGameLayer->setVisible(visible);
	}
}


std::function<void(cocos2d::Ref*)> MainMenuScene::MakePlayButtonClicked()
{
	return [](cocos2d::Ref*)
	{
		auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>();
		auto sceneWithTransition = cocos2d::TransitionZoomFlipAngular::create(1, scene);
		StaticMethods::ReplaceScene(scene, sceneWithTransition);
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeOptionsButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);

		if (_optionsLayer)
		{
			_optionsLayer->setVisible(true);
		}
		else
		{
            _optionsLayer = Cocos2dCreator::CreateNode<OptionsLayer>();
            if(_optionsLayer)
            {
                TakeLayerToBoxPosition(_optionsLayer);
                addChild(_optionsLayer);
            }
		}
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeAboutButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);


		if (_aboutLayer)
		{
			_aboutLayer->setVisible(true);
		}
		else
		{
            _aboutLayer = Cocos2dCreator::CreateNode<AboutLayer>();
            if(_aboutLayer)
            {
                TakeLayerToBoxPosition(_aboutLayer);
                addChild(_aboutLayer);
            }
		};
	};
}

std::function<void(cocos2d::Ref*)> MainMenuScene::MakeExitButtonClicked()
{
	return [this](cocos2d::Ref*)
	{
		SetAllLayersVisible(false);


		if (_exitGameLayer)
		{
			_exitGameLayer->setVisible(true);
		}
		else
		{
            _exitGameLayer = Cocos2dCreator::CreateNode<ExitGameLayer>();
            if (_exitGameLayer)
            {
                TakeLayerToBoxPosition(_exitGameLayer);
                addChild(_exitGameLayer);
            }
		}
	};
}
