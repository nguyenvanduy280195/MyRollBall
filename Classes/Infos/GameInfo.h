#pragma once
#include "base/CCRef.h"
#include "PlayerInfo.h"
#include "ScreenLogInfo.h"

class GameInfo : public cocos2d::Ref
{
public:
	GameInfo();
	
	const PlayerInfo& GetPlayerInfo() const { return _playerInfo; }
	const ScreenLogInfo& GetScreenLogInfo() const { return _screenLogInfo; }

private:
	const std::string FILENAME = "gameinfo.json";

	PlayerInfo _playerInfo;
	ScreenLogInfo _screenLogInfo;
};