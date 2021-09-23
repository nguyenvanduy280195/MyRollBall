#include "TouchSwallower.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerTouch.h"

template<typename TNode>
requires std::is_base_of_v<cocos2d::Node, TNode>
bool TouchSwallower<TNode>::init()
{
	if (!TNode::init())
	{
		return false;
	}

	InitTouchListener();

	return true;
}

template<class TNode>
requires std::is_base_of_v<cocos2d::Node, TNode>
template<class Param>
inline bool TouchSwallower<TNode>::init(const std::function<bool(Param)>& initFunc, Param param)
{
	if (!initFunc(param))
	{
		return false;
	}

	InitTouchListener();

	return true;
}

template<class TNode>
requires std::is_base_of_v<cocos2d::Node, TNode>
inline void TouchSwallower<TNode>::InitTouchListener()
{
	_touchListener = cocos2d::EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; };
	TNode::_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

template<typename TNode>
requires std::is_base_of_v<cocos2d::Node, TNode>
void TouchSwallower<TNode>::SetSwallowTouches(bool needSwallow)
{
	_touchListener->setSwallowTouches(needSwallow);
}
