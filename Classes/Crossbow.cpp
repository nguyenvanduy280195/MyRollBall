#include "Crossbow.h"
#include "Bitmask.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "2d/CCSpriteFrameCache.h"
#include "Utils/StaticMethods.h"

using Super = cocos2d::Sprite;
using Vec2 = cocos2d::Vec2;

Crossbow::~Crossbow()
{
	_crossbowAction->release();
	_arrowAction->release();
}

bool Crossbow::init(cocos2d::ValueMap& value, const cocos2d::Size& levelSize)
{
	if (value.empty())
	{
		return false;
	}
	auto x = value["x"].asFloat();
	auto y = value["y"].asFloat();
	auto src = value["src-crossbow"].asString();
	auto srcArrow = value["src-arrow"].asString();
	auto movementDuration = value["duration-movement"].asFloat();
	auto direction = value["direction"].asString();

	auto document = StaticMethods::GetJSONFromFile(src);
	auto srcImage = document["path.image"].GetString();
	auto srcFormat = document["path.plist"].GetString();
	auto animationDelay = document["animation.delay"].GetFloat();
	auto nFrames = document["frames.number"].GetInt();
	auto anchorPointX = document["anchor.point.x"].GetFloat();
	auto anchorPointY = document["anchor.point.y"].GetFloat();

	StaticMethods::OpenSpritesheet open(srcFormat, srcImage);

	if (!Super::initWithSpriteFrameName("crossbow-0.png"))
	{
		return false;
	}

	_arrow = MakeArrow(srcArrow);
	addChild(_arrow);

	setAnchorPoint(Vec2(anchorPointX, anchorPointY));
	setPosition(x, y);
	setLocalZOrder(500);

	
	if (direction.compare("right") == 0)
	{
		setRotation(90);
	}
	else if (direction.compare("bottom") == 0)
	{
		setRotation(180);
	}
	else if (direction.compare("left") == 0)
	{
		setRotation(-90);
	}

	_crossbowAction = MakeCrossbowAction(nFrames, animationDelay);
	_crossbowAction->retain();

	const auto destination = Vec2(_arrow->getPositionX(), MAX(levelSize.width, levelSize.height));
	_arrowAction = MakeArrowAction(destination, movementDuration);
	_arrowAction->retain();

	runAction(_crossbowAction);

	//_arrow->getPhysicsBody()->applyImpulse(arrowDestination);
	scheduleUpdate();
	return true;
}

void Crossbow::update(float)
{
	CCLOG("%f, %f", _arrow->getPositionX(), _arrow->getPositionY());
}

cocos2d::Sprite* Crossbow::MakeArrow(const std::string imagePath) const
{
	auto arrow = cocos2d::Sprite::create(imagePath);
	arrow->setPositionX(0.5f * _contentSize.width);
	arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	auto arrowBody = MakeArrowPhysicsBody(arrow->getContentSize());
	arrow->setPhysicsBody(arrowBody);

	return arrow;
}

cocos2d::PhysicsBody* Crossbow::MakeArrowPhysicsBody(const cocos2d::Size& size) const
{
	auto body = cocos2d::PhysicsBody::createBox(size);

	body->setCategoryBitmask(ARROW_CATEGORY_BITMASK);
	body->setCollisionBitmask(ARROW_COLLISION_BITMASK);
	body->setContactTestBitmask(ARROW_CONTACT_TEST_BITMASK);

	return body;
}

cocos2d::ActionInterval* Crossbow::MakeCrossbowAction(int nFrames, float animationDelay)
{
	cocos2d::Vector<cocos2d::SpriteFrame*> frames;
	frames.reserve(nFrames);
	for (int i = 1; i < nFrames; i++)
	{
		auto name = cocos2d::StringUtils::format("crossbow-%d.png", i);
		auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		frames.pushBack(spriteFrame);
	}
	auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("crossbow-0.png");
	frames.pushBack(spriteFrame);

	auto animation = cocos2d::Animation::createWithSpriteFrames(frames, animationDelay);
	auto animate = cocos2d::Animate::create(animation);

	auto onCrossbowAnimatingDone = cocos2d::CallFunc::create([this]()
	{
		_arrow->runAction(_arrowAction->clone());
	});

	return cocos2d::Sequence::createWithTwoActions(animate, onCrossbowAnimatingDone);
}

cocos2d::ActionInterval* Crossbow::MakeArrowAction(const cocos2d::Vec2& destination, float movementDuration)
{
	auto moveTo = cocos2d::MoveTo::create(movementDuration, destination);
	auto onArrowMovingDone = cocos2d::CallFunc::create([this]()
	{
		_arrow->setPosition(0.5f * _contentSize.width, 0);
		runAction(_crossbowAction->clone());
	});

	return cocos2d::Sequence::createWithTwoActions(moveTo, onArrowMovingDone);
}