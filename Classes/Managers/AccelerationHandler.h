#pragma once

#include "base/CCRef.h"
#include <functional>

namespace cocos2d
{
class Node;
class Event;
class Acceleration;
}

class AccelerationHandler : public cocos2d::Ref
{
public:
	AccelerationHandler(cocos2d::Node* owner);
	std::function<void(cocos2d::Acceleration*)> onAcceleration;
};