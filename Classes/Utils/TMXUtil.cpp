#include "TMXUtil.h"
#include "2d/CCTMXTiledMap.h"
#include "ScreenLog/ScreenLog.h"

using Vec2 = cocos2d::Vec2;

void TMXUtil::RequireTMXObjectGroupNotFound(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::TMXObjectGroup*)>& action)
{
	auto objectGroup = tiledMap->getObjectGroup(name);
	if (objectGroup)
	{
		action(objectGroup);
	}
	else
	{
		//gScreenLog->Log(LL_WARNING, "`%s` object group not found", name.c_str());
	}
}

void TMXUtil::RequireTMXObjectNotFound(cocos2d::TMXObjectGroup* objectGroup, const std::string& name, const std::function<void(cocos2d::ValueMap& value)>& action)
{
	auto valueMap = objectGroup->getObject(name);
	if (valueMap != cocos2d::ValueMap())
	{
		action(valueMap);
	}
	else
	{
		//gScreenLog->Log(LL_WARNING, "`%s` object not found", name.c_str());
	}
}

void TMXUtil::ForeachAllObjectsInObjectGroup(cocos2d::TMXTiledMap* tiledMap, const std::string& name, const std::function<void(cocos2d::ValueMap&)>& action)
{
	RequireTMXObjectGroupNotFound(tiledMap, name, [action](cocos2d::TMXObjectGroup* objGroup)
	{
		auto objects = objGroup->getObjects();
		for (auto& object : objects)
		{
			action(object.asValueMap());
		}
	});
}
