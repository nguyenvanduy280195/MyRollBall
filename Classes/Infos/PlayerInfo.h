#pragma once

#include <string>

struct PlayerInfo
{
	std::string imagePath;
	float anchorPointX;
	float anchorPointY;
	float moveSpeed;
	float maxMoveSpeed;
	float dashPower;
	float destiny;
	float restitution;
	float friction;
};