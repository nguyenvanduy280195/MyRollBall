#pragma once

#include <string>
#include "base/CCRef.h"


namespace another_author
{
class ScopeLog
{
public:
	std::string _functionName;
	ScopeLog(const std::string& fn);
	~ScopeLog();
};

}

namespace duy
{
class ScopeLog
{
public:
	std::string _functionName;
	ScopeLog(const std::string& fn);
	~ScopeLog();
};
}