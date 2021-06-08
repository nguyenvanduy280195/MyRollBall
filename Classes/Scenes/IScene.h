#pragma once

class IScene
{
public:
	virtual class ScreenLog* GetScreenLog() const = 0;
	virtual class HandlerManager* GetHandlerManager() const = 0;
	virtual class GameInfo* GetGameInfo() const = 0;
};