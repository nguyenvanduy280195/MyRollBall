#pragma once

#include "base/CCValue.h"
#include "base/CCVector.h"

namespace cocos2d
{
class Sprite;
class SpriteFrame;
class PhysicsBody;
class Animate;
}

class CoinCreator
{
public:
	CoinCreator(cocos2d::ValueMap& value);
	cocos2d::Sprite* CreateSprite();
	cocos2d::Animate* CreateAnimateAction() const;

private:
	float _x;
	float _y;
	std::string _jsonPath;
	struct Info
	{
		std::string imagePath;
		std::string plistPath;
		float animationDelay;
		int nFrames;
		float anchorPointX;
		float anchorPointY;
		float bodyWidth;
		float bodyHeight;
		float bodyOffsetX;
		float bodyOffsetY;
	};

	Info _info;

	Info LoadInfoFromJSON(); // please try to use std::move
	cocos2d::PhysicsBody* CreateBody() const;
	cocos2d::Vector<cocos2d::SpriteFrame*> CreateSpriteFrames() const; // please try to use std::move
};