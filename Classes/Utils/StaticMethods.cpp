#include "StaticMethods.h"
#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "ScreenLog/ScreenLog.h"
#include "2d/CCTMXTiledMap.h"
#include "ScreenLog/ScreenLog.h"

cocos2d::Color3B StaticMethods::MakeColor3BFromHex(const std::string hex)
{
	const auto r = hex.substr(1, 2);
	const auto rr = std::stoi(r, 0, 16);

	const auto g = hex.substr(3, 2);
	const auto gg = std::stoi(g, 0, 16);

	const auto b = hex.substr(5, 2);
	const auto bb = std::stoi(b, 0, 16);

	return cocos2d::Color3B(rr, gg, bb);
}

void StaticMethods::ReplaceScene(cocos2d::Scene* scene, cocos2d::Scene* sceneWithTransition)
{
	cocos2d::Director::getInstance()->replaceScene(sceneWithTransition);
	//gScreenLog->AttachToScene(scene);
}

void StaticMethods::ReplaceScene(cocos2d::Scene* scene)
{
	cocos2d::Director::getInstance()->replaceScene(scene);
	//gScreenLog->AttachToScene(scene);
}

void StaticMethods::PopScene()
{
	cocos2d::Director::getInstance()->popScene();
	auto scene = cocos2d::Director::getInstance()->getRunningScene();
	//gScreenLog->AttachToScene(scene);
}
