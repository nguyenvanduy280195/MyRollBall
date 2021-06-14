#include "ScopeLog.h"

#define USE_SCREEN_LOG 0

#if USE_SCREEN_LOG
#include "ScreenLog.h"
#else
#include "base/CCConsole.h"
#endif

ScopeLog::ScopeLog(const std::string& fn)
{
	m_functionName = fn;
#if USE_SCREEN_LOG
	gScreenLog->Log(LL_TRACE, "Entered %s", m_functionName.c_str());
#else
	CCLOG("Entered %s", m_functionName.c_str());
#endif
}

ScopeLog::~ScopeLog()
{
#if USE_SCREEN_LOG
	gScreenLog->Log(LL_TRACE, "Exiting %s", m_functionName.c_str());
#else
	CCLOG("Exiting %s", m_functionName.c_str());
#endif
}