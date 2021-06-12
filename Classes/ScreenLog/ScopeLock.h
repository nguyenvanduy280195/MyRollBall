#pragma once
#include <mutex>

class ScopeLock
{
public:
	std::mutex* m_mutex;
	ScopeLock(std::mutex* m);
};
