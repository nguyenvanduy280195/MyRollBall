#include "CoinLevelComponent.h"
#include "Bitmask.h"
#include "physics/CCPhysicsBody.h"
#include "2d/CCActionInterval.h"

void CoinLevelComponent::SetBitmask(cocos2d::PhysicsBody* body) const
{
	body->setCategoryBitmask(COIN_CATEGORY_BITMASK);
	body->setCollisionBitmask(COIN_COLLISION_BITMASK);
	body->setContactTestBitmask(COIN_CONTACT_TEST_BITMASK);
}

cocos2d::Action* CoinLevelComponent::CreateAction() const
{
	auto animate = CreateAnimate();
	return cocos2d::RepeatForever::create(animate);
}