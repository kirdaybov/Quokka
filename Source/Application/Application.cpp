#include "WinApplication.h"

namespace quokka
{
	QApplication* GApp() { return QWinApplication::GetInstance();	}
	QWinApplication* QWinApplication::Instance = NULL;
}