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
	void CheckVictory(cocos2d::PhysicsContact& contact);
	void CheckGameOver(cocos2d::PhysicsContact& contact);
	void CheckToGetKey(cocos2d::PhysicsContact& contact);



	bool OnContactBegan(cocos2d::PhysicsContact& contact);
	bool OnContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
	void OnContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
	void OnContactSeparate(cocos2d::PhysicsContact& contact);

	class InGameScene* _inGameScene;
};