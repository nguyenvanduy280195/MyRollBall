#include "ScreenLog.h"
#include "ScreenLogMessage.h"
#include "ScopeLock.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "Scenes/InGameScene.h"

#define SCREENLOG_PRINT_BUFFER_SIZE 8192 // The maximum total length of one log message.

ScreenLog *gScreenLog = nullptr;
char g_screenLogPrintBuffer[SCREENLOG_PRINT_BUFFER_SIZE];

ScreenLog *ScreenLog::GetInstance()
{
	static ScreenLog instance;
	return &instance;
}

ScreenLog::ScreenLog()
{
	_timeout = 5; //seconds
	_level = 0;

	cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 10000, false);
}

ScreenLog::~ScreenLog()
{
	cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	if (getParent())
	{
		getParent()->removeChild(this, false);
	}
	{
		ScopeLock lock(&_contentMutex);
		for (unsigned int i = 0; i < _messages.size(); i++)
			delete _messages[i];
	}
}

void ScreenLog::SetFontFile(std::string file)
{
	_fontFile = file;
}

void ScreenLog::SetLevelMask(int p_level)
{
	_level = p_level;
}

void ScreenLog::SetTimeoutSeconds(float t)
{
	_timeout = t;
}

void ScreenLog::AttachToScene(cocos2d::Node *scene)
{
	if (dynamic_cast<InGameScene *>(scene))
	{
		CCLOGWARN("This can be wrong when the camera moves");
	}

	if (getParent())
	{
		getParent()->removeChild(this, false);
	}

	if (scene)
	{
		scene->addChild(this, SCREENLOG_LAYER_LEVEL);
	}
}

ScreenLogMessage *ScreenLog::Log(int level, const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(level, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Log(int level, const std::string &msg, va_list args)
{
#if COCOS2D_DEBUG
	//ScopeLock lock(&_contentMutex);

	if (!msg.c_str())
	{
		return nullptr;
	}

	vsnprintf(g_screenLogPrintBuffer, SCREENLOG_PRINT_BUFFER_SIZE - 1, msg.c_str(), args);

	ScreenLogMessage *slm = new ScreenLogMessage(this);
	slm->_level = level;
	slm->_text = g_screenLogPrintBuffer;
	slm->_timestamp = _timer;
	_messages.push_back(slm);

	return slm;
#else
	return nullptr;
#endif
}

ScreenLogMessage *ScreenLog::Info(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_INFO, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Debug(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_DEBUG, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Warning(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_WARNING, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Error(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_ERROR, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Trace(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_TRACE, msg, args);
	va_end(args);
	return slm;
}

ScreenLogMessage *ScreenLog::Fatal(const std::string &msg, ...)
{
	va_list args;
	auto temp = msg.c_str();
	va_start(args, temp);
	auto slm = Log(LL_FATAL, msg, args);
	va_end(args);
	return slm;
}

void ScreenLog::SetMessageText(ScreenLogMessage *slm, const char *p_str, ...)
{
	ScopeLock lock(&_contentMutex);

	//loop through to find matching message, in case it has already gone
	bool messageStillExists = false;
	for (int i = 0; i < _messages.size(); i++)
	{
		if (_messages[i] == slm)
		{
			messageStillExists = true;
			break;
		}
	}
	if (messageStillExists)
	{
		va_list t_va;
		va_start(t_va, p_str);
		vsnprintf(g_screenLogPrintBuffer, SCREENLOG_PRINT_BUFFER_SIZE - 1, p_str, t_va);
		va_end(t_va);

		slm->SetLabelText(g_screenLogPrintBuffer);
		slm->_timestamp = _timer;
	}
}

void ScreenLog::update(float dt)
{
	ScopeLock lock(&_contentMutex);

	for (int i = 0; i < _messages.size(); i++)
	{
		ScreenLogMessage *slm = _messages[i];
		if (slm->CheckLabel())
			MoveLabelsUp(i);
	}

	int c = 0;
	for (int i = _messages.size() - 1; i >= 0; i--)
	{
		ScreenLogMessage *slm = _messages[i];
		const auto delta = _timer - slm->_timestamp;
		if (delta > _timeout || c > (2 * SCREENLOG_NUM_LINES))
		{
			removeChild(slm->_label, true);
			delete slm;
			_messages.erase(_messages.begin() + i);
		}
		c++;
	}

	_timer += dt;
}

void ScreenLog::MoveLabelsUp(int maxIndex)
{
	ScopeLock lock(&_contentMutex);

	float screenHeightPixels = cocos2d::Director::getInstance()->getWinSize().height;
	float fontSize = screenHeightPixels / (float)SCREENLOG_NUM_LINES - 1;

	if (maxIndex >= _messages.size())
	{
		maxIndex = _messages.size();
	}

	for (int i = 0; i < maxIndex; i++)
	{
		ScreenLogMessage *slm = _messages[i];
		cocos2d::Point p = slm->_label->getPosition();
		p.y += fontSize;
		slm->_label->setPosition(p);
	}
}

void ScreenLog::ClearEntries()
{
	ScopeLock lock(&_contentMutex);

	for (unsigned int i = 0; i < _messages.size(); i++)
	{
		delete _messages[i];
	}
	_messages.clear();
}