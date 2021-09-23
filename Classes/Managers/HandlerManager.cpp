#include "HandlerManager.h"
#include "KeyboardHandler.h"
#include "AccelerationHandler.h"
#include "ContactHandler.h"

#include "Scenes/InGameScene.h"


HandlerManager::HandlerManager(InGameScene* scene)
{
	keyboardHandler = new(std::nothrow) KeyboardHandler(scene);
	accelerationHandler = new(std::nothrow) AccelerationHandler(scene);
	_contactHandler = new(std::nothrow) ContactHandler(scene);
}

HandlerManager::~HandlerManager()
{
	keyboardHandler->release();
	accelerationHandler->release();
	_contactHandler->release();
}
