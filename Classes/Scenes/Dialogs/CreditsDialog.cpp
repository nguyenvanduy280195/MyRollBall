#include "CreditsDialog.h"

#include "ui/UIVBox.h"
#include "ui/UIText.h"
#include "ui/UILayout.h"
#include "2d/CCSprite.h"
#include "../MyCustomGUI.inl"

bool CreditsDialog::init(const cocos2d::Vec2& boxPosition)
{
	if (!MyDialog::init("ui/dialog-credits.tmx"))
	{
		return false;
	}

	_tiledMap->setPosition(boxPosition);

	AddCallbackToButton("close", [this](cocos2d::Ref*){ setVisible(false); });

	AddSpriteFromTMXObject("image-texts", "credits");
	AddSpriteFromTMXObject("image-texts", "programmer");
	AddSpriteFromTMXObject("image-texts", "programmer-name");
	AddSpriteFromTMXObject("image-texts", "artist");
	AddSpriteFromTMXObject("image-texts", "artist-name");
	
	return true;
}

void CreditsDialog::AddSpriteFromTMXObject(const std::string& objectGroupName, const std::string& objectName)
{
	TMXUtil::RequireTMXObjectGroupNotFound(_tiledMap, objectGroupName, [this, objectName](cocos2d::TMXObjectGroup* objGroup)
	{
		TMXUtil::RequireTMXObjectNotFound(objGroup, objectName, [this](cocos2d::ValueMap& value)
		{
			const auto x = value["x"].asFloat();
			const auto y = value["y"].asFloat();
			const auto src = value["src"].asString();

			cocos2d::Sprite* sprite = cocos2d::Sprite::create(src);
			sprite->setPosition(x, y);
			_tiledMap->addChild(sprite);
		});
	});
}