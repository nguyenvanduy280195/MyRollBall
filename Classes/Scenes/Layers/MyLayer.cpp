#include "MyLayer.h"

#include "../MyCustomGUI.inl"

bool MyLayer::init(const std::string& tmxPath)
{
	if (!Super::init(tmxPath))
	{
		return false;
	}

	return true;
}


