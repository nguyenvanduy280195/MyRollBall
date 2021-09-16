#include "ChoosingLevelLayer.h"

#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"
#include "platform/CCFileUtils.h"

#include "../MyCustomGUI.inl"
#include "../IntroLevelScene.h"

#include "Utils/TMXUtil.h"
#include "Utils/Cocos2dCreator.h"
#include "Utils/TMXUtil.h"

bool ChoosingLevelLayer::init(const cocos2d::Vec2& boxPosition, const std::string& tmxPath)
{
	if (!MyLayer::init(tmxPath))
	{
		return false;
	}

	_tiledMap->setPosition(boxPosition);
	_tiledMap->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	{
		auto children = _tiledMap->getChildren();
		for (auto& child : children)
		{
			if (child->getName().find("level") != std::string::npos)
			{
				if (auto button = dynamic_cast<cocos2d::ui::Button*>(child))
				{
					auto level = stoi(button->getTitleText());
					button->addClickEventListener([this, level](cocos2d::Ref*){ StartLevel(level); });

					auto levelPath = cocos2d::StringUtils::format("levels/%03d.tmx", level);
					if (!cocos2d::FileUtils::getInstance()->isFileExist(levelPath))
					{
						button->setEnabled(false);
					}
				}
			}
		}
	}
	return true;
}

bool ChoosingLevelLayer::init(const cocos2d::Vec2& boxPosition) { return init(boxPosition, "ui/dialog-choosing-level-0.tmx"); }

void ChoosingLevelLayer::StartLevel(int level)
{
	auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(level);
	cocos2d::Director::getInstance()->pushScene(scene);
}