#pragma once

#include "base/ccTypes.h"
#include "base/CCValue.h"
#include "json/document.h"

namespace cocos2d
{
class Scene;
class Node;
}

class StaticMethods
{
public:
	static cocos2d::Color3B MakeColor3BFromHex(const std::string& hex);

	static rapidjson::Document GetJSONFromFile(const std::string filePath);
	static void WriteJSONOnFile(rapidjson::Document& document, const std::string filePath);
	
	static void DrawAnchorPoint(cocos2d::Node*);
	static void DrawBorder(cocos2d::Node*);

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
		OpenSpritesheet(const std::string& plist, const std::string& textureFileName);
		virtual ~OpenSpritesheet();
	};

};
