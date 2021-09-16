#include "CoinCreator.h"

#include "2d/CCSprite.h"
#include "2d/CCActionInterval.h"

#include "Utils/StaticMethods.h"
#include "Bitmask.h"

using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

CoinCreator::CoinCreator(cocos2d::ValueMap& value)
{
	_x = value["x"].asFloat();
	_y = value["y"].asFloat();
	_jsonPath = value["src"].asString();
	_info = LoadInfoFromJSON();
}

cocos2d::Sprite* CoinCreator::CreateSprite()
{
	StaticMethods::OpenSpritesheet openCoinSpritesheet(_info.plistPath, _info.imagePath);
	auto coin = cocos2d::Sprite::createWithSpriteFrameName("coin-0.png");

	coin->setName("coin");
	coin->setAnchorPoint(Vec2(_info.anchorPointX, _info.anchorPointY));
	coin->setPosition(_x, _y);
	coin->setPhysicsBody(CreateBody());

	return coin;
}

cocos2d::Animate* CoinCreator::CreateAnimateAction() const
{
	auto frames = CreateSpriteFrames();
	auto animation = cocos2d::Animation::createWithSpriteFrames(frames, _info.animationDelay);
	return cocos2d::Animate::create(animation);
}

CoinCreator::Info CoinCreator::LoadInfoFromJSON()
{
	auto document = StaticMethods::GetJSONFromFile(_jsonPath);

	Info info;
	info.imagePath = document["path.image"].GetString();
	info.plistPath = document["path.plist"].GetString();
	info.animationDelay = document["animation.delay"].GetFloat();
	info.nFrames = document["frames.number"].GetInt();
	info.anchorPointX = document["anchor.point.x"].GetFloat();
	info.anchorPointY = document["anchor.point.y"].GetFloat();
	info.bodyWidth = document["body.size.width"].GetFloat();
	info.bodyHeight = document["body.size.height"].GetFloat();
	info.bodyOffsetX = document["body.offset.x"].GetFloat();
	info.bodyOffsetY = document["body.offset.y"].GetFloat();

	return info;
}

cocos2d::PhysicsBody* CoinCreator::CreateBody() const
{
	auto body = cocos2d::PhysicsBody::createBox(Size(_info.bodyWidth, _info.bodyHeight), cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, Vec2(_info.bodyOffsetX, _info.bodyOffsetY));
	body->setDynamic(false);
	body->setCategoryBitmask(COIN_CATEGORY_BITMASK);
	body->setCollisionBitmask(COIN_COLLISION_BITMASK);
	body->setContactTestBitmask(COIN_CONTACT_TEST_BITMASK);

	return body;
}

cocos2d::Vector<cocos2d::SpriteFrame*> CoinCreator::CreateSpriteFrames() const
{
	StaticMethods::OpenSpritesheet openCoinSpritesheet(_info.plistPath, _info.imagePath);

	cocos2d::Vector<cocos2d::SpriteFrame*> frames;
	frames.reserve(_info.nFrames);
	for (int i = 0; i < _info.nFrames; i++)
	{
		auto name = cocos2d::StringUtils::format("coin-%d.png", i);
		auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		frames.pushBack(spriteFrame);
	}

	return frames;
}