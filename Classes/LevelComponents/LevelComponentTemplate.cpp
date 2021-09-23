#include "LevelComponentTemplate.h"
#include "Utils/StaticMethods.h"
#include "2d/CCActionInterval.h"
#include "2d/CCSpriteFrameCache.h"
#include "Bitmask.h"

using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

bool LevelComponentTemplate::init(cocos2d::ValueMap& value)
{
	if (value.empty())
	{
		return false;
	}

	const auto srcJSON = value["src"].asString();
	_document = StaticMethods::GetJSONFromFile(srcJSON);
	if (_document.HasParseError())
	{
		CCLOG("[LevelComponentTemplate::init(...)] - Loading JSON failed");
		return false;
	}

	if (!InitWithSpriteSource() && !InitWithAnimation())
	{
		return false;
	}

	//TODO localZOrder

	const auto x = value["x"].asFloat();
	const auto y = value["y"].asFloat();
	setPosition(x, y);

	const auto name = value["name"].asString();
	setName(name);

	const auto anchorPointX = _document["anchorPoint.x"].GetFloat();
	const auto anchorPointY = _document["anchorPoint.y"].GetFloat();
	setAnchorPoint(Vec2(anchorPointX, anchorPointY));

	if (auto body = CreateBody())
	{
		setPhysicsBody(body);
	}

	if (auto action = CreateAction())
	{
		runAction(action);
	}

	return true;
}

bool LevelComponentTemplate::InitWithSpriteSource()
{
	if (!_document.HasMember("sprite.imagePath"))
	{
		CCLOG("[LevelComponentTemplate::InitWithSpriteSource()] - Creating sprite failed");
		return false;
	}

	auto srcImage = _document["sprite.imagePath"].GetString();
	if (!cocos2d::Sprite::initWithFile(srcImage))
	{
		CCLOG("[LevelComponentTemplate::InitWithSpriteSource()] - Creating sprite failed");
		return false;
	}

	return true;
}

bool LevelComponentTemplate::InitWithAnimation()
{
	auto srcImage = _document["animation.imagePath"].GetString();
	auto srcFormat = _document["animation.plistPath"].GetString();
	StaticMethods::OpenSpritesheet open(srcFormat, srcImage);

	auto animationFormatName = _document["animation.formatName"].GetString();
	auto animation0Name = cocos2d::StringUtils::format(animationFormatName, 0);
	if (!cocos2d::Sprite::initWithSpriteFrameName(animation0Name))
	{
		CCLOG("[LevelComponentTemplate::InitWithAnimation()] - Creating sprite failed");
		return false;
	}

	return true;
}

cocos2d::PhysicsBody* LevelComponentTemplate::CreateBody()
{
	const auto bodyType = _document["body.type"].GetString();

	cocos2d::PhysicsBody* body = nullptr;
	if (std::string(bodyType).compare("circle") == 0)
	{
		body = CreateCircleBody();
	}
	else if (std::string(bodyType).compare("box") == 0)
	{
		body = CreateBoxBody();
	}
	else
	{
		return nullptr;
	}

	SetBitmask(body);

	return body;
}

cocos2d::PhysicsBody* LevelComponentTemplate::CreateBoxBody()
{
	auto bodyWidth = _document["body.size.width"].GetFloat();
	auto bodyHeight = _document["body.size.height"].GetFloat();
	auto bodyOffsetX = _document["body.offset.x"].GetFloat();
	auto bodyOffsetY = _document["body.offset.y"].GetFloat();
	auto body = cocos2d::PhysicsBody::createBox(Size(bodyWidth, bodyHeight), cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, Vec2(bodyOffsetX, bodyOffsetY));
	body->setDynamic(false);
	return body;
}

cocos2d::PhysicsBody* LevelComponentTemplate::CreateCircleBody()
{
	auto radius = _document["body.radius"].GetFloat();
	auto bodyOffsetX = _document["body.offset.x"].GetFloat();
	auto bodyOffsetY = _document["body.offset.y"].GetFloat();
	auto body = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, Vec2(bodyOffsetX, bodyOffsetY));
	body->setDynamic(false);
	return body;
}

cocos2d::Animate* LevelComponentTemplate::CreateAnimate() const
{
	auto animation = CreateAnimation();
	return cocos2d::Animate::create(animation);
}

cocos2d::Animation* LevelComponentTemplate::CreateAnimation() const
{
	auto srcImage = _document["animation.imagePath"].GetString();
	auto srcFormat = _document["animation.plistPath"].GetString();
	StaticMethods::OpenSpritesheet open(srcFormat, srcImage);

	auto nFrames = _document["animation.nFrames"].GetInt();
	auto animationDelay = _document["animation.delay"].GetFloat();
	auto animationFormatName = _document["animation.formatName"].GetString();

	cocos2d::Vector<cocos2d::SpriteFrame*> frames;
	frames.reserve(nFrames);
	for (int i = 1; i < nFrames; i++)
	{
		auto name = cocos2d::StringUtils::format(animationFormatName, i);
		auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		frames.pushBack(spriteFrame);
	}

	return cocos2d::Animation::createWithSpriteFrames(frames, animationDelay);
}