#pragma once

#include <string>

class ScopeLog
{
public:
	std::string m_functionName;
	ScopeLog(const std::string& fn);
	~ScopeLog();
};