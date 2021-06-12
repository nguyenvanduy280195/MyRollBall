#pragma once

#include <string>

namespace cocos2d
{
class Label;
}

class ScreenLogMessage
{
	friend class ScreenLog;

	class ScreenLog* _layer;
	int _level;
	std::string _text;
	float _timestamp;
	cocos2d::Label* _label;
	bool _dirty;

	ScreenLogMessage(class ScreenLog* layer)
	{
		_layer = layer;
		_label = nullptr;
		_level = 0;
		_timestamp = 0;
		_dirty = true;
	}

	void SetLabelText(const std::string& msg);
	void CreateLabel();
	bool CheckLabel();
};