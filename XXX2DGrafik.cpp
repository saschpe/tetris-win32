// Datei: xxx2dgrafik.cpp

/* 
 *	Disclaimer: niedliche kleine Funktionssammlung für 2D Grafikdarstellung
 */

// INCLUDES
#include<ddraw.h>		// DirectDraw Bibliothek
#include"XXXGlobal.h"

// GLOBALS

// FUNKTIONEN

BOOL xxxMale_Pixel(int x,int y,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf)
{
	int				Zeilenbreite;
	UCHAR			*Vram;
	DDSURFACEDESC2	ddsd;

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	lpDDSurf->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	Zeilenbreite = ddsd.lPitch;
	Vram = (UCHAR*)ddsd.lpSurface;
	Vram[x+y*Zeilenbreite] = Farbe;
	lpDDSurf->Unlock(NULL);
	return TRUE;
}//x

BOOL xxxMale_Text(int x,int y,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf,const char *Text)
{
	char			Token;			// Aktuelles Zeichne im Text
	int				TextLaenge;		// Laenge des Textes
	int				Zeilenbreite;	// Zeilenbreite
	int				DDS_Breite;		// Breite der Surface
	int				DDS_Hoehe;		// Hoehe der Surface
	UCHAR			*Vram;			// Speicheradresse der Surface
	DDSURFACEDESC2	ddsd;			// SurfaceDesc Struktur

	TextLaenge = strlen(Text);
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	lpDDSurf->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	Zeilenbreite = ddsd.lPitch;
	DDS_Breite	 = ddsd.dwWidth;
	DDS_Hoehe	 = ddsd.dwHeight;
	Vram		 = (UCHAR*)ddsd.lpSurface;

	// Text zu hoch oder zu tief für Anzeige
	if(y<8 || y>(DDS_Hoehe))
		return FALSE;

	// Text durchlaufen
	for(int i=0;i<TextLaenge;i++)
	{
		Token = Text[i];
		// zu weit links kann korrigiert werden
		if(x<0)
		{
			x+=10;
			continue;
		}
		// ...zu weit rechts nicht
		else if(x>DDS_Breite)
		{
			lpDDSurf->Unlock(NULL);		// nicht vergessen!
			return TRUE;				// Funktion beenden
		}

		// Welcher Buchstabe soll gemalt werden
		switch(Token)
		{
	/* 
	 * Die folgende Implementierung der einzelnen Buchstaben unseres
	 * wunderschönen Alphabets dauerte ungefähr <?$&*'@ß> Stunden
	 * Resultate:	- Handgelenkskrebs
	 *				- Bänderzerrung
	 *				- bin immer noch nicht fertig :-)
	 */
		case ' ':
			{
				x+=4;
			}break;
		case '.':
			{
				Vram[x+2 +     y*Zeilenbreite] = Farbe;

				x+=4;
			}break;
		case ':':
			{
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;

				x+=4;
			}break;
		case '0':
			{
				//linker Strich
				//Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				//Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Striche
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;		
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				
				x+=7;
			}break;
		case '1':
			{
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;	
				
				x+=7;
			}break;
		case '2':
			{
				// Beta
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+1 +	   y*Zeilenbreite] = Farbe;
				Vram[x+2 +	   y*Zeilenbreite] = Farbe;
				Vram[x+3 +	   y*Zeilenbreite] = Farbe;
				Vram[x+4 +	   y*Zeilenbreite] = Farbe;
				Vram[x+5 +	   y*Zeilenbreite] = Farbe;
			
				x+=7;
			}break;
		case '3':
			{
				// Querstriche
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Striche
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
			
				x+=7;
			}break;
		case '4':
			{
				// Beta
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case '5':
			{
				// Oben
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
			    Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;

				Vram[x+1 + 	   y*Zeilenbreite] = Farbe;
				Vram[x+2 +	   y*Zeilenbreite] = Farbe;
				Vram[x+3 +	   y*Zeilenbreite] = Farbe;
				Vram[x+4 +	   y*Zeilenbreite] = Farbe;
				x+=7;
			}break;
		case '6':
			{
				// Beta
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case '7':
			{
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				x+=7;
			}break;
		case '8':
			{	
				// Beta
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case '9':
			{
				// Beta
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				//Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				//Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'a':
		case 'A':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechter Strich
				Vram[x+6 +     y*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				// Mittelstrich
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				x += 9; // Position de Cursors weiterschieben
			}break;
		case 'b':
		case 'B':
			{
				//linker Strich
				Vram[x  +     y*Zeilenbreite] = Farbe;
				Vram[x  + (y-1)*Zeilenbreite] = Farbe;
				Vram[x  + (y-2)*Zeilenbreite] = Farbe;
				Vram[x  + (y-3)*Zeilenbreite] = Farbe;
				Vram[x  + (y-4)*Zeilenbreite] = Farbe;
				Vram[x  + (y-5)*Zeilenbreite] = Farbe;
				Vram[x  + (y-6)*Zeilenbreite] = Farbe;
				Vram[x  + (y-7)*Zeilenbreite] = Farbe;
				Vram[x  + (y-8)*Zeilenbreite] = Farbe;
				// Querstriche
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Striche
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;

				x+=7;	// Position des Cursors weiterschieben
			}break;
		case 'c':
		case 'C':
			{
				// linker Bogen
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				// Oben und Unten
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Seite
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				x+=8;	// Position Cursor
			}break;
		case 'd':
		case 'D':
			{
				//linker Strich
				Vram[x  +     y*Zeilenbreite] = Farbe;
				Vram[x  + (y-1)*Zeilenbreite] = Farbe;
				Vram[x  + (y-2)*Zeilenbreite] = Farbe;
				Vram[x  + (y-3)*Zeilenbreite] = Farbe;
				Vram[x  + (y-4)*Zeilenbreite] = Farbe;
				Vram[x  + (y-5)*Zeilenbreite] = Farbe;
				Vram[x  + (y-6)*Zeilenbreite] = Farbe;
				Vram[x  + (y-7)*Zeilenbreite] = Farbe;
				Vram[x  + (y-8)*Zeilenbreite] = Farbe;
				// Querstriche
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Striche
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;		
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				
				x+=7;
			}break;
		case 'e':
		case 'E':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				//Vram[x+5 +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				
				x+=7;
			}break;
		case 'f':
		case 'F':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'g':
		case 'G':
			{
				// linker Bogen
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				// Oben und Unten
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Seite
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;

				x+=8;
			}break;
		case 'h':
		case 'H':
			{
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+5 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;

				x+=8;
			}break;
		case 'i':
		case 'I':
			{
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;

				x+=5;
			}break;
		case 'j':
		case 'J':
			{
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'k':
		case 'K':
			{
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 +     y*Zeilenbreite] = Farbe;
				
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;

				x+=8;
			}break;
		case 'l':
		case 'L':
			{
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 +     y*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'm':
		case 'M':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				// mittlere Pixel
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				// rechter Strich
				Vram[x+6 +     y*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-8)*Zeilenbreite] = Farbe;


				x+=9;
			}break;
		case 'n':
		case 'N':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				// mittlere Pixel
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;				
				// rechter Strich
				Vram[x+5 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;

				x+=8;
			}break;
		case 'o':
		case 'O':
			{
				// linker Strich
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechter Strich
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;

				x+=9;
			}break;
		case 'p':
		case 'P':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				// Bauch
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'q':
		case 'Q':
			{
				// linker Strich
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				// rechter Strich
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+6 +     y*Zeilenbreite] = Farbe;

				x+=9;
			}break;
		case 'r':
		case 'R':
			{
				// linker Strich
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				// Bauch
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				// Strich nach (x+5;y+0)
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-0)*Zeilenbreite] = Farbe;
				
				x+=7;
			}break;
		case 's':
		case 'S':
			{
					// linker oberer Bogen
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				// Oben
			    Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Seite
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-4)*Zeilenbreite] = Farbe;
				//Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+1 + 	   y*Zeilenbreite] = Farbe;
				Vram[x+2 +	   y*Zeilenbreite] = Farbe;
				Vram[x+3 +	   y*Zeilenbreite] = Farbe;
				Vram[x+4 +	   y*Zeilenbreite] = Farbe;
				x+=7;
			}break;
		case 't':
		case 'T':
			{
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;

				x+=6;
			}break;
		case 'u':
		case 'U':
			{
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				Vram[x   + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-6)*Zeilenbreite] = Farbe;
				Vram[x   + (y-5)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-4)*Zeilenbreite] = Farbe;
				Vram[x   + (y-3)*Zeilenbreite] = Farbe;
				Vram[x   + (y-2)*Zeilenbreite] = Farbe;
				Vram[x   + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'v':
		case 'V':
			{
				// oben links
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+5 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-8)*Zeilenbreite] = Farbe;

				x+=8;
			}break;
		case 'w':
		case 'W':
			{
				// linke Seite
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+5 +     y*Zeilenbreite] = Farbe;
				Vram[x+6 +     y*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+6 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+7 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+7 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+7 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+7 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+7 + (y-8)*Zeilenbreite] = Farbe;
				// rechte Seite
				
				x+=10;
			}break;
		case 'x':
		case 'X':
			{
				// von unten links nach oben rechts
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				// von oben links nach unten rechts
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				x+=7;
			}break;
		case 'y':
		case 'Y':
			{
				// oben
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-7)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;
				// unten 	
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;

				x+=7;
			}break;
		case 'z':
		case 'Z':
			{
				//Vram[x+5 +     y*Zeilenbreite] = Farbe;
				Vram[x+4 +     y*Zeilenbreite] = Farbe;
				Vram[x+3 +     y*Zeilenbreite] = Farbe;
				Vram[x+2 +     y*Zeilenbreite] = Farbe;
				Vram[x+1 +     y*Zeilenbreite] = Farbe;
				Vram[x   +     y*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-1)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-2)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-3)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-4)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-5)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-6)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-7)*Zeilenbreite] = Farbe;
				//Vram[x+5 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+4 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+3 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+2 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x+1 + (y-8)*Zeilenbreite] = Farbe;
				Vram[x   + (y-8)*Zeilenbreite] = Farbe;

				x+=7;
			}
		/* [...] alle weiteren Buchstaben, Zahlen uns Sonderzeichen */
		default:
			break;
		}//switch
	}//for
	lpDDSurf->Unlock(NULL);
	return TRUE;
}//x

BOOL xxxMale_Rechteck(int xLinks,int yOben,int xRechts,int yUnten,UCHAR Farbe,
					  LPDIRECTDRAWSURFACE7 lpDDSurf,int solide)
{
	int				Zeilenbreite;	// Zeilenbreite
	int				DDS_Breite;		// Breite der Surface
	int				DDS_Hoehe;		// Hoehe der Surface
	UCHAR			*Vram;			// Speicheradresse der Struktur
	DDSURFACEDESC2	ddsd;			// SurfaceDesc Struktur

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	lpDDSurf->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
		
		Zeilenbreite = ddsd.lPitch;
		DDS_Breite	 = ddsd.dwWidth;
		DDS_Hoehe	 = ddsd.dwHeight;
		Vram		 = (UCHAR*)ddsd.lpSurface;

		// Rechteck auf Grösse der Surface schneiden
		if(xLinks	< 0)			xLinks	= 0;
		if(xRechts	> DDS_Breite)	xRechts = DDS_Breite;
		if(yOben	< 0)			yOben	= 0;
		if(yUnten	> DDS_Hoehe)	yUnten  = DDS_Hoehe;

		if(solide == FALSE)
		{
			// Horizontale Linien
			for(int i=xLinks; i<=xRechts; i++)
			{
				Vram[i + yOben*Zeilenbreite] = Farbe;
				Vram[i + yUnten*Zeilenbreite]= Farbe;
			}
			// Vertikale Linien
			for(int j=yOben; j<=yUnten; j++)
			{
				Vram[xLinks  + j*Zeilenbreite] = Farbe;
				Vram[xRechts + j*Zeilenbreite] = Farbe;
			}
		}
		else
		{
			// Zeilen
			for(int k=xLinks; k<=xRechts; k++)
			{
				// Spalten
				for(int l=yOben; l<=yUnten; l++)
				{
					Vram[k + l*Zeilenbreite] = Farbe;
				}
			}
		}
		lpDDSurf->Unlock(NULL);
		return TRUE;
}//x

BOOL xxxMale_Linie(int sx,int sy,int ex,int ey,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf)
{
	/*
	 *	MIT FREUNDLICHER UNTERSTÜTZUNG VON BRESENHAMS LINIENALGORHYTMUS
	 */
	
	int				Zeilenbreite;	// Zeilenbreite
	int				DDS_Breite;		// Breite der Surface
	int				DDS_Hoehe;		// Hoehe der Surface
	int				x_wert,y_wert;	// Änderungen in x und y
	int				x_diff,y_diff;	// Differenzen in x und y
	int				offset,Fehler,Laenge;
	UCHAR			*Vram;			// Speicheradresse der Surface
	DDSURFACEDESC2	ddsd;		// SurfaceDesc Strucktur

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	lpDDSurf->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	Zeilenbreite	= ddsd.lPitch;
	DDS_Breite		= ddsd.dwWidth;
	DDS_Hoehe		= ddsd.dwHeight;
	Vram			= (UCHAR*)ddsd.lpSurface;

	// INITIALISIERUNGEN
	offset  = sy*Zeilenbreite+sx;	// Startadresse im VideoRAM
	y_diff	= ey - sy;				// Hoehe der Linie
	x_diff	= ex - sx;				// Breite der Linie
	Fehler	= 0;

	if(y_diff<0)
	{
		y_diff	= -y_diff;			// Absolutbetrag der Hoehe
		y_wert	= -Zeilenbreite;	// y läuft von unten nach oben
	}
	else y_wert = Zeilenbreite;		// y läuft von oben nach unten

	if(x_diff<0)
	{
		x_diff = -x_diff;			// Absolutbetrag der Breite
		x_wert = -1;				// x läuft von rechts nach links
	}
	else x_wert = 1;				// x läuft von links nach rechts

	// BRESENHAMS LINIE
	if(x_diff > y_diff)				// Steigung < 1
	{
		Laenge	= x_diff + 1;		
		for(int i=0;i<Laenge;i++)	// Schleife über x Koordinate
		{
			Vram[offset] = Farbe;	// Pixel malen !!!
			offset	+= x_wert;		// x Koordinate verschieben
			Fehler  += y_diff;		// Bresenhams Fehler anpassen
			if(Fehler>x_diff)
			{
				Fehler	-= x_diff;	// Fehler neu ansetzen
				offset	+= y_wert;	// y Koordinate verschieben
			}
		}
	}
	else							// Steigerung >= 1
	{
		Laenge	= y_diff +1;		// Schleife über x Koordinate
		for(int i=0;i<Laenge;i++)
		{
			Vram[offset] = Farbe;	// Pixel malen
			offset	+= y_wert;		// y Koordinate verschieben
			Fehler  += x_diff;		// Bresenhams Fehler anpassen
			if(Fehler>0)
			{
				Fehler	-= y_diff;	// Fehler neu ansetzen
				offset	+= x_wert;	// x Koordinate verschieben
			}
		}
	}
	lpDDSurf->Unlock(NULL);
	return TRUE;
}//x