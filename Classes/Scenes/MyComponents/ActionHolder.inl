#include "ActionHolder.h"
#include "2d/CCAction.h"

template<class Owner>
inline ActionHolder<Owner>::~ActionHolder()
{
	if (_action)
	{
		_action->release();
	}
}

template<class Owner>
inline void ActionHolder<Owner>::SetAction(cocos2d::FiniteTimeAction* action)
{
	if (action == nullptr)
	{
		return;
	}
	if (_action != nullptr)
	{
		_action->release();
	}
	_action = action;
	_action->retain();
}

template<class Owner>
cocos2d::FiniteTimeAction* ActionHolder<Owner>::GetAction() const { return _action; }