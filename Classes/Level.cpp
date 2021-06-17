#include "Level.h"
#include "2d/CCSprite.h"
#include "Bitmask.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "ScreenLog/ScreenLog.h"

using Super = cocos2d::TMXTiledMap;
using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

bool Level::init(const std::string& filename)
{
	if (!Super::initWithTMXFile(filename))
	{
		return false;
	}

	auto body = MakeBody();
	setPhysicsBody(body);


	auto positionsObjects = getObjectGroup("positions");
	auto objects = positionsObjects->getObjects();
	for (auto& object : objects)
	{
		auto& valueMap = object.asValueMap();
		const auto name = valueMap["name"].asString();
		if (name.compare("start") == 0)
		{
			auto start = MakeStart(valueMap);
			addChild(start);
		}
		else if (name.compare("goal") == 0)
		{
			auto goal = MakeGoal(valueMap);
			addChild(goal);
		}
		else if (name.compare("key") == 0)
		{
			auto key = MakeKey(valueMap);
			addChild(key);
		}
		else if (name.compare("laser-shooter") == 0)
		{
			auto laserShooter = MakeLaserShooter(valueMap);
			addChild(laserShooter, 100);
		}
	}

	return true;
}

cocos2d::PhysicsBody* Level::MakeBody()
{
	auto body = cocos2d::PhysicsBody::create();

	auto objectGroup = getObjectGroup("meta");
	CCASSERT(objectGroup != nullptr, "objectGroup is null, so PhysicsBody has no shape");

	if (objectGroup)
	{
		auto objects = objectGroup->getObjects();
		for (const cocos2d::Value& object : objects)
		{
			auto valueMap = object.asValueMap();
			auto name = valueMap["name"].asString();
			auto width = valueMap["width"].asFloat();
			auto height = valueMap["height"].asFloat();
			auto xOffset = valueMap["x"].asFloat() - 0.5f * _contentSize.width + 0.5f * width;
			auto yOffset = valueMap["y"].asFloat() - 0.5f * _contentSize.height + 0.5f * height;
			auto size = Size(width, height);
			auto offset = Vec2(xOffset, yOffset);
			auto shape = cocos2d::PhysicsShapeEdgeBox::create(size, cocos2d::PHYSICSSHAPE_MATERIAL_DEFAULT, 0.0f, offset);
			body->addShape(shape);
		}
	}


	body->setCategoryBitmask(WALL_CATEGORY_BITMASK);
	body->setCollisionBitmask(WALL_COLLISION_BITMASK);
	body->setContactTestBitmask(WALL_CONTACT_TEST_BITMASK);
	body->setDynamic(false);

	return body;
}

std::vector<cocos2d::PhysicsShape*> Level::MakeWalls()
{
	std::vector<cocos2d::PhysicsShape*> shapes;


	auto objectGroup = getObjectGroup("meta");
	CCASSERT(objectGroup != nullptr, "objectGroup is null, so PhysicsBody has no shape");

	if (objectGroup)
	{
		auto objects = objectGroup->getObjects();
		for (const cocos2d::Value& object : objects)
		{
			auto valueMap = object.asValueMap();
			auto name = valueMap["name"].asString();
			auto width = valueMap["width"].asFloat();
			auto height = valueMap["height"].asFloat();
			auto xOffset = valueMap["x"].asFloat() - 0.5f * _contentSize.width + 0.5f * width;
			auto yOffset = valueMap["y"].asFloat() - 0.5f * _contentSize.height + 0.5f * height;
			auto size = Size(width, height);
			auto offset = Vec2(xOffset, yOffset);
			auto shape = cocos2d::PhysicsShapeEdgeBox::create(size, cocos2d::PHYSICSSHAPE_MATERIAL_DEFAULT, 0.0f, offset);

			shapes.push_back(shape);
		}
	}


	return shapes;
}

cocos2d::Sprite* Level::MakeStart(cocos2d::ValueMap& value)
{
	auto name = value["name"].asString();
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();

	auto sprite = cocos2d::Sprite::create(src);
	sprite->setName(name);
	sprite->setPosition(x, y);

	return sprite;
}

cocos2d::Sprite* Level::MakeGoal(cocos2d::ValueMap& value)
{
	auto name = value["name"].asString();
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();
	Locked = value["locked"].asBool();

	auto sprite = cocos2d::Sprite::create(src);
	sprite->setPosition(x, y);


	auto body = cocos2d::PhysicsBody::createCircle(0.25f * sprite->getContentSize().width);
	body->setDynamic(false);
	body->setCategoryBitmask(GOAL_CATEGORY_BITMASK);
	body->setCollisionBitmask(GOAL_COLLISION_BITMASK);
	body->setContactTestBitmask(GOAL_CONTACT_TEST_BITMASK);
	sprite->setPhysicsBody(body);


	return sprite;
}

cocos2d::Sprite* Level::MakeKey(cocos2d::ValueMap& value)
{
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();

	auto key = cocos2d::Sprite::create(src);
	key->setPosition(x, y);

	auto body = cocos2d::PhysicsBody::createCircle(0.5f * key->getContentSize().width);
	body->setDynamic(false);
	body->setCategoryBitmask(KEY_CATEGORY_BITMASK);
	body->setCollisionBitmask(KEY_COLLISION_BITMASK);
	body->setContactTestBitmask(KEY_CONTACT_TEST_BITMASK);

	key->setPhysicsBody(body);

	return key;
}

cocos2d::Sprite* Level::MakeLaserShooter(cocos2d::ValueMap& value)
{
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();
	auto direction = value["direction"].asString();
	auto srcLaser = value["src-laser"].asString();
	auto laserScaleFactorY = value["laser-scale-factor-y"].asFloat();

	auto laserShooter = cocos2d::Sprite::create(src);
	laserShooter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	laserShooter->setPosition(x, y);
	laserShooter->setName("laser-shooter");
	if (direction.compare("right") == 0)
	{
		laserShooter->setRotation(90);
	}

	auto laser = cocos2d::Sprite::create(srcLaser);
	laserShooter->addChild(laser);

	laser->setScaleY(laserScaleFactorY);
	laser->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	laser->setPosition(0.5f * laserShooter->getContentSize());
	laser->setName("laser");

	auto body = cocos2d::PhysicsBody::createBox(laser->getContentSize());
	body->setDynamic(false);
	body->setCategoryBitmask(LASER_CATEGORY_BITMASK);
	body->setCollisionBitmask(LASER_COLLISION_BITMASK);
	body->setContactTestBitmask(LASER_CONTACT_TEST_BITMASK);

	laser->setPhysicsBody(body);



	auto fadeIn = cocos2d::FadeIn::create(1);
	auto onBodyEnabled = cocos2d::CallFunc::create([this]()
	{
		if (auto laserShooter = getChildByName("laser-shooter"))
		{
			if (auto laser = laserShooter->getChildByName("laser"))
			{
				if (auto laserBody = laser->getPhysicsBody())
				{
					laserBody->setEnabled(true);
				}
			}
		}
	});
	auto delay = [](float seconds) { return cocos2d::DelayTime::create(seconds); };
	auto onBodyDisabled = cocos2d::CallFunc::create([this]()
	{
		if (auto laserShooter = getChildByName("laser-shooter"))
		{
			if (auto laser = laserShooter->getChildByName("laser"))
			{
				if (auto laserBody = laser->getPhysicsBody())
				{
					laserBody->setEnabled(false);
				}
			}
		}
	});
	auto fadeOut = cocos2d::FadeOut::create(1);
	auto sequence = cocos2d::Sequence::create(onBodyEnabled, fadeIn, delay(1), fadeOut, onBodyDisabled, delay(3), nullptr);
	auto action = cocos2d::RepeatForever::create(sequence);

	laser->runAction(action);

	return laserShooter;
}

cocos2d::Vec2 Level::GetStartPosition() const
{
	if (auto start = getChildByName("start"))
	{
		return start->getPosition();
	}

	return Vec2::ZERO;
}
