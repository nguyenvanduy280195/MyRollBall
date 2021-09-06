#include "Level.h"
#include "2d/CCSprite.h"
#include "Bitmask.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "ScreenLog/ScreenLog.h"
#include "Utils/TMXUtil.h"
#include "json/document.h"
#include "Utils/StaticMethods.h"

using Super = cocos2d::TMXTiledMap;
using Vec2 = cocos2d::Vec2;
using Size = cocos2d::Size;

bool Level::init(const std::string& filename)
{
	if (!Super::initWithTMXFile(filename))
	{
		CCLOG("Loading %s fails", filename.c_str());
		return false;
	}

	auto body = MakeBody();
	setPhysicsBody(body);

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "positions", [this](cocos2d::ValueMap& value)
	{
		const auto name = value["name"].asString();
		if (name.compare("start") == 0)
		{
			auto x = value["x"].asFloat();
			auto y = value["y"].asFloat();
			_startPosition = Vec2(x, y);
		}
		else if (name.compare("goal") == 0)
		{
			auto goal = MakeGoal(value);
			addChild(goal);
		}
		else if (name.compare("key") == 0)
		{
			auto key = MakeKey(value);
			addChild(key);
		}
	});

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "carrots", [this](cocos2d::ValueMap& value)
	{
		auto carrot = MakeCarrot(value);
		addChild(carrot);
	});

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "lasers", [this](cocos2d::ValueMap& value)
	{
		auto laserShooter = MakeLaserShooter(value);
		addChild(laserShooter);
	});

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "rotating-blocks", [this](cocos2d::ValueMap& value)
	{
		auto block = MakeRotatingBlock(value);
		addChild(block);
	});

	TMXUtil::RequireTMXObjectGroupNotFound(this, "spikes_collision", [this](cocos2d::TMXObjectGroup* objectGroup)
	{
		auto spikesBody = cocos2d::PhysicsBody::create();
		spikesBody->setDynamic(false);

		auto objects = objectGroup->getObjects();
		for (auto& object : objects)
		{
			auto shape = MakeSpikeBodyShape(object.asValueMap());
			shape->setRestitution(10.f);
			spikesBody->addShape(shape);
		}

		spikesBody->setCategoryBitmask(SPIKE_CATEGORY_BITMASK);
		spikesBody->setCollisionBitmask(SPIKE_COLLISION_BITMASK);
		spikesBody->setContactTestBitmask(SPIKE_CONTACT_TEST_BITMASK);

		// just a sprite stores all spikes' collision
		auto spikes = cocos2d::Sprite::create();
		spikes->setPhysicsBody(spikesBody);
		addChild(spikes);
	});

	return true;
}

cocos2d::Vec2 Level::GetStartPosition() const
{
	return _startPosition;
}

cocos2d::Vec2 Level::GetGoalPosition() const
{
	if (auto goal = getChildByName("goal"))
	{
		return goal->getPosition();
	}

	return Vec2::ZERO;
}

cocos2d::PhysicsBody* Level::MakeBody()
{
	auto body = cocos2d::PhysicsBody::create();

	auto objectGroup = getObjectGroup("walls");
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

cocos2d::Sprite* Level::MakeSpriteWithBoxBodyShape(cocos2d::ValueMap& value)
{
	auto name = value["name"].asString();
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();

	auto document = StaticMethods::GetJSONFromFile(src);
	auto srcImage = document["path"].GetString();
	auto anchorPointX = document["anchor.point.x"].GetFloat();
	auto anchorPointY = document["anchor.point.y"].GetFloat();
	auto bodyWidth = document["body.size.width"].GetFloat();
	auto bodyHeight = document["body.size.height"].GetFloat();
	auto bodyOffsetX = document["body.offset.x"].GetFloat();
	auto bodyOffsetY = document["body.offset.y"].GetFloat();

	auto body = cocos2d::PhysicsBody::createBox(Size(bodyWidth, bodyHeight), cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, Vec2(bodyOffsetX, bodyOffsetY));
	body->setDynamic(false);

	auto sprite = cocos2d::Sprite::create(srcImage);
	sprite->setName(name);
	sprite->setAnchorPoint(Vec2(anchorPointX, anchorPointY));
	sprite->setPosition(x, y);
	sprite->setPhysicsBody(body);

	return sprite;
}

cocos2d::Sprite* Level::MakeSpriteWithCircleBodyShape(cocos2d::ValueMap& value)
{
	auto name = value["name"].asString();
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();

	auto document = StaticMethods::GetJSONFromFile(src);
	auto srcImage = document["path"].GetString();
	auto anchorPointX = document["anchor.point.x"].GetFloat();
	auto anchorPointY = document["anchor.point.y"].GetFloat();
	auto radius = document["body.radius"].GetFloat();
	auto bodyOffsetX = document["body.offset.x"].GetFloat();
	auto bodyOffsetY = document["body.offset.y"].GetFloat();

	auto body = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, Vec2(bodyOffsetX, bodyOffsetY));
	body->setDynamic(false);

	auto sprite = cocos2d::Sprite::create(srcImage);
	sprite->setName(name);
	sprite->setAnchorPoint(Vec2(anchorPointX, anchorPointY));
	sprite->setPosition(x, y);
	sprite->setPhysicsBody(body);

	return sprite;
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
	auto goal = MakeSpriteWithCircleBodyShape(value);
	Locked = value["locked"].asBool();

	auto body = goal->getPhysicsBody();
	body->setCategoryBitmask(GOAL_CATEGORY_BITMASK);
	body->setCollisionBitmask(GOAL_COLLISION_BITMASK);
	body->setContactTestBitmask(GOAL_CONTACT_TEST_BITMASK);

	return goal;
}

cocos2d::Sprite* Level::MakeKey(cocos2d::ValueMap& value)
{
	auto key = MakeSpriteWithBoxBodyShape(value);

	auto body = key->getPhysicsBody();
	body->setCategoryBitmask(KEY_CATEGORY_BITMASK);
	body->setCollisionBitmask(KEY_COLLISION_BITMASK);
	body->setContactTestBitmask(KEY_CONTACT_TEST_BITMASK);

	return key;
}

cocos2d::Sprite* Level::MakeLaserShooter(cocos2d::ValueMap& value)
{
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();
	auto direction = value["direction"].asString();

	auto laserShooter = cocos2d::Sprite::create(src);
	laserShooter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	laserShooter->setPosition(x, y);
	laserShooter->setLocalZOrder(500);
	if (direction.compare("right") == 0)
	{
		laserShooter->setRotation(90);
	}
	else if (direction.compare("bottom") == 0)
	{
		laserShooter->setRotation(180);
	}
	else if (direction.compare("left") == 0)
	{
		laserShooter->setRotation(-90);
	}

	auto laserBeam = MakeLaserBeam(value, 0.5f * laserShooter->getContentSize());
	laserShooter->addChild(laserBeam);

	return laserShooter;
}

cocos2d::Sprite* Level::MakeRotatingBlock(cocos2d::ValueMap& value)
{
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src"].asString();
	auto angle = value["angle"].asFloat();
	auto rotatingBlock = cocos2d::Sprite::create(src);
	rotatingBlock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rotatingBlock->setPosition(x, y);
	rotatingBlock->setLocalZOrder(500);
	if (angle == 0.0f)
	{
		angle = 30 * cocos2d::RandomHelper::random_real(-10.f, 10.f);
	}

	if (angle > 0)
	{
		rotatingBlock->setFlippedX(true);
	}
	rotatingBlock->runAction(cocos2d::RepeatForever::create(cocos2d::RotateBy::create(1, angle)));

	auto body = cocos2d::PhysicsBody::createBox(rotatingBlock->getContentSize());
	body->setDynamic(false);
	rotatingBlock->setPhysicsBody(body);

	return rotatingBlock;
}

cocos2d::Sprite* Level::MakeCarrot(cocos2d::ValueMap& value)
{
	auto carrot = MakeSpriteWithBoxBodyShape(value);
	carrot->setName("carrot");
	auto body = carrot->getPhysicsBody();
	body->setCategoryBitmask(CARROT_CATEGORY_BITMASK);
	body->setCollisionBitmask(CARROT_COLLISION_BITMASK);
	body->setContactTestBitmask(CARROT_CONTACT_TEST_BITMASK);

	return carrot;
}

cocos2d::PhysicsShape* Level::MakeSpikeBodyShape(cocos2d::ValueMap& value)
{
	const auto name = value["name"].asString();
	const auto width = value["width"].asFloat();
	const auto height = value["height"].asFloat();
	const auto xOffset = value["x"].asFloat() + 0.5f * width;
	const auto yOffset = value["y"].asFloat() + 0.5f * height;
	const auto size = Size(width, height);
	const auto offset = Vec2(xOffset, yOffset);

	return cocos2d::PhysicsShapeEdgeBox::create(size, cocos2d::PHYSICSSHAPE_MATERIAL_DEFAULT, 0.0f, offset);
}

cocos2d::Sprite* Level::MakeLaserBeam(cocos2d::ValueMap& value, const cocos2d::Vec2& position)
{
	auto src = value["src-laser"].asString();
	const float laserScaleFactorY = value["laser-scale-factor-y"].asFloat();
	const float durationOfFadeIn = value["laser-scale-factor-y"].asFloat();
	const float durationOfFadeInDelay = value["laser-scale-factor-y"].asFloat();
	const float durationOfFadeOut = value["laser-scale-factor-y"].asFloat();
	const float durationOfFadeOutDelay = value["laser-scale-factor-y"].asFloat();


	auto laserBeam = cocos2d::Sprite::create(src);
	laserBeam->setScaleY(laserScaleFactorY);
	laserBeam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	laserBeam->setPosition(position);

	auto body = MakeLaserBeamBody(laserBeam->getContentSize());
	laserBeam->setPhysicsBody(body);

	auto action = MakeLaserBeamAction(body, durationOfFadeIn, durationOfFadeInDelay, durationOfFadeOut, durationOfFadeOutDelay);
	laserBeam->runAction(action);

	return laserBeam;
}

cocos2d::PhysicsBody* Level::MakeLaserBeamBody(const cocos2d::Size& size)
{
	auto body = cocos2d::PhysicsBody::createBox(size);
	body->setDynamic(false);
	body->setCategoryBitmask(LASER_CATEGORY_BITMASK);
	body->setCollisionBitmask(LASER_COLLISION_BITMASK);
	body->setContactTestBitmask(LASER_CONTACT_TEST_BITMASK);

	return body;
}

/*
*		 fade out			delay			fade in			 delay
* shown ----------> hidden -------> hidden ---------> shown ------->
*/
cocos2d::Action* Level::MakeLaserBeamAction(cocos2d::PhysicsBody* body,
											float durationOfFadeIn,
											float durationOfFadeInDelay,
											float durationOfFadeOut,
											float durationOfFadeOutDelay)
{
	auto setBodyEnalbled = [body](bool enabled)
	{
		if (body)
		{
			body->setEnabled(enabled);
		}
	};

	auto fadeIn = cocos2d::FadeIn::create(durationOfFadeIn);
	auto onBodyEnabled = cocos2d::CallFunc::create([setBodyEnalbled]() { setBodyEnalbled(true); });
	auto delay = [](float seconds) { return cocos2d::DelayTime::create(seconds); };
	auto onBodyDisabled = cocos2d::CallFunc::create([setBodyEnalbled]() { setBodyEnalbled(false); });
	auto fadeOut = cocos2d::FadeOut::create(durationOfFadeOut);

	auto sequence = cocos2d::Sequence::create(onBodyEnabled, fadeIn, delay(durationOfFadeInDelay), fadeOut, onBodyDisabled, delay(durationOfFadeOutDelay), nullptr);
	return cocos2d::RepeatForever::create(sequence);
}