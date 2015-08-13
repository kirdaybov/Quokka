#pragma once
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

#include "Application.h"

#include "Game/Game.h"
#include "Common/Timer.h"
#include "Input/Input.h"
#include "Profiler/Profiler.h"
#include "Events/EventManager.h"
#include "Graphic/Graphic.h"

#include "Quokka.h"

namespace quokka
{  
	// Designed to be a singletone
	class WinApplication : public Application
	{
    friend static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	public:
		WinApplication(HINSTANCE a_hInstance) : Application()
			,Game()
			,hInstance(a_hInstance)
			,bQuit(false)
		{
			Instance = this;
		}
		static WinApplication* GetInstance()	{	return Instance; }				
		
		virtual void Init()
		{
			CreateConsole();
			InitWindowsRoutine();

			OpenGLGraphic* OGLGraphic = new OpenGLGraphic();
			OGLGraphic->InitForWindows(hWnd);
			Graphic = OGLGraphic;

			Game = new quokka::Game();
			Game->Init();			

			Input = new quokka::Input();
		}

		void InitWindowsRoutine()
		{
			WNDCLASSEX wcex;

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style          = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc    = WndProc;
			wcex.cbClsExtra     = 0;
			wcex.cbWndExtra     = 0;
			wcex.hInstance      = hInstance;
			wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName   = NULL;
			wcex.lpszClassName  = "QuokkaWindowClass";
			wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

			if(!RegisterClassEx(&wcex))
			{
				throw REGISTER_CLASS_FAILED;
			}

			static TCHAR szWindowClass[] = "QuokkaWindowClass";
			static TCHAR szTitle[] = "Quokka Project";

			HWND hWnd = CreateWindow(
				szWindowClass,
				szTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				1024, 768,
				NULL,
				NULL,
				hInstance,
				NULL
			);

			if(!hWnd)
			{
				throw WINDOW_CREATION_FAILED;
			}

			this->hWnd = hWnd;

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);

		}

		virtual void Run()
		{
			MSG msg = MSG();
			while(!bQuit)
			{				
				if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					// Timer update
					Timer()->CalculateFrameTime();
					
					// Event Manager Update
					GProfiler()->Start("Event Manager");					
					GEventManager()->Tick();
					GProfiler()->Stop("Event Manager");


					// Input update
					GProfiler()->Start("Input");
					Input->Tick();
					GProfiler()->Stop("Input");

					// Game logic update
					GProfiler()->Start("Game");
					Game->Tick();
					GProfiler()->Stop("Game");
					
					// Now draw
					GProfiler()->Start("Graphic");
					Graphic->Draw(Timer()->GetFrameTime());
					GProfiler()->Stop("Graphic");
				}
			}
		}

		virtual void Shutdown()
		{
			GProfiler()->Print();

			Game->Shutdown();

			Graphic->Shutdown();
			delete Graphic;
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
		{
			switch (Msg)
			{
			case WM_KEYDOWN:		
				WinApplication::GetInstance()->Input->Keyboard_OnKeyPressed(wParam);
				break;
			case WM_KEYUP:		
        WinApplication::GetInstance()->Input->Keyboard_OnKeyReleased(wParam);
				break;
      case WM_RBUTTONDOWN:
        WinApplication::GetInstance()->Input->Mouse_RightButtonDown();
        break;
      case WM_LBUTTONDOWN:
        WinApplication::GetInstance()->Input->Mouse_LeftButtonDown();
        break;
			case WM_CLOSE:
				GApp()->Quit();
				break;
			default:
				return DefWindowProcW(hWnd, Msg, wParam, lParam);			
			}
			return 0;
			
		}

		void Quit() { bQuit = true; }

    HWND GetHWND() { return hWnd; }

	private:
		void CreateConsole()
		{
			//TODO: refactor and handle shutdown
			const int MAX_CONSOLE_LINES = 500;

			int hConHandle;			
			long lStdHandle;
			
			CONSOLE_SCREEN_BUFFER_INFO coninfo;			
			FILE *fp;
			
			// allocate a console for this app			
			AllocConsole();
			
			// set the screen buffer to be big enough to let us scroll text			
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);			
			coninfo.dwSize.Y = MAX_CONSOLE_LINES;
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
			
			// redirect unbuffered STDOUT to the console			
			lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);			
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);			
			fp = _fdopen( hConHandle, "w" );
			
			*stdout = *fp;			
			setvbuf( stdout, NULL, _IONBF, 0 );
			
			// redirect unbuffered STDIN to the console			
			lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);			
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);			
			fp = _fdopen( hConHandle, "r" );			
			*stdin = *fp;			
			setvbuf( stdin, NULL, _IONBF, 0 );
			
			// redirect unbuffered STDERR to the console			
			lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);			
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);			
			fp = _fdopen( hConHandle, "w" );			
			*stderr = *fp;
			
			setvbuf( stderr, NULL, _IONBF, 0 );
			
			// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
			
			// point to console as well			
			std::ios::sync_with_stdio();
		}

		float Width() override
		{
			RECT Rect;
			GetWindowRect(hWnd, &Rect);
			return float(Rect.right - Rect.left);
		}

		float Height() override
		{
			RECT Rect;
			GetWindowRect(hWnd, &Rect);
			return float(Rect.bottom - Rect.top);
		}

		HWND hWnd;
		HINSTANCE hInstance;
		Game* Game;
		Graphic* Graphic;
		Input* Input;

		bool bQuit;

		static WinApplication* Instance;
	};
}