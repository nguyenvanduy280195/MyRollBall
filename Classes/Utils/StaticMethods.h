#pragma once

#include "base/ccTypes.h"

namespace cocos2d
{
class Scene;
}

class StaticMethods
{
public:
	static cocos2d::Color3B MakeColor3BFromHex(const std::string hex);
	static void ReplaceScene(cocos2d::Scene* scene, cocos2d::Scene* sceneWithTransition);
	static void ReplaceScene(cocos2d::Scene*);
	static void PopScene();
};
