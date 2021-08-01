#include "ScreenLogMessage.h"
#include "ScopeLock.h"
#include "ScreenLog.h"
#include "2d/CCLabel.h"
namespace another_author
{
void ScreenLogMessage::SetLabelText(const std::string& msg)
{
	// can be called from other threads, delay label creation to main thread to make sure OpenGL works
	ScopeLock lock(&_layer->_contentMutex);

	_text = msg;
	_dirty = true;
}

void ScreenLogMessage::CreateLabel()
{
	float screenHeightPixels = cocos2d::Director::getInstance()->getWinSize().height;
	float fontSize = screenHeightPixels / (float)_layer->SCREENLOG_NUM_LINES - 1;
	_label = cocos2d::Label::createWithTTF(_text, _layer->_fontFile, fontSize);
	_label->setAnchorPoint(cocos2d::Point(0, 0));

	switch (_level)
	{
	case LL_TRACE:
	case LL_DEBUG:
		_label->setColor(cocos2d::Color3B(255, 255, 255));
		break; // white
	case LL_INFO:
		_label->setColor(cocos2d::Color3B(32, 255, 32));
		break; // green
	case LL_WARNING:
		_label->setColor(cocos2d::Color3B(255, 127, 32));
		break; // orange
	default:
		_label->setColor(cocos2d::Color3B(255, 32, 32));
		break; // red
	}

	_label->setPosition(2, _layer->SCREENLOG_START_HEIGHT_PERCENT * screenHeightPixels);
	_layer->addChild(_label);
}

//returns true if label was created for the first time (other labels should be moved upward)
bool ScreenLogMessage::CheckLabel()
{
	if (!_label)
	{
		CreateLabel();
		_dirty = false;
		return true;
	}

	if (_dirty)
	{
		cocos2d::Point originalPos = _label->getPosition();
		_layer->removeChild(_label, true);
		_label = NULL;
		CreateLabel();
		_label->setPosition(originalPos);
		_dirty = false;
	}

	return false;
}
}