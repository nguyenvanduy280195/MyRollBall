#include "math/Vec2.h"

namespace cocos2d
{
class Node;
}


// I am confused when I don't do this.
// The reason I use this interface to separate Player methods and cocos2d::Node methods
class IPlayer
{
public:
	virtual void Break() = 0;
	virtual void Dash() = 0;
	virtual void MoveToCenterGoal(const cocos2d::Vec2& goalPosition, const std::function<void()>& onMovingDone) = 0;
	virtual void RemoveBodyFromWorld() = 0;
	virtual cocos2d::Node* AsNode() const = 0;
};