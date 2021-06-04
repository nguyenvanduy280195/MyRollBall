#pragma once
#include "base/CCRef.h"

class HandlerManager : public cocos2d::Ref
{
public:
	HandlerManager(class InGameScene* scene);
	~HandlerManager();

	class KeyboardHandler* keyboardHandler;
	class AccelerationHandler* accelerationHandler;
};
