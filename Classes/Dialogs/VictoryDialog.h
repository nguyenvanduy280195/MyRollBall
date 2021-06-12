#pragma once

#include "2d/CCLayer.h"
#include "ui/UILayoutParameter.h"
#include "base/CCValue.h"

namespace cocos2d
{

class TMXObjectGroup;
class TMXTiledMap;

namespace ui
{
class Button;
class Layout;
class Text;
}
}

class VictoryDialog : public cocos2d::Layer
{
public:
	bool init();
	void SetLevelTextContent(const std::string&);
	void SetTimeTextContent(const std::string&);
	void SetBestTimeTextContent(const std::string&);
	void SetOnNextButtonPressed(const std::function<void(cocos2d::Ref*)>&);
	void SetOnHomeButtonPressed(const std::function<void(cocos2d::Ref*)>&);
	
private:
	cocos2d::ui::Button* MakeButton(cocos2d::ValueMap& value);
	cocos2d::ui::Text* MakeText(cocos2d::ValueMap& value);

	void RequireObjectGroupNotFound(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::TMXObjectGroup*)>& action);
	void RequireObjectNotFound(cocos2d::TMXObjectGroup* objectGroup, const std::string& name, const std::function<void(cocos2d::ValueMap& value)>& action);
	void RequireNotNull(cocos2d::Node*, const std::function<void(cocos2d::Node*)>& action);

	cocos2d::ui::Button* _homeButton;
	cocos2d::ui::Button* _nextButton;
	cocos2d::ui::Text* _levelText;
	cocos2d::ui::Text* _timeText;
	cocos2d::ui::Text* _bestTimeText;
};
