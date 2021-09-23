#include "KeyLevelComponent.h"
#include "Bitmask.h"

void KeyLevelComponent::SetBitmask(cocos2d::PhysicsBody* body) const
{
	body->setCategoryBitmask(KEY_CATEGORY_BITMASK);
	body->setCollisionBitmask(KEY_COLLISION_BITMASK);
	body->setContactTestBitmask(KEY_CONTACT_TEST_BITMASK);
}