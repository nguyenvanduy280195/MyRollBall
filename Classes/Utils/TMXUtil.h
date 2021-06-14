#pragma once
#include "base/CCValue.h"

namespace cocos2d
{
class TMXTiledMap;
class TMXObjectGroup;
}

class TMXUtil
{
public:
	static void RequireTMXObjectGroupNotFound(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::TMXObjectGroup*)>& action);
	static void RequireTMXObjectNotFound(cocos2d::TMXObjectGroup* objectGroup, const std::string& name, const std::function<void(cocos2d::ValueMap&)>& action);
	static void ForeachAllObjectsInObjectGroup(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::ValueMap&)>& action);
};