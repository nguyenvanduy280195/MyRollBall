#pragma once

#include "2d/CCLayer.h"
#include "base/CCValue.h"
#include "MyDialog.h"

namespace cocos2d
{
namespace ui
{
class Button;
class Layout;
class Text;
}
}

class VictoryDialog : public MyDialog
{
public:
	bool init();
	void SetLevelTextContent(const std::string&);
	void SetTimeTextContent(const std::string&);
	void SetBestTimeTextContent(const std::string&);
	void SetOnNextButtonPressed(const std::function<void(cocos2d::Ref*)>&);
	void SetOnHomeButtonPressed(const std::function<void(cocos2d::Ref*)>&);
};
