/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "Scenes/InGameScene.h"
#include "Utils/Cocos2dCreator.h"
#include "Scenes/IntroLevelScene.h"
#include "Scenes/MainMenuScene.h"
#include "ScreenLog/ScreenLog.h"
#include "Scenes/SplashScreenScene.h"


#define USE_AUDIO_ENGINE (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
	duy::ScreenLog::GetInstance()->SetFontFamily("fonts/MonospaceBold.ttf");
	duy::ScreenLog::GetInstance()->SetFontSize(20.f);
	duy::ScreenLog::GetInstance()->SetTimeout(10.f);
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("MyRollBall", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
	#else
		glview = GLViewImpl::create("MyRollBall");
	#endif
		director->setOpenGLView(glview);
	}

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

	director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));

	register_all_packages();

	// create a scene. it's an autorelease object
	//auto scene = InGameScene::CreateScene(17);
	//auto scene = Cocos2dCreator::CreateNode<IntroLevelScene>(17);

	auto splashScreenScene = Cocos2dCreator::CreateNode<SplashScreenScene>([director]()
	{
		auto mainMenuScene = Cocos2dCreator::CreateNode<MainMenuScene>();
		director->replaceScene(mainMenuScene);
	});
	// run
	director->runWithScene(splashScreenScene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#endif
}
