// Datei: xxxbitmap.cpp

/*
 *	Disclaimer: hier gibts Sprite- und BitmapFunktionen
 */

// INCLUDES
#include<io.h>
#include<ddraw.h>		// DirectDraw Bibliothek
#include"XXXGlobal.h"


// GLOBALS
extern HWND					hwnd;
extern LPDIRECTDRAW7		lpDD;			// directdraw hauptobjekt
extern LPDIRECTDRAWSURFACE7	lpDDSPrimary;	// frontbuffer
extern LPDIRECTDRAWSURFACE7	lpDDSBack;		// backbuffer
extern LPDIRECTDRAWPALETTE	lpDDPal;		// farbpalette des programms


// FUNKTIONEN

/**
 *	Liest das Bitmap und speichert es in der übergebenden Surface und setzt die Palette
 *	der PrimarySurface auf die Palette des Bitmaps falls der 3. Parameter auf TRUE steht
 */
BOOL xxxBMP_in_Surface(const char* BMPDatei,LPDIRECTDRAWSURFACE7 lpDDSurf,BOOL Palette_laden)
{
	LPBYTE				lpbyBild;				// speichert die BMP bilddaten
	HANDLE				hDatei;					// Handle auf die BMP datei
	BITMAPFILEHEADER	BMPFileHead;			// BMP datei header
	BITMAPINFOHEADER	BMPFileInfo;			// BMP datei info header
	RGBQUAD				Palette[256];			// farbpalette
	DWORD				dwLeseSpeicher;			// zwischenspeicher
	DDSURFACEDESC2		ddsd;					// surface description
	LPSTR				lpZielRAM;				// zielspeicher (ddraw surface)
	LPSTR				lpQuellRAM;				// quellspeicher (BMP bild)
	int					DDS_Breite;				// breite der zielsurface
	int					DDS_Hoehe;				// hoehe  der zielsurface

	// Surface (in die das BMP soll) verriegeln
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	lpDDSurf->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	DDS_Breite	=	ddsd.dwWidth;
	DDS_Hoehe	=	ddsd.dwHeight;

	// Speicher für die Bilddaten bereitstellen
	if(!(lpbyBild=(LPBYTE)malloc(DDS_Breite*DDS_Hoehe)))
		return FALSE;

	// BMP Datei öffnen
	hDatei = xxxBMP_oeffne_Datei(BMPDatei);
	if(hDatei == INVALID_HANDLE_VALUE)
		return FALSE;

	// BMP Header einlesen
	if(!ReadFile(hDatei,&BMPFileHead,sizeof(BMPFileHead),&dwLeseSpeicher,NULL))
	{
		CloseHandle(hDatei);
		return FALSE;
	}

	// BMP Info Header einlesen
	if(!ReadFile(hDatei,&BMPFileInfo,sizeof(BMPFileInfo),&dwLeseSpeicher,NULL))
	{
		CloseHandle(hDatei);
		return FALSE;
	}

	// Hat das Bitmap das gewünschte Format
	if((BMPFileInfo.biWidth != DDS_Breite) || (BMPFileInfo.biHeight != DDS_Hoehe) ||
	   (BMPFileInfo.biBitCount != 8))
	{
		CloseHandle(hDatei);
		return FALSE;
	}

	// Lies und speichere doe Palette des BMP
	if(!ReadFile(hDatei,Palette,sizeof(Palette),&dwLeseSpeicher,NULL))
	{
		CloseHandle(hDatei);
		return FALSE;
	}

	//Lies die eigentlichen Bilddaten des BMP
	if(!ReadFile(hDatei,lpbyBild,DDS_Breite*DDS_Hoehe,&dwLeseSpeicher,NULL))
	{
		CloseHandle(hDatei);
		return FALSE;
	}

	// Schliesse die Datei wieder
	CloseHandle(hDatei);

	// Setze die Startadressen der Pointer
	lpZielRAM = (LPSTR)ddsd.lpSurface;
	lpQuellRAM= (LPSTR)(&lpbyBild[(DDS_Hoehe-1)*DDS_Breite]);

	// Kopiere die Bilddaten aus der Variablen 'lpbyBild'
	// zeilenweise in den VRAM der Surface
	for(int j=0;j<DDS_Hoehe;j++)
	{
		memcpy(lpZielRAM,lpQuellRAM,DDS_Breite);
		lpZielRAM += ddsd.lPitch;
		lpQuellRAM -= DDS_Breite;
	}

	// entriegeln der Surface nach dem Schreiben der Daten
	lpDDSurf->Unlock(NULL);
	// Speicher freigeben
	if(lpbyBild)
		free(lpbyBild);

	// Wenn Palette übernommen werden soll, dann DDraw Palette aus
	// der BMP Palette erzeugen und für Primary  Surface setzen
	if(Palette_laden)
	{
		PALETTEENTRY		pe[256];	// Paletteneinträge
		HRESULT				ddrval;		// Rückgabewert

		ZeroMemory(pe,MAX_FARBEN*sizeof(PALETTEENTRY));

		for(int i=0;i<MAX_FARBEN;i++)
		{
			pe[i].peRed   = Palette[i].rgbRed;
			pe[i].peGreen = Palette[i].rgbGreen;
			pe[i].peBlue  = Palette[i].rgbBlue;
			pe[i].peFlags = PC_NOCOLLAPSE;
		}
		ddrval = lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256,pe,&lpDDPal,NULL);
		if(ddrval != DD_OK)
		{
			MessageBox(hwnd,"Bitmap Fehler: CreatePalette()","XXX",MB_OK);
			return FALSE;
		}

		ddrval = lpDDSPrimary->SetPalette(lpDDPal);
		if(ddrval != DD_OK)
		{
			MessageBox(hwnd,"Bitmap Fehler: SetPalette()","XXX",MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}//x
		
/**
 *	Erstellt ein ADVS Objekt für den übergebenen Pointer an der
 *	übergebenen Position in der angegebenen Größe
 */
BOOL xxxADVS_erstellen(XXX_ADVSPRITE_PTR pAdvs,int x,int y,
					   int Breite,int Hoehe,int Max_Bilder)
{
	DDSURFACEDESC2			ddsd;
	DDCOLORKEY				ddck;
	HRESULT					ddrval;

	if(MAX_BILDER > MAX_SPRITE_BILDER) Max_Bilder = MAX_SPRITE_BILDER;

	pAdvs->x			= x;		// startposition
	pAdvs->y			= y;		// startposition
	pAdvs->Breite		= Breite;	// breite des AdvSprites
	pAdvs->Hoehe		= Hoehe;	// hoehe ...
	pAdvs->Akt_Bild		= 0;		// akt. bild (o bis Anz_Bilder -1)
	pAdvs->Anz_Bilder   = 0;		// Aktuelle Gesamtzahl Bilder
	pAdvs->Max_Bilder	= Max_Bilder;	// max. mögl. gesamtzahl bilder
	pAdvs->Startzeit	= 0;		// startzeit für bildanimation

	// alle pointer für potentielle bilddaten NULL'en
	for(int i=0;i<MAX_SPRITE_BILDER;i++)
	{
		pAdvs->lpDDSBilder[i] = NULL;
	}

	// DirectDraw Offscreen Surfaces für Bilddaten erstellen
	for(int k=0;k<pAdvs->Max_Bilder;k++)
	{
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize		= sizeof(ddsd);
		ddsd.dwFlags	= DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
		ddsd.dwWidth	= pAdvs->Breite;
		ddsd.dwHeight	= pAdvs->Hoehe;

		ddrval = lpDD->CreateSurface(&ddsd,&pAdvs->lpDDSBilder[k],NULL);
		if(ddrval != DD_OK)
		{
			MessageBox(hwnd,"Create OffscreenSurface failed: lpDDSBilder","XXX",MB_OK);
			return FALSE;
		}

		// DirectDraw ColorKey für AdvSprite setzen
		ddck.dwColorSpaceLowValue	= 0;
		ddck.dwColorSpaceHighValue	= 0;
		pAdvs->lpDDSBilder[k]->SetColorKey(DDCKEY_SRCBLT,&ddck);
	}
	return TRUE;
}//x

/**
 *	Lädt Bitmap Daten aus einer DDraw Surface in Tiles Form.
 *	Dabei ist das Laden mehrer Tiles aus einer Reihe in einem Rutsch möglich
 */
BOOL xxxADVS_Bilder_laden(XXX_ADVSPRITE_PTR pAdvs,LPDIRECTDRAWSURFACE7 lpDDS,
						  int Anzahl,int Anim_Reihe)
{
	DDSURFACEDESC2		ddsd;
	DDSURFACEDESC2		ddsd2;
	LPSTR				lpZielRAM;			// Zielspeicher ( Advs DDSurface)
	LPSTR				lpQuellRAM;			// Quellspeicher (Surface Bilddaten)
	LPSTR				tmpQuellRAM;		
	int					index;				// Zaehler für vorhandene Bilder
	int					DDS_Zeilenbreite;	// Breite der ZielSurface

	if(!pAdvs) return FALSE;

	ZeroMemory(&ddsd2,sizeof(ddsd2));
	ddsd2.dwSize	= sizeof(ddsd2);
	lpDDS->Lock(NULL,&ddsd2,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL);

		DDS_Zeilenbreite = ddsd2.lPitch;

		// Bilddaten-Quelle setzen:
		// = lpSurface + 1			wg. 1 Pixel x Rahmen um die Bilddaten
		// + Anim_Reihe*ZBreite		wg. evnt. mehrerer Anim.Reihen je BMP
		// + ZBreite				wg. 1 Pixel y Rahmen um die Bilddaten
		lpQuellRAM = (LPSTR)ddsd2.lpSurface +1 
					 +Anim_Reihe*(DDS_Zeilenbreite*(pAdvs->Hoehe-1)) + DDS_Zeilenbreite;
	
	lpDDS->Unlock(NULL);

	// Lade die geforderte Anzahl von BMPs in die Advs Surface
	for(int i=0;i<Anzahl;i++)
	{
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize		= sizeof(ddsd);

		// Welche Bildnummer ist die nächste freie?
		index = pAdvs->Anz_Bilder;
		tmpQuellRAM = lpQuellRAM;

		// Surface verriegeln
		pAdvs->lpDDSBilder[index]->Lock(NULL,&ddsd,DDLOCK_WAIT |
										DDLOCK_SURFACEMEMORYPTR,NULL);

			// ZeilRAM	ist die verriegelte Surface des Advs
			lpZielRAM = (LPSTR)ddsd.lpSurface;
			// Bilddaten zeilenweise kopieren
			for(int y=0;y<pAdvs->Hoehe;y++)
			{
				memcpy(lpZielRAM,tmpQuellRAM+i*(pAdvs->Breite+1),pAdvs->Breite);
				tmpQuellRAM += DDS_Zeilenbreite;
				lpZielRAM	+= ddsd.lPitch;
			}
		pAdvs->lpDDSBilder[index]->Unlock(NULL);	// entriegeln
		pAdvs->Anz_Bilder++;
	}
	return TRUE;
}//x

/**
 *	Ausgabe eines ADVS mit seinem aktuell eingestellten Bild in die übergebene DDraw Surface
 */
BOOL xxxADVS_malen(XXX_ADVSPRITE_PTR pAdvs,LPDIRECTDRAWSURFACE7 lpDDS)
{
	RECT		rcZiel,rcQuelle;	// Ziel- und Quellbereiche
	HRESULT		ddrval;

	if(!pAdvs) return FALSE;

	if(!pAdvs->lpDDSBilder[pAdvs->Akt_Bild]) return FALSE;

	rcZiel.top		= pAdvs->y;
	rcZiel.left		= pAdvs->x;
	rcZiel.bottom	= pAdvs->y + pAdvs->Hoehe;
	rcZiel.right	= pAdvs->x + pAdvs->Breite;

	rcQuelle.top	= 0;
	rcQuelle.left	= 0;
	rcQuelle.bottom = pAdvs->Hoehe;
	rcQuelle.right	= pAdvs->Breite;
	
	ddrval = lpDDS->Blt(&rcZiel,pAdvs->lpDDSBilder[pAdvs->Akt_Bild],&rcQuelle,
						DDBLT_WAIT | DDBLT_KEYSRC,NULL);
	if(ddrval != DD_OK) return FALSE;
	
	return TRUE;
}//x

HANDLE xxxBMP_oeffne_Datei(const char* Datei_name)
{
	HANDLE	hDatei;

	hDatei = CreateFile(Datei_name,					// name
						GENERIC_READ,				// Nur-Lese Zugriff
						FILE_SHARE_READ,			// andere dürfen nur lesen
						(LPSECURITY_ATTRIBUTES)NULL,// HANDLE nicht verrerbbar
						OPEN_EXISTING,				// nur existierende Datei
						FILE_ATTRIBUTE_NORMAL,		// keine speziellen attrib.
						(HANDLE)NULL);				// muss NULL sein
	
	if(hDatei != INVALID_HANDLE_VALUE) return hDatei;

	return INVALID_HANDLE_VALUE;
}//x