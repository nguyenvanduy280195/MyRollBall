#include "ChoosingLevelDialog.h"
#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"
#include "Utils/TMXUtil.h"
#include "../MyCustomGUI.inl"
#include "Utils/Cocos2dCreator.h"
#include "../IntroLevelScene.h"

bool ChoosingLevelDialog::init(const cocos2d::Vec2& boxPosition)
{
	if (!MyDialog::init("ui/dialog-choosing-level.tmx"))
	{
		return false;
	}

	AddCallbackToButton("close", [this](cocos2d::Ref*) { Hide(); });

	_tiledMap->setPosition(boxPosition);

	for (int i = 0; i < NUMBER_OF_PAGE_IN_DIALOG; i++)
	{
		const auto name = cocos2d::StringUtils::format("page#%d", i);
		AddCallbackToButton(name, MakePageNumberButtonCallback(i));
	}

	// init page #0
	{
		int i = 1;
		auto children = _tiledMap->getChildren();
		for (auto& child : children)
		{
			if (child->getName().find("level") != std::string::npos)
			{
				if (auto button = dynamic_cast<cocos2d::ui::Button*>(child))
				{
					int level = i;
					button->setTitleText(std::to_string(level));
					button->addClickEventListener([this, level](cocos2d::Ref*)
					{
						StartLevel(level);
					});
					i++;
				}
			}
		}
	}
	return true;
}

std::function<void(cocos2d::Ref*)> ChoosingLevelDialog::MakePageNumberButtonCallback(int pageNumber)
{
	return [this, pageNumber](cocos2d::Ref*)
	{
		int i = 1;
		auto children = _tiledMap->getChildren();
		for (auto& child : children)
		{
			if (child->getName().find("level") != std::string::npos)
			{
				if (auto button = dynamic_cast<cocos2d::ui::Button*>(child))
				{
					int level = pageNumber * NUMBER_OF_LEVEL_IN_PAGE + i;
					button->setTitleText(std::to_string(level));
					button->addClickEventListener([this, level](cocos2d::Ref*)
					{
						StartLevel(level);
					});
					i++;
				}
			}
		}
	};
}

void ChoosingLevelDialog::StartLevel(int level)
{
	auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(level);
	cocos2d::Director::getInstance()->pushScene(scene);
}