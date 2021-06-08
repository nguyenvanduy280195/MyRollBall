#include "GameInfo.h"
#include "platform/CCFileUtils.h"
#include "Utils/StaticMethods.h"
#include "json/rapidjson.h"
#include "json/document.h"

GameInfo::GameInfo()
{
	rapidjson::Document document;
	

	auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(FILENAME);
	
	document.Parse<0>(json.c_str());
	if (document.HasParseError())
	{
		CCLOG("[GameInfo][RapidJSON] - Error Code: %d", (int)document.GetParseError());
		return;
	}
	
	
	_playerInfo.moveSpeed = document["player"]["moveSpeed"].GetFloat();
	_playerInfo.maxMoveSpeed = document["player"]["maxMoveSpeed"].GetFloat();
	_playerInfo.destiny = document["player"]["destiny"].GetFloat();
	_playerInfo.restitution = document["player"]["restitution"].GetFloat();
	_playerInfo.friction = document["player"]["friction"].GetFloat();

	_screenLogInfo.fontFamily = document["screen-log"]["font-family"].GetString();
	_screenLogInfo.fontSize = document["screen-log"]["font-size"].GetFloat();
	_screenLogInfo.textColor = StaticMethods::MakeColor3BFromHex(document["screen-log"]["text-color"].GetString());
	_screenLogInfo.removingTimerLimit = document["screen-log"]["removing-timer-limit"].GetFloat();
	_screenLogInfo.logCountLimit = document["screen-log"]["log-count-limit"].GetInt();
}