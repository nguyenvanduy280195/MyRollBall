#pragma once

#include "2d/CCNode.h"

namespace cocos2d
{
namespace ui
{
class ListView;
class Text;
}
}

class ScreenLog : public cocos2d::Node
{
public:
	bool init(class GameInfo* gameInfo);


	void AddLog(const std::string& msg);
private:
	void update(float);

	cocos2d::ui::ListView* MakeListView() const;
	cocos2d::ui::Text* MakeText(const std::string& text) const;
	void RemoveFirstItemAfterTime(float);
	void UpdatePosition();

	cocos2d::ui::ListView* _listView;
	float _removingTimer;
	class GameInfo* _gameInfo;
};