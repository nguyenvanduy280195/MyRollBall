#pragma once
#include <utility>

namespace cocos2d
{
class FiniteTimeAction;
}

template<class Owner>
class ActionHolder : public Owner
{
public:
	template<class ...TParams>
	bool init(TParams... params) { return Owner::init(std::forward<TParams>(params)...); }
	virtual ~ActionHolder();
	void SetAction(cocos2d::FiniteTimeAction* action);
	cocos2d::FiniteTimeAction* GetAction() const;

private:
	cocos2d::FiniteTimeAction* _action;
};