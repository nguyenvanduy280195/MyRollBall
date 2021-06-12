#include "ScopeLog.h"
#include "ScreenLog.h"

ScopeLog::ScopeLog(const std::string& fn)
{
	m_functionName = fn;
	gScreenLog->Log(LL_TRACE, "Entered %s", m_functionName.c_str());
}

ScopeLog::~ScopeLog()
{
	gScreenLog->Log(LL_TRACE, "Exiting %s", m_functionName.c_str());
}