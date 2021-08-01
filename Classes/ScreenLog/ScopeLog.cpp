#include "ScopeLog.h"
#include "ScreenLog.h"

namespace another_author
{
ScopeLog::ScopeLog(const std::string& fn)
{
	_functionName = fn;
	another_author::ScreenLog::GetInstance()->Trace("Entered %s", _functionName.c_str());

}

ScopeLog::~ScopeLog()
{
	another_author::ScreenLog::GetInstance()->Trace("Exiting %s", _functionName.c_str());
}
}

namespace duy
{
ScopeLog::ScopeLog(const std::string& fn)
{
	_functionName = fn;
	CCLOG("Entered %s", _functionName.c_str());
}

ScopeLog::~ScopeLog()
{
	CCLOG("Exitted %s", _functionName.c_str());
}
}