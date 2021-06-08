#pragma once
#include "base/CCRef.h"
#include "base/CCRefPtr.h"

namespace cocos2d
{
class PhysicsContact;
class PhysicsContactPreSolve;
class PhysicsContactPostSolve;
}
class ContactHandler : public cocos2d::Ref
{
public:
	ContactHandler(class InGameScene* owner);


private:
	bool OnContactBegan(cocos2d::PhysicsContact& contact);
	bool OnContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
	void OnContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
	void OnContactSeparate(cocos2d::PhysicsContact& contact);
	class InGameScene* _inGameScene;
};