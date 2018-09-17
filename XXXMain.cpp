/************************************************************************************
 *																					*
 *  IsoDD (Arbeitstitel)															*
 *																					*
 *	Dieses Projekt soll ein kleines isometrisches Spiel sein und dient nicht zur	*
 *	Veröffentlichung oder zur Veranschaulichung von gutem Programmierstil (:-))		*
 *																					*
 *  geschrieben von [TOTA]-Cheffe (aka Sinnlosi (new: aka DungeonMaster))			*
 *	mithilfe des Buches:															*
 *				"3D Spieleprogrammierung mit DirectX in C/C++" (Stefan Zerbst)		*
 *																					*
 *	verbrauchte Rohstoffe: >4700 Zigaretten, 150 Liter Flüssiges,					*
 *						   >5500 Kg Nahrung, 47 BlueScreens,(und 84 Nervenkoller)	*
 *																					*
 *			~4000 Zeilen Code (Stand 16.06.2001 -> ca. 10 Jahre vor Deadline)		*
 ************************************************************************************/
// Datei: xxxmain.cpp

/*
 *	Disclaimer: Hauptdatei: Fenster,Hauptnachrichten- und InputSchleifen,
 *							Initialisierung von DirectDraw
 */

// INCLUDES
#include<windows.h>
#include<ddraw.h>
#include<dinput.h>
#include"stdio.h"
#include"XXXGlobal.h"
#include"XXXSound.h"


// GLOBALS

// XXXInput Variablen
extern BOOL Eingabe[AK_MAXIMAL];		// eingabe aktion
extern int Maus_x;						// speichert maus koordinaten
extern int Maus_y;						// speichert maus koordinaten
extern int Joystick_x;					// speichert joystick koordinaten
extern int Joystick_y;					// speichert joystick koordinaten
extern char strJoy_Name[50];			// joystick name
extern int Spieler_Energie;

BOOL done;
int  Spiel_Zustand;
HWND hwnd;
RECT rcRect;
double framerate;

// DirectDraw Objekte	
LPDIRECTDRAW7			lpDD;
LPDIRECTDRAWSURFACE7	lpDDSPrimary;
LPDIRECTDRAWSURFACE7	lpDDSBack;
LPDIRECTDRAWPALETTE		lpDDPal;
LPDIRECTDRAWCLIPPER		lpDDClipper;

// DirectSound
extern LPDIRECTSOUNDBUFFER lpDSB[ANZ_SOUNDS_GESAMT];

// Spielspezifische globale Variablen
LPDIRECTDRAWSURFACE7	lpDDSRahmen;		// Umrandung des Bildes
LPDIRECTDRAWSURFACE7	lpDDSStartup;		// Titelbild
LPDIRECTDRAWSURFACE7	lpDDSFeldgrafiken;	// Grafiken der Spielfelder
LPDIRECTDRAWSURFACE7	lpDDSViecher;		// Grafiken der Spielobjekte

HDC			hDC;
HINSTANCE	hinst;
int			x_Scroll=0,y_Scroll=0;
XXX_OBJEKT	Objektliste[ANZ_OBJ_GESAMT];





// FUNKTIONEN

// Callback Funktion
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int w=0;
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
	WNDCLASSEX	wndClass;
	MSG			msg;
	DWORD		loop_start_time;
	hinst		= hInstance;
	
	const char szclassname[] = "Klassenname";
	char buffer[48];
	
	rcRect.left		= 0;
	rcRect.top		= 0;
	rcRect.right	= SCREEN_BREITE;
	rcRect.bottom	= SCREEN_HOEHE;


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

	
	if(!(hwnd = CreateWindowEx(NULL,szclassname,"Titelleiste",WS_POPUP,
							 0,0,400,300,NULL,NULL,hInstance,NULL)))
	{
		MessageBox(hwnd,"Fehler: CreateWindowEx()","XXX",MB_OK);
		return 0;
	}
	
	// WindowsCursor ausschalten
	ShowCursor(FALSE);

	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	done = FALSE;
	Spiel_Zustand = SPIEL_AUSWAHL;


// PHASE 1:
	Spiel_Initialisieren();

// PHASE 2:

	// Hauptschleife
	while(!done)
	{
		// Ist eine Nachricht zu verarbeiten?
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// SPIELSTART
		if(Spiel_Zustand == SPIEL_AUSWAHL)
		{
			// Tastatur abfragen
			xxxDInput_Update();
			if(Eingabe[AK_STARTEN])
				Spiel_Zustand = SPIEL_LAEUFT;
			else if(Eingabe[AK_BEENDEN])
				Spiel_Zustand = SPIEL_BEENDEN;
		}
		// SPIEL LAEUFT
		else if(Spiel_Zustand == SPIEL_LAEUFT)
		{
			// START DER EIGENTLICHEN HAUPTSCHLEIFE
			loop_start_time = GetTickCount();

			// Eingabegeräte auf neuen Stand bringen
			xxxDInput_Update();
			xxxDDraw_Puffer_leeren(lpDDSBack,255);
			
			// Scrolling durchführen
			if(Eingabe[AK_SCROLL_L])
				x_Scroll -= SCROLL_RATE;
			if(Eingabe[AK_SCROLL_R])
				x_Scroll += SCROLL_RATE;
			if(Eingabe[AK_SCROLL_U])
				y_Scroll -= SCROLL_RATE;
			if(Eingabe[AK_SCROLL_D])
				y_Scroll += SCROLL_RATE;

			if(x_Scroll < 48) x_Scroll = 48;
			if(x_Scroll > (FELD_BREITE*FELDER_SPALTE-SCREEN_BREITE))
				x_Scroll = FELD_BREITE*FELDER_SPALTE-SCREEN_BREITE;
			if(y_Scroll < 37) y_Scroll = 37;
			if(y_Scroll > (FELD_HOEHE/2*FELDER_ZEILE-SCREEN_HOEHE))
				y_Scroll = FELD_HOEHE/2*FELDER_ZEILE-SCREEN_HOEHE;

			// Das Spielfeld malen
			xxxSFeld_malen(lpDDSBack);

			// Alle Objekte bewegen un malen
			xxxObjekte_bewegen();
			xxxObjekt_Kollisionstest();
			xxxObjektliste_sortieren();
			xxxObjektliste_malen(lpDDSBack);

			// MausZeiger
			xxxMale_Rechteck(Maus_x-5,Maus_y-5,Maus_x+5,Maus_y+5,255,lpDDSBack,FALSE);

			// Rahmen malen
			lpDDSBack->Blt(&rcRect,lpDDSRahmen,&rcRect,DDBLT_WAIT | DDBLT_KEYSRC,NULL);

			// Framerate und Energie anzeigen /////

			framerate = (double)(GetTickCount() - loop_start_time)/1000;
			framerate = 1/framerate;
			sprintf(buffer,"FRAMERATE: %f",framerate);
			xxxMale_Text(47,591,120,lpDDSBack,buffer);
			sprintf(buffer,"ENERGIE: %d",Spieler_Energie);
			xxxMale_Text(196,591,120,lpDDSBack,buffer);
			
			xxxMale_Text(660,591,225,lpDDSBack,"Cheffe 2001");

			
			xxxDDraw_Puffer_wechseln();

			// Soll Spiel beendet werden ?
			if(Eingabe[AK_BEENDEN])
				Spiel_Zustand = SPIEL_BEENDEN;
			/*else if(Spieler_Energie <= 1)	// funzt irgendwie nicht (!syntax)
				Spiel_Zustand = SPIEL_BEENDEN;*/

			// FRAMEBREMSE
			while((GetTickCount() - loop_start_time) < 40);
		}
		// SPIEL BEENDEN
		else if(Spiel_Zustand == SPIEL_BEENDEN)
		{
			// *BILD ANZEIGEN*
			if(Eingabe[AK_BEENDEN])
				done = TRUE;
		}
	}

// PHASE 3:
Spiel_Beenden();

return msg.wParam;
}


// DEKLARATIONEN

// Phase 1:
int Spiel_Initialisieren()
{
	BOOL ret;
	ret = xxxDDraw_initialisieren();
	if(ret == FALSE)
	{
		MessageBox(hwnd,"DDraw Fehler","XXX",MB_OK);
		
		done = TRUE;
		return FALSE;
	}
	
	ret = xxxDInput_initialisieren(hinst,FALSE);
	if(ret == FALSE)
	{
		MessageBox(hwnd,"DInput Fehler","XXX",MB_OK);
		done = TRUE;
		return FALSE;
	}

	ret =xxxDSound_initialisieren(hwnd);
	if(ret == FALSE)
	{
		MessageBox(hwnd,"xxxDSound_initialisieren() Fehler","XXX",MB_OK);
		done = TRUE;
		return FALSE;
	}
	
	ret = xxxSFeld_generieren(lpDDSFeldgrafiken);
	if(ret == FALSE)
	{
		MessageBox(hwnd,"xxxSFeld_generieren() Fehler","XXX",MB_OK);
		done = TRUE;
		return FALSE;
	}

	ret = xxxObjektliste_erstellen(lpDDSViecher);
	if(ret == FALSE)
	{
		MessageBox(hwnd,"xxxObjektliste_erstellen() Fehler","XXX",MB_OK);
		done = TRUE;
		return FALSE;
	}

	xxxObjektliste_sortieren();

	lpDDSPrimary->Blt(&rcRect,lpDDSStartup,&rcRect,DDBLT_WAIT,NULL);
	
	return TRUE;
}

// Phase 3:
int Spiel_Beenden(void)
{
	xxxDDraw_beenden();
	xxxDInput_beenden();
	xxxDSound_beenden();
	return TRUE;
}

BOOL xxxDDraw_initialisieren(void)
{
	HRESULT			ddrval;		// Rueckgabewert
	BOOL			rval = TRUE;// Rueckgabewert
	DDSURFACEDESC2	ddsd;		// Surface Description
	DDSCAPS2		ddscaps;	// Captions
	RECT			Clip_Rect;
	DDCOLORKEY		ddck;
	
	//	HDC				hDC;		// Handle aufn Device Context
	
	ddrval = DirectDrawCreateEx(NULL,(LPVOID*)&lpDD,IID_IDirectDraw7,NULL);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: CreateEx()","XXX",MB_OK);
		return 0;
	}

	ddrval = lpDD->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: CooperativeLevel()","XXX",MB_OK);
		return 0;
	}

	ddrval = lpDD->SetDisplayMode(SCREEN_BREITE,SCREEN_HOEHE,SCREEN_BPP,0,0);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: SetDisplayMode()","XXX",MB_OK);
		return 0;
	}

	// Primary Surface.
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |DDSCAPS_FLIP |DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	ddrval = lpDD->CreateSurface(&ddsd,&lpDDSPrimary,NULL);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: CreateSurface(): PrimarySurface","XXX",MB_OK);
		return 0;
	}
	
	// Back Buffer == COMPLEX SURFACE
	ZeroMemory(&ddscaps,sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: GetAttachedSurface()","XXX",MB_OK);
		return FALSE;
	}

	rval = xxxDDraw_Clipper_erzeugen(&Clip_Rect,1);
	if(rval!=TRUE)
		return FALSE;
	
	// Beide Oberflaechen mit weiß loeschen
	xxxDDraw_Puffer_leeren(lpDDSPrimary,255);
	xxxDDraw_Puffer_leeren(lpDDSBack,255);

	// Erstellen der Offscreen Surface Description
	ZeroMemory(&ddscaps,sizeof(ddscaps));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwHeight = SCREEN_HOEHE;
	ddsd.dwWidth  = SCREEN_BREITE;

	// Erstellen  einer OffscreenSurface für Bitmaps im System RAM
	ddrval = lpDD->CreateSurface(&ddsd,&lpDDSFeldgrafiken,NULL);
	if(ddrval!=DD_OK)
	{
		MessageBox(hwnd,"Create Offscreen-Surface failed: lpDDSFeldgrafiken","XXX",MB_OK);
		return FALSE;
	}

	if(!xxxBMP_in_Surface("terrain.bmp",lpDDSFeldgrafiken,TRUE))
		MessageBox(hwnd,"Fehler: BMP laden: terrain.bmp","XXX",MB_OK);

	// Ebenso die Grafiken der Spielobjekte
	ddrval = lpDD->CreateSurface(&ddsd,&lpDDSViecher,NULL);
	if(ddrval!=DD_OK)
	{
		MessageBox(hwnd,"Create Offscreen-Surface failed: lpDDSViecher","XXX",MB_OK);
		return FALSE;
	}

	if(!xxxBMP_in_Surface("viecher.bmp",lpDDSViecher,TRUE))
		MessageBox(hwnd,"Fehler: BMP laden: viecher.bmp","XXX",MB_OK);

	// Ebenso die Startgrafik
	ddrval = lpDD->CreateSurface(&ddsd,&lpDDSStartup,NULL);
	if(ddrval!=DD_OK)
	{
		MessageBox(hwnd,"Create Offscreen-Surface failed: lpDDSStartup","XXX",MB_OK);
		return FALSE;
	}

	if(!xxxBMP_in_Surface("startup.bmp",lpDDSStartup,TRUE))
		MessageBox(hwnd,"Fehler: BMP laden: startup.bmp","XXX",MB_OK);
	
	// Und der Rahmen
	ddrval = lpDD->CreateSurface(&ddsd,&lpDDSRahmen,NULL);
	if(ddrval!=DD_OK)
	{
		MessageBox(hwnd,"Create Offscreen-Surface failed: lpDDSRahmen","XXX",MB_OK);
		return FALSE;
	}
	// DirectDraw ColorKey für AdvSprite setzen
	ddck.dwColorSpaceLowValue = 0;
	ddck.dwColorSpaceHighValue= 0;
	lpDDSRahmen->SetColorKey(DDCKEY_SRCBLT , &ddck);

	if(!xxxBMP_in_Surface("rahmen.bmp",lpDDSRahmen,TRUE))
		MessageBox(hwnd,"Fehler: BMP laden: rahmen.bmp","XXX",MB_OK);


	/*
	 *	TEST*FUNKTIONEN*
	 */ 
	if(GDIvsDIRECTTEXT == 1)
	{
		DWORD dwS1,dwE1,dwZeit1;
		DWORD dwS2,dwE2,dwZeit2;
		char buffer[50];
		const char s[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		dwS1 = GetTickCount();
		for(int i=0;i<2000;i++)
		{
			lpDDSBack->GetDC(&hDC);
			TextOut(hDC,100,100,s,lstrlen(s));
			lpDDSBack->ReleaseDC(hDC);
		}
		dwE1 = GetTickCount();

		dwS2 = GetTickCount();
		for(int j=0;j<2000;j++)
		{
			xxxMale_Text(100,100,0,lpDDSBack,s);
		}
		dwE2 = GetTickCount();
	
		dwZeit1 = dwE1 - dwS1;
		dwZeit2 = dwE2 - dwS2;

		sprintf(buffer,"Zeit GDI: %f vs Zeit DirectText: %f",(float)dwZeit1/1000,
														 (float)dwZeit2/1000);
		lpDDSBack->GetDC(&hDC);
		TextOut(hDC,50,300,buffer,lstrlen(buffer));
		lpDDSBack->ReleaseDC(hDC);
	}

	if(GDIvsDIRECTRECT == 1)
	{
		DWORD	dwS1,dwE1,dwZeit1;
		DWORD	dwS2,dwE2,dwZeit2;
		char buffer[50];
		RECT	rect;
		HANDLE  bPen;

		rect.top	= 30;
		rect.left	= 30;
		rect.bottom = 131;
		rect.right  = 131;

		bPen = CreateSolidBrush(RGB(0,255,0));

		dwS1 = GetTickCount();
		for(int i=0;i<2000;i++)
		{
			if(lpDDSBack->GetDC(&hDC) == DD_OK)
			{
				Rectangle(hDC,rect.left,rect.top,rect.right,rect.bottom);
				//FillRect(hDC,&rect,(HBRUSH)bPen);
				lpDDSBack->ReleaseDC(hDC);
			}
		}
		dwE1 = GetTickCount();

		dwS2 = GetTickCount();
		for(int j=0;j<2000;j++)
		{	
			xxxMale_Rechteck(30,30,130,130,0,lpDDSBack,/* TRUE */ FALSE);
		}											   /*solid or not */
		dwE2 = GetTickCount();

		dwZeit1 = dwE1 - dwS1;
		dwZeit2 = dwE2 - dwS2;

		sprintf(buffer,"Zeit GDI: %f vs Zeit DirectRechteck: %f",(float)dwZeit1/1000,
																(float)dwZeit2/1000);

		if(lpDDSBack->GetDC(&hDC) == DD_OK)
		{
			TextOut(hDC,40,300,buffer,lstrlen(buffer));
			lpDDSBack->ReleaseDC(hDC);
		}
	}

	if(GDIvsDIRECTLINE == 1)
	{
		DWORD	dwS1,dwE1,dwZeit1;
		DWORD	dwS2,dwE2,dwZeit2;
		char	buffer[50];
		HANDLE	pen;
		HDC		hdc;

		pen = CreatePen(PS_SOLID,1,RGB(0,255,0));

		dwS1 = GetTickCount();
		for(int i=0;i<2000;i++)
		{
			if(lpDDSBack->GetDC(&hdc) == DD_OK)
			{
				MoveToEx(hdc,70,550,NULL);
				SelectObject(hdc,pen);
				LineTo(hdc,750,50);
				DeleteObject(pen);
				lpDDSBack->ReleaseDC(hdc);
			}
		}
		dwE1 = GetTickCount();

		dwS2 = GetTickCount();
		for(int j=0;j<2000;j++)
		{
			xxxMale_Linie(20,550,700,50,0,lpDDSBack);
		}
		dwE2 = GetTickCount();

		dwZeit1 = dwE1 - dwS1;
		dwZeit2 = dwE2 - dwS2;

		sprintf(buffer,"Zeit GDI: %f Zeit DirectLinie: %f",(float)dwZeit1/1000,
														   (float)dwZeit2/1000);
		
		if(lpDDSBack->GetDC(&hdc) == DD_OK)
		{
			TextOut(hdc,50,300,buffer,lstrlen(buffer));
			lpDDSBack->ReleaseDC(hdc);
		}
	}
	/*
	 *	ENDE DER TESTFUNKTIONEN
	 */

	return TRUE;
}//x

BOOL xxxDDraw_Puffer_wechseln(void)
{
	HRESULT		ddrval;
	for(int i=0;i<1000;i++)
	{
		xxxMale_Pixel(rand()%800,rand()%600,rand()%256,lpDDSBack);
	}

	ddrval = lpDDSPrimary->Flip(NULL,DDFLIP_WAIT);
	if(ddrval = DD_OK)
		return TRUE;

	else if(ddrval == DDERR_SURFACELOST)
	{
		ddrval = lpDDSPrimary->Restore();
		if(ddrval != DD_OK)
			return FALSE;

		ddrval = lpDDSPrimary->Flip(NULL,DDFLIP_WAIT);
		if(ddrval != DD_OK)
			return FALSE;

		return TRUE;
	}
	return TRUE;
}//x

BOOL xxxDDraw_Puffer_leeren(LPDIRECTDRAWSURFACE7 lpDDSurf,UCHAR Farbe)
{
	HRESULT		ddrval;
	DDBLTFX		ddBltFx;

	ZeroMemory(&ddBltFx,sizeof(ddBltFx));
	ddBltFx.dwSize = sizeof(ddBltFx);
	ddBltFx.dwFillColor = Farbe;

	ddrval = lpDDSurf->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddBltFx);
	if(ddrval != DD_OK)
	{
		MessageBox(hwnd,"DDraw Fehler: Puffer_leeren()","XXX",MB_OK);
		return FALSE;
	}
	return TRUE;
}//x

BOOL xxxDDraw_beenden(void)
{
	if(lpDD != NULL)
	{
		if(lpDDSPrimary != NULL)
		{
			lpDDSPrimary->Release();
			lpDDSPrimary = NULL;
			lpDDSBack	 = NULL;
		}
		lpDD->Release();
		lpDD = NULL;
	}
	return TRUE;
}//x

void xxxJoystick_Test(void)
{
	int dx,dy;
	// joystickdaten von 1000 runterskalieren und von Mitte des Rechtecks (s.u.) aus setzen
	dx = 250 + (int)Joystick_x/10;
	dy = 450 + (int)Joystick_y/10;

	// bereich weis löschen und schwarzen rahmen zeichnen
	xxxMale_Rechteck(150,350,350,550,255,lpDDSPrimary,TRUE);
	xxxMale_Rechteck(150,350,350,550,0,lpDDSPrimary,FALSE);

	// pixel zeigt joystickbewegung an
	xxxMale_Pixel(dx,dy,0,lpDDSPrimary);

	if(Eingabe[AK_SCHIESSEN])
	{
		xxxMale_Rechteck(200,300,250,313,0,lpDDSPrimary,TRUE);
		xxxMale_Text(202,310,255,lpDDSPrimary,"SCHUSS");
	}
	if(Eingabe[AK_RAKETE])
	{
		xxxMale_Rechteck(200,300,250,313,0,lpDDSPrimary,TRUE);
		xxxMale_Text(202,310,255,lpDDSPrimary,"RAKETE");
	}

	xxxMale_Text(150,330,0,lpDDSPrimary,strJoy_Name);
}//x

BOOL xxxDDraw_Clipper_erzeugen(LPRECT lpRect,int Anz_Rect)
{
	LPRGNDATA		lpRgnData = NULL;
	HRESULT			ddrval;

	ddrval = lpDD->CreateClipper(0,&lpDDClipper,NULL);
	if(ddrval != DD_OK) 
	{
		MessageBox(hwnd,"DDraw Fehler: CreateClipper()","XXX",MB_OK);
		return FALSE;
	}

	lpRgnData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+Anz_Rect*sizeof(RECT));
	ZeroMemory(lpRgnData,sizeof(RGNDATAHEADER)+Anz_Rect*sizeof(RECT));
	lpRgnData->rdh.dwSize		= sizeof(RGNDATAHEADER);
	lpRgnData->rdh.iType		= RDH_RECTANGLES;
	lpRgnData->rdh.nCount		= Anz_Rect;
	lpRgnData->rdh.nRgnSize		= Anz_Rect * sizeof(RECT);
	lpRgnData->rdh.rcBound.left		= 0;
	lpRgnData->rdh.rcBound.top		= 0;
	lpRgnData->rdh.rcBound.right	= SCREEN_BREITE;
	lpRgnData->rdh.rcBound.bottom	= SCREEN_HOEHE;

	memcpy(lpRgnData->Buffer,lpRect,Anz_Rect*sizeof(RECT));
	ddrval = lpDDClipper->SetClipList(lpRgnData,0);
	if(ddrval != DD_OK)
	{
		free(lpRgnData);
		return FALSE;
	}

	return TRUE;
}//x