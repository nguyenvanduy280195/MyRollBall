#include "ScopeLog.h"
#include "ScreenLog.h"

ScopeLog::ScopeLog(const std::string& fn)
{
	_functionName = fn;
	ScreenLog::GetInstance()->Trace("Entered %s", _functionName.c_str());

}

ScopeLog::~ScopeLog()
{
	ScreenLog::GetInstance()->Trace("Exiting %s", _functionName.c_str());
}