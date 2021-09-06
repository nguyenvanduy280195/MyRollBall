#pragma once

class IScene
{
public:
	virtual class HandlerManager* GetHandlerManager() const = 0;
	virtual class GameInfo* GetGameInfo() const = 0;
};