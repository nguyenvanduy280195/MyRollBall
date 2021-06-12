#pragma once

#include "2d/CCLayer.h"
#include <string>
#include <vector>
#include "cocos2d.h"
#include <mutex>


#define LL_FATAL    0x01
#define LL_ERROR    0x02
#define LL_WARNING  0x04
#define LL_INFO     0x08
#define LL_DEBUG    0x10
#define LL_TRACE    0x20


class ScreenLog : public cocos2d::Layer
{
	friend class ScreenLogMessage;
public:
	ScreenLog();
	~ScreenLog();

	void update(float dt);

	void SetFontFile(std::string file);
	void SetLevelMask(int level);
	void SetTimeoutSeconds(float seconds);
	void AttachToScene(cocos2d::Scene* scene);

	class ScreenLogMessage* Log(int level, const char* msg, ...);
	void SetMessageText(class ScreenLogMessage* slm, const char* p_str, ...);

	void MoveLabelsUp(int maxIndex);
	void ClearEntries();

private:
	std::string _fontFile;
	int _level;
	float _timeout;
	float _timer = 0;
	std::vector<class ScreenLogMessage*> _messages;
	std::mutex _contentMutex;

	// The total number of lines shown (font height will be the screen height divided by this)
	const int SCREENLOG_NUM_LINES = 32;

	// The child level of this layer in the scene. Make it higher than your other layers, if you want to see the log :)
	const int SCREENLOG_LAYER_LEVEL = 1000;

	// Zero will make the first log line appear at the bottom of the screen. A value of about 0.2 will place it above the fps counter.
	const float SCREENLOG_START_HEIGHT_PERCENT = 0.2f;
};

extern ScreenLog* gScreenLog;
