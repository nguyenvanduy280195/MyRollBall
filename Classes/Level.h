#pragma once

#include "2d/CCTMXTiledMap.h"

namespace cocos2d
{
class PhysicsBody;
}

class Level : public cocos2d::TMXTiledMap
{
	
public:
	bool init(const std::string& filename);

	cocos2d::Vec2 GetStartPosition() const;
	cocos2d::Vec2 GetEndPosition() const;

	std::unordered_map<std::string, int> nameTagMap;
private:
	cocos2d::PhysicsBody* MakeBody();
	
};