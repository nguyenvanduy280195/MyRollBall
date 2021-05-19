#include "Level.h"

using Super = cocos2d::TMXTiledMap;

bool Level::init(const std::string& filename)
{
	if (!Super::initWithTMXFile(filename))
	{
		return false;
	}

	nameTagMap["wall-right"] = 0;
	nameTagMap["wall-left"] = 1;
	nameTagMap["wall-top"] = 2;
	nameTagMap["wall-bottom"] = 3;
	nameTagMap["player"] = 4;

	auto body = MakeBody();
	setPhysicsBody(body);

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
			auto size = cocos2d::Size(width, height);
			auto offset = cocos2d::Vec2(xOffset, yOffset);
			auto shape = cocos2d::PhysicsShapeEdgeBox::create(size, cocos2d::PHYSICSSHAPE_MATERIAL_DEFAULT, 0.0f, offset);
			shape->setTag(nameTagMap[name]);
			body->addShape(shape);
		}
	}

	body->setCategoryBitmask(0xFFFFFFFF);
	body->setCollisionBitmask(0xFFFFFFFF);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);

	return body;
}

// TODO please check here
cocos2d::Vec2 Level::GetStartPosition() const
{
	auto pointsObjectGroup = getObjectGroup("positions");
	auto startObject = pointsObjectGroup->getObject("start");
	auto x = startObject["x"].asFloat();
	auto y = startObject["y"].asFloat();
	return cocos2d::Vec2(x, y);
}

// TODO please check here
cocos2d::Vec2 Level::GetEndPosition() const
{
	auto pointsObjectGroup = getObjectGroup("positions");
	auto endObject = pointsObjectGroup->getObject("end");
	auto x = endObject["x"].asFloat();
	auto y = endObject["y"].asFloat();
	return cocos2d::Vec2(x, y);
}