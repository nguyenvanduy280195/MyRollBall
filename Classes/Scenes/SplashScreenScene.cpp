#include "SplashScreenScene.h"
#include "MyCustomGUI.inl"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"



bool SplashScreenScene::init(const std::function<void()>& onDone)
{
	if (!Super::init("ui/scene-splash-screen.tmx"))
	{
		return false;
	}

	if (auto cocos2dxLogo = GetMyWidgetByName<MyImageView>("cocos2d-x"))
	{
		cocos2dxLogo->setOpacity(0.f);

		auto firstCocos2dxLogoAction = cocos2dxLogo->GetAction();
		auto secondCocos2dxLogoAction = cocos2d::CallFunc::create(onDone);

		if (auto gameLogo = GetMyWidgetByName<MyImageView>("game"))
		{
			gameLogo->setOpacity(0.f);

			secondCocos2dxLogoAction = cocos2d::CallFunc::create([gameLogo, onDone]()
			{
				auto firstAction = gameLogo->GetAction();
				auto secondAction = cocos2d::CallFunc::create(onDone);
				gameLogo->runAction(cocos2d::Sequence::createWithTwoActions(firstAction, secondAction));
			});
		}

		cocos2dxLogo->runAction(cocos2d::Sequence::createWithTwoActions(firstCocos2dxLogoAction, secondCocos2dxLogoAction));
	}
	
	return true;
}