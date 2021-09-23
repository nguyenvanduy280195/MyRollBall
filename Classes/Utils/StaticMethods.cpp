#include "StaticMethods.h"

#include "base/ccUTF8.h"
#include "base/CCDirector.h"

#include "2d/CCSpriteFrameCache.h"

#include "json/stringbuffer.h"
#include "json/prettywriter.h"

#include "ScreenLog/ScreenLog.h"

cocos2d::Color3B StaticMethods::MakeColor3BFromHex(const std::string& hex)
{
	if (hex.empty())
	{
		return cocos2d::Color3B::BLACK;
	}
	const auto r = hex.substr(1, 2);
	const auto rr = std::stoi(r, 0, 16);

	const auto g = hex.substr(3, 2);
	const auto gg = std::stoi(g, 0, 16);

	const auto b = hex.substr(5, 2);
	const auto bb = std::stoi(b, 0, 16);

	return cocos2d::Color3B(rr, gg, bb);
}

rapidjson::Document StaticMethods::GetJSONFromFile(const std::string filePath)
{
	auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);

	rapidjson::Document document;
	document.Parse(json.c_str());
	if (document.HasParseError())
	{
		CCLOG("[StaticMethods::GetJSONFromFile(...)][RapidJSON] - Error Code: %d", (int)document.GetParseError());
	}

	return document;
}

void StaticMethods::WriteJSONOnFile(rapidjson::Document& document, const std::string filePath)
{
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	auto newJsonFileContent = strbuf.GetString();
	cocos2d::FileUtils::getInstance()->writeStringToFile(newJsonFileContent, filePath);
}

void StaticMethods::DrawAnchorPoint(cocos2d::Node* node)
{
	const auto& anchorPoint = node->getAnchorPoint();
	const auto& size = node->getContentSize();

	auto drawNode = cocos2d::DrawNode::create();
	drawNode->drawDot(cocos2d::Vec2(size.width * anchorPoint.x, size.height * anchorPoint.y),
					  5,
					  cocos2d::Color4F::BLUE);
	node->addChild(drawNode);
}

void StaticMethods::DrawBorder(cocos2d::Node* node)
{
	const auto& size = node->getContentSize();
	auto drawNode = cocos2d::DrawNode::create();
	drawNode->drawRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(size.width, size.height), cocos2d::Color4F::RED);
	node->addChild(drawNode);
}

StaticMethods::OpenSpritesheet::OpenSpritesheet(const std::string& plist, const std::string& textureFileName)
{
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, textureFileName);
}

StaticMethods::OpenSpritesheet::~OpenSpritesheet()
{
	cocos2d::SpriteFrameCache::destroyInstance();
}