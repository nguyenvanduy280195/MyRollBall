#pragma once

#include "base/ccTypes.h"
#include "base/CCValue.h"
#include "json/document.h"
#include "2d/CCSpriteFrameCache.h"


namespace cocos2d
{
class Scene;
class TMXTiledMap;
class TMXObjectGroup;
}

class StaticMethods
{
public:
	static cocos2d::Color3B MakeColor3BFromHex(const std::string& hex);
	static void ReplaceScene(cocos2d::Scene* scene, cocos2d::Scene* sceneWithTransition);
	static void ReplaceScene(cocos2d::Scene*);
	static void PopScene();

	static rapidjson::Document GetJSONFromFile(const std::string filePath);
	static void WriteJSONOnFile(rapidjson::Document& document, const std::string filePath);
	

	template<class TObj>
	static void RequireObjectNotNull(TObj* obj, const std::function<void(TObj*)>& action)
	{
		if (obj)
		{
			action(obj);
		}
	}




	class OpenSpritesheet
	{
	public:
		OpenSpritesheet(const std::string& plist, const std::string& textureFileName)
		{
			cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, textureFileName);
		}

		~OpenSpritesheet()
		{
			cocos2d::SpriteFrameCache::destroyInstance();
		}
	};

};
