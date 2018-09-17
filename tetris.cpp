/*
 *	KILLER TETRIS
 */
#include<windows.h>

HINSTANCE hinst;
bool done;
HWND hwnd;

int Spiel_Zustand;
#define SPIEL_AUSWAHL	1
#define	SPIEL_LAEUFT	2
#define	SPIEL_BEENDEN	3

WNDCLASSEX	wndClass;
MSG			msg;

HDC			hDC;

RECT		rectTetris;

const char szclassname[] = "Klassenname";
char buffer[48];

// der spannende Teil
int fMainFunction();


LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		done = TRUE;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}


// Startfunktion
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{

	hinst		= hInstance;
	done		= FALSE;
	bool escape = FALSE;




	wndClass.cbSize			= sizeof(WNDCLASSEX);
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wndClass.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= szclassname;
	wndClass.hIconSm		= LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&wndClass)) return FALSE;

	
	if(!(hwnd = CreateWindowEx(NULL,szclassname,"KILLER TETRIS v1.0",WS_POPUP,
							 100,100,800,600,NULL,NULL,hInstance,NULL)))
	{
		MessageBox(hwnd,"Fehler: CreateWindowEx()","XXX",MB_OK);
		return 0;
	}
	

	
	// WindowsCursor ausschalten
	ShowCursor(FALSE);

	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	// Hauptschleife
	while(!done)
	{
		// Ist eine Nachricht zu verarbeiten?
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		fMainFunction();	
	}

return msg.wParam;
}
	
int fMainFunction()	// dummy
{
	GetClientRect(hwnd,&rectTetris);

	return 0;
}
