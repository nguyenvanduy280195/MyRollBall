#pragma once

#include "base/ccTypes.h"
#include "base/CCValue.h"


namespace cocos2d
{
class Scene;
class TMXTiledMap;
class TMXObjectGroup;
}

class StaticMethods
{
public:
	static cocos2d::Color3B MakeColor3BFromHex(const std::string hex);
	static void ReplaceScene(cocos2d::Scene* scene, cocos2d::Scene* sceneWithTransition);
	static void ReplaceScene(cocos2d::Scene*);
	static void PopScene();

	template<class TObj>
	static void RequireObjectNotNull(TObj* obj, const std::function<void(TObj*)>& action)
	{
		if (obj)
		{
			action(obj);
		}
		else
		{
			CCLOG("obj is nullptr");
		}
	}
};
