#pragma once
#include "2d/CCNode.h"


namespace cocos2d
{
class EventListenerTouchOneByOne;
}

//template<class TNode>
//template<typename TNode, typename std::enable_if<std::is_base_of<cocos2d::Node, TNode>::value>::type* = nullptr>
template<class TNode>
requires std::is_base_of_v<cocos2d::Node, TNode>
class TouchSwallower : public TNode
{
public:

	void SetSwallowTouches(bool needSwallow);

protected:
	
	bool init();

	template<class Param>
	bool init(const std::function<bool(Param)>& initFunc, Param param);

private:
	void InitTouchListener();

	cocos2d::EventListenerTouchOneByOne* _touchListener;
};