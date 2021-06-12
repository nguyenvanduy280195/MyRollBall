#include "ScopeLock.h"

ScopeLock::ScopeLock(std::mutex* m)
{
	m_mutex = m;
	std::lock_guard<std::mutex> guard(*m_mutex);
}