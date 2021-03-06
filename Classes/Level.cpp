#include "Level.h"

#include "2d/CCActionInterval.h"

#include "Bitmask.h"

#include "LevelComponents/Crossbow.h"
#include "LevelComponents/CoinCreator.h"
#include "LevelComponents/CoinLevelComponent.h"
#include "LevelComponents/KeyLevelComponent.h"
//#include "ScreenLog/ScreenLog.h"

#include "Utils/TMXUtil.h"
#include "Utils/StaticMethods.h"
#include "Utils/Cocos2dCreator.h"

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

	setPhysicsBody(MakeBody());

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
			auto key = Cocos2dCreator::CreateNode<KeyLevelComponent>(value);
			addChild(key);
		}
	});

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "coins", [this](cocos2d::ValueMap& value)
	{
		//CoinCreator coinCreator(value);
		//auto coin = coinCreator.CreateSprite();
		//addChild(coin);
		//auto animateAction = coinCreator.CreateAnimateAction();
		//coin->runAction(cocos2d::RepeatForever::create(animateAction));
		auto coin = Cocos2dCreator::CreateNode<CoinLevelComponent>(value);
		addChild(coin);
	});

	TMXUtil::ForeachAllObjectsInObjectGroup(this, "crossbows", [this](cocos2d::ValueMap& value)
	{
		auto crossbow = Cocos2dCreator::CreateNode<Crossbow>(value, _contentSize);
		addChild(crossbow);
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
