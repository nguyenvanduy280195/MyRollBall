#include "OptionsDialog.h"
#include "../MyCustomGUI.inl"

bool OptionsDialog::init()
{
	if (!MyDialog::init("<empty>"))
	{
		return false;
	}



	return true;
}
