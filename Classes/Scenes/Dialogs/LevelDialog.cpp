#include "LevelDialog.h"
#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"
#include "Utils/TMXUtil.h"
#include "../MyCustomGUI.inl"
bool LevelDialog::init()
{
	if (!MyDialog::init("ui/dialog-victory-0.tmx"))
	{
		return false;
	}

	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, "positions", [this](cocos2d::TMXObjectGroup* objectGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objectGroup, "victory", [this](cocos2d::ValueMap& value)
		{
			auto x = value["x"].asFloat();
			auto y = value["y"].asFloat();
			auto src = value["src"].asString();

			auto sprite = cocos2d::Sprite::create(src);
			sprite->setPosition(x, y);
			sprite->setScale(10.f);

			auto action = cocos2d::ScaleTo::create(0.5f, 2.f);
			sprite->runAction(action);

			_tiledMap->addChild(sprite);
		});
	});

	return true;
}