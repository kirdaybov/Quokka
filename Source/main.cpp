#include "Configuration.h"


#include "Quokka.h"
#include "Application/WinApplication.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	quokka::QApplication* app = new quokka::QWinApplication(hInstance);
  app->Init();
  app->Run();
  app->Shutdown();

	getchar();
  return 0;
}