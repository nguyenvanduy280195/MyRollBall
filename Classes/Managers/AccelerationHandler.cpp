#include "AccelerationHandler.h"
#include "2d/CCNode.h"
#include "platform/CCDevice.h"
#include "base/CCEventListenerAcceleration.h"
#include "base/CCEventDispatcher.h"

AccelerationHandler::AccelerationHandler(cocos2d::Node* owner)
{
	cocos2d::Device::setAccelerometerEnabled(true);
	auto listener = cocos2d::EventListenerAcceleration::create([this](cocos2d::Acceleration* acceleration, cocos2d::Event* event)
	{
		if (onAcceleration)
		{
			onAcceleration(acceleration);
		}
	});

	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, owner);
}