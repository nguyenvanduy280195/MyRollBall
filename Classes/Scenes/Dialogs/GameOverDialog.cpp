#include "GameOverDialog.h"

#include "../MyCustomGUI.inl"

#include "2d/CCCamera.h"


bool GameOverDialog::init()
{
	if (!MyDialog::init("ui/dialog-game-over.tmx"))
	{
		return false;
	}

	auto cameraPosition = cocos2d::Camera::getDefaultCamera()->getPosition();
	setPosition(cameraPosition);

	if (auto gameOverImageView = GetMyWidgetByName<MyImageView>("game-over"))
	{
		if (auto action = gameOverImageView->GetAction())
		{
			gameOverImageView->runAction(action);
		}
	}

	return true;
}

void GameOverDialog::SetOnMainMenuButtonPressed(const std::function<void(cocos2d::Ref*)>& callback)
{
	AddCallbackToButton("main-menu", callback);
}

void GameOverDialog::SetOnReplayButtonPressed(const std::function<void(cocos2d::Ref*)>& callback)
{
	AddCallbackToButton("replay", callback);
}
