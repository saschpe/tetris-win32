// datei: xxxspielfeld.cpp

/*
 *	Disclaimer: Spielfeld generieren und malen
 */

// INCLUDES

#include<stdio.h>			// für sprintf
#include"XXXGlobal.h"

// GLOBALS

extern HWND				hwnd;	// handle des 'Fullscreen-Fensters'
extern LPDIRECTDRAW7	lpDD;	// DirectDraw Hauptobjekt
extern int			x_Scroll;	// Speichert Scroll-Werte
extern int			y_Scroll;	// Speichert Scroll-Werte

int				Spielfeld[FELD_HOEHE][FELD_BREITE];		// Das Spielfeld
XXX_ADVSPRITE	Spielfeldgrafik[ANZ_FELDER];			// Grafik aller Felder
char			buffer[40];								// für Textausgabe

// FUNKTIONEN

/*
 *	Generieren des Spielfelds: Dazu wird eine Surface erwartet,die alle versch.
 *	Grafiken der Spielfelder untereinander enthält. Genauer sind das gleich
 *	ANZ_FELDER Bilder der Abmessung FELD_HOEHE * FELD_BREITE ( siehe XXXGlobal.h)
 */
BOOL xxxSFeld_generieren(LPDIRECTDRAWSURFACE7 lpDD)
{
	// 1. grafiken aller versch. felder in je ein sprite laden
	for(int i=0;i<ANZ_FELDER;i++)
	{
		xxxADVS_erstellen(&Spielfeldgrafik[i],0,0,FELD_BREITE,FELD_HOEHE,1);
		xxxADVS_Bilder_laden(&Spielfeldgrafik[i],lpDD,1,i);
	}

	// 2. gesamtes spielfeld zufällig erzeugen
	srand(GetTickCount());
	for(int j=0;j<FELDER_SPALTE;j++)
	{
		for(int k=0;k<FELDER_ZEILE;k++)
		{
			if((rand() %2)==0)
				Spielfeld[j][k] = 0;
			else
				Spielfeld[j][k] = 4;
		}
	}

	// 3. strasse malen: zuerst 3 von links oben nach rechts unten
	int m=1;
	for(int n=1;n<=3;n++)
	{
		// Strassen -> m=horizontal(Spalte) | l=vertikal(Zeile)
		for(int l=0;l<FELDER_SPALTE;l++)
		{
			Spielfeld[l][m]	=1;
			l++;
			Spielfeld[l][m] =1;
			m++;
			if(m>=FELDER_ZEILE)
				break;
		}
		m += 3;		// nächste strasse verschieben
	}

	// 4. noch eine strasse: zuerst 3 von links unten nach rechts oben
	int p=2;
	for(int r=1;r<=3;r++)
	{
		// Strassen -> p=horizontal(Spalte) | q=vertikal(Zeile)
		for(int q=FELDER_SPALTE-1;q>=0;q--)
		{
			// Falls sich zwei strassen treffen, dann kreuzung
			if(Spielfeld[p][q]==1)
				Spielfeld[p][q] = 3;
			else
				Spielfeld[p][q] = 2;
			q--;
			p++;
			if(Spielfeld[p][q]==1)
				Spielfeld[p][q] = 3;
			else
				Spielfeld[p][q] = 2;
			if(p>=FELDER_ZEILE)
				break;
		}
		p += 2;		// nächste strassen verschieben
	}
	return TRUE;
}//x
				
/*
 *	Malen des Feldes in die angegebene DirectDraw Surface. Dabei werden die Felder
 *  jeweils Spalten für Spalte gemlt, also die von oben nach unten, dann eine Spalte
 *	weiter nach rechts usw.
 */
BOOL xxxSFeld_malen(LPDIRECTDRAWSURFACE7 lpDDS)
{
	int x=0,y=0;					// koordinaten zum malen
	int y_offset = FELD_HOEHE  /2;	// wg. ISO-Perspektive nur 0,5
	int x_offset = FELD_BREITE /2;	// wg. ISO-Perspektive nur 0,5
	
	/* äussere schleife für die zeilen (horizontal) */
	for(int i=0;i<FELDER_ZEILE;i++)
	{
		// ganze zeile ist zu hoch für den bildschirm
		if(y-y_Scroll < - (FELD_HOEHE+y_offset))
		{
			y += y_offset;
			continue;		// diesen schleifendurchgang beenden
		}
		// alle folgenden zeilen zu tief für den bildschirm
		else if(y-y_Scroll > FELD_HOEHE + SCREEN_HOEHE)
			break;			// ganze schleife beenden
		
		/* innere Schleife für alle spalten einer zeile */
		for(int j=0;j<FELDER_SPALTE;j++)
		{
			Spielfeldgrafik[Spielfeld[i][j]].x = x - x_Scroll;
			Spielfeldgrafik[Spielfeld[i][j]].y = y - y_Scroll;

			// feld zu weit links für den bildschirm
			if(x-x_Scroll < - (FELD_BREITE+FELD_BREITE))
			{
				x += FELD_BREITE;
				continue;
			}
			// alle folgenden felder zu weit rechts für den bildschirm
			else if(x-x_Scroll > ((FELD_BREITE+FELD_BREITE)+SCREEN_HOEHE))
				break;
			// sonst können wir das feld beruhigt malen
			else
			{
				xxxADVS_malen(&Spielfeldgrafik[Spielfeld[i][j]],lpDDS);
			}
			x += FELD_BREITE;
		}

		// Wenn i ungerade war, dann nächster Durchlauf ohne offset
		// gerade Zeilen werden also ganz links gezeichnet,ungerade
		// Zeilen eine halbe Feldbreite nach rechts verschoben
		if(i&1)
			x = 0;
		else
			x = x_offset;

		// y Koordinate für nächstes Feld berechnen. Jede neue Zeile
		// wird um eine halbe Feldhöhe nach unten verschoben
		y += y_offset;
	}
	return TRUE;
}//x
