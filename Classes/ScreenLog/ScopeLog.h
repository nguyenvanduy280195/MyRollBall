#pragma once

#include <string>
#include "base/CCRef.h"

class ScopeLog
{
public:
	std::string _functionName;
	ScopeLog(const std::string& fn);
	~ScopeLog();
};