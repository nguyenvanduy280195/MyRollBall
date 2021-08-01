#include "GameOverDialog.h"
#include "../MyCustomGUI.inl"
#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"
#include "2d/CCCamera.h"

bool GameOverDialog::init()
{
    if (!MyDialog::init("ui/dialog-game-over.tmx"))
    {
        return false;
    }

    auto cameraPosition = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPosition);

    TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "positions", [this](cocos2d::TMXObjectGroup* objectGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objectGroup, "game-over", [this](cocos2d::ValueMap& value)
		{
			auto x = value["x"].asFloat();
			auto y = value["y"].asFloat();
			auto src = value["src"].asString();
			auto scaleFactor0 = value["scale-factor-0"].asFloat();
			auto scaleFactor1 = value["scale-factor-1"].asFloat();
			auto duration = value["duration"].asFloat();

			auto sprite = cocos2d::Sprite::create(src);
			sprite->setPosition(x, y);
			sprite->setScale(scaleFactor0);

			auto action = cocos2d::ScaleTo::create(duration, scaleFactor1);
			sprite->runAction(action);

			_tiledMap->addChild(sprite);
		});
	});


    return true;
}

void GameOverDialog::SetOnMainMenuButtonPressed(const std::function<void(cocos2d::Ref*)> & callback) {
	AddCallbackToButton("main-menu", callback);
}

void GameOverDialog::SetOnReplayButtonPressed(const std::function<void(cocos2d::Ref*)> & callback) {
	AddCallbackToButton("replay", callback);
}
