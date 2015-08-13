#include "WinApplication.h"

namespace quokka
{
	Application* GApp() { return WinApplication::GetInstance();	}
	WinApplication* WinApplication::Instance = NULL;
}