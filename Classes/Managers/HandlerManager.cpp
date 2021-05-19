#include "HandlerManager.h"
#include "KeyboardHandler.h"
#include "Scenes/InGameScene.h"

HandlerManager::HandlerManager(InGameScene* scene)
{
	keyboardHandler = new(std::nothrow) KeyboardHandler();
	keyboardHandler->Setup(scene);
}

HandlerManager::~HandlerManager()
{
	keyboardHandler->release();
}
