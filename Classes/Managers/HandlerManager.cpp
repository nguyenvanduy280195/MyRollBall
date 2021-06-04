#include "HandlerManager.h"
#include "KeyboardHandler.h"
#include "AccelerationHandler.h"
#include "Scenes/InGameScene.h"


HandlerManager::HandlerManager(InGameScene* scene)
{
	keyboardHandler = new(std::nothrow) KeyboardHandler(scene);
	accelerationHandler = new(std::nothrow) AccelerationHandler(scene);
}

HandlerManager::~HandlerManager()
{
	keyboardHandler->release();
	accelerationHandler->release();
}
