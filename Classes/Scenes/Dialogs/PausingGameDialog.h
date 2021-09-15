#pragma once

#include "MyDialog.h"

class PausingGameDialog : public MyDialog
{
public:
	bool init();
	std::function<void()> OnEventPaused;
	std::function<void()> OnEventUnpaused;
};