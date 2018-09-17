// datei: xxxobjekt.cpp

/*
 *	Disclaimer: Datei für alle Objekte im Spiel (verdammt viel Zeug)
 */

// INCLUDES
#include<stdio.h>		// für sprintf
#include"XXXGlobal.h"

// GLOBALS

// XXXMain Variablen
extern HWND			hwnd;						// Hauptfensterhandle
extern int			x_Scroll,y_Scroll;			// Scrolling Werte
extern XXX_OBJEKT	Objektliste[ANZ_OBJ_GESAMT];// Objektliste

extern LPDIRECTDRAWSURFACE7 lpDDSBack,lpDDSFeldgrafiken;

// XXXInput Variablen
extern int Maus_x,Maus_y;			// Maus Koordinaten
extern BOOL Eingabe[AK_MAXIMAL];	// Eingabe-Aktionen

XXX_ADVSPRITE Ggn1_Grafik,Ggn2_Grafik,Trm_Grafik,Splr_Grafik,Hdns_Grafik;

// Verkette Liste für dynamische Objektanzahl
XXXList *pLaserliste;

int Spieler_x,Spieler_y;
int Spieler_Energie;
int anz_Laser = 0;

// FUNKTIONEN	( tons of fuckin needed function crap...:-))

/*
 *	Erzeugen der Objektliste und Start-Initialisierung aller Einträge
 */
BOOL xxxObjektliste_erstellen(LPDIRECTDRAWSURFACE7 lpDDSurf)
{
	// Sprite Objekt erstellen
	xxxADVS_erstellen(&Ggn1_Grafik,0,0, 94, 74,13);
	xxxADVS_erstellen(&Ggn2_Grafik,0,0, 94, 74,1 );
	xxxADVS_erstellen(&Trm_Grafik, 0,0, 94, 74,13);
	xxxADVS_erstellen(&Splr_Grafik,0,0, 94, 74,13);
	xxxADVS_erstellen(&Hdns_Grafik,0,0,201,101,6 );

	// Grafiken für Sprites laden
	xxxADVS_Bilder_laden(&Ggn1_Grafik,lpDDSurf,8,2);
	xxxADVS_Bilder_laden(&Ggn1_Grafik,lpDDSurf,4,3);
	xxxADVS_Bilder_laden(&Ggn2_Grafik,lpDDSurf,1,4);
	xxxADVS_Bilder_laden(&Trm_Grafik,lpDDSurf,8,6);
	xxxADVS_Bilder_laden(&Trm_Grafik,lpDDSurf,4,7);
	xxxADVS_Bilder_laden(&Splr_Grafik,lpDDSurf,8,0);
	xxxADVS_Bilder_laden(&Splr_Grafik,lpDDSurf,4,1);
	xxxADVS_Bilder_laden(&Hdns_Grafik,lpDDSFeldgrafiken,3,3);
	xxxADVS_Bilder_laden(&Hdns_Grafik,lpDDSFeldgrafiken,2,4);

	// Startwert der Zufallszahlen angeben, basierend
	// auf Zeit wird sie immer unterschiedlich
	srand(GetTickCount()%SCREEN_HOEHE);

	for(int i=0;i<ANZ_OBJ_GESAMT;i++)
	{
		// Startwerte festlegen, Position nur innerhalb des Spielfeldes
		Objektliste[i].x	= (rand()%(FELDER_ZEILE * FELD_BREITE -500  ))+200;
		Objektliste[i].y	= (rand()%(FELDER_SPALTE* FELD_HOEHE  /2-300))+100;
		Objektliste[i].zx	= 50;			// Zielposition
		Objektliste[i].zy	= 50;			// Zielposition
		Objektliste[i].vx	= 0;			// Geschwindigkeit
		Objektliste[i].vy	= 0;			// Geschwindigkeit
		Objektliste[i].Zielrichtung = W;
		Objektliste[i].Zeitnehmer	= 0;
		Objektliste[i].Zustand		= ANGRIFF;
		Objektliste[i].Akt_Bild		= W;

		// Typ festlegen und Bilder laden
		if(i==0)
		{
			Objektliste[i].x	= 50;
			Objektliste[i].y	= 50;
			Objektliste[i].Typ	= OBJ_SPIELER;
			Objektliste[i].Energie	= ENERGIE_SPLR;
			Objektliste[i].Bilder	= &Splr_Grafik;
			Objektliste[i].Akt_Bild	= N;
		}
		else if(i<=ANZ_HINDERNIS)
		{
			Objektliste[i].Typ	= OBJ_HINDERNIS;
			Objektliste[i].Energie	= ENERGIE_HDNS;
			Objektliste[i].Bilder	= &Hdns_Grafik;
			Objektliste[i].Akt_Bild	= rand()%5;	// 1 von 5 beliebig
		}
		else if(i<=ANZ_HINDERNIS+ANZ_GEGNER_1)
		{
			Objektliste[i].Typ	= OBJ_GEGNER_1;
			Objektliste[i].Energie	= ENERGIE_GGN1;
			Objektliste[i].Bilder	= &Ggn1_Grafik;
		}
		else if(i<=ANZ_HINDERNIS+ANZ_GEGNER_1+ANZ_GEGNER_2)
		{
			Objektliste[i].Typ	= OBJ_GEGNER_2;
			Objektliste[i].Energie	= ENERGIE_GGN2;
			Objektliste[i].Bilder	= &Ggn2_Grafik;
		}
		else if(i<=ANZ_HINDERNIS+ANZ_GEGNER_1+ANZ_GEGNER_2+ANZ_LASERTURM)
		{
			Objektliste[i].Typ	= OBJ_LASERTURM;
			Objektliste[i].Energie	= ENERGIE_TRM;
			Objektliste[i].Bilder	= &Trm_Grafik;
			Objektliste[i].Zeitnehmer	= 10;
			Objektliste[i].Akt_Bild		= N_NW;
		}
	}

	// verkettete Liste initialisieren
	if(xxx_List_Create(&pLaserliste) != XXX_OK)
		MessageBox(hwnd,"Objekt Fehler: Initialisierung der Laserliste","XXX",MB_OK);

	return TRUE;
}//x


/*
 *	Erzeugt einen Laserschuss und fügt ihn in die verkettete Liste ein.
 *	Übergeben werden Startposition und Flugvektor
 */
BOOL xxxObjekt_Laserschuss_erzeugen(int x,int y,		// Position
									int zx,int zy,		// Zielkoordinaten
									int Farbe)			// Partikelfarbe
{
	XXX_OBJEKT_PTR	pLaserschuss;

	pLaserschuss = (XXX_OBJEKT_PTR)malloc(sizeof(XXX_OBJEKT));

	pLaserschuss->x				= x;				// Startposition x
	pLaserschuss->y				= y;				// Startposition y
	pLaserschuss->zx			= zx;				// Zielposition x
	pLaserschuss->zy			= zy;				// Zielposition y
	pLaserschuss->vx			= 0;				// Geschwindigkeit x
	pLaserschuss->vy			= 0;				// Geschwindigkeit y
	pLaserschuss->Zielrichtung	= W;				// irrelevant
	pLaserschuss->Zeitnehmer	= 0;				// Für Lebensdauer
	pLaserschuss->Zustand		= SUCHEN;			// irrelevant
	pLaserschuss->Akt_Bild		= Farbe;			// Feldmissbrauch!!!
	pLaserschuss->Typ			= OBJ_LASERSCHUSS;	// Typ
	pLaserschuss->Energie		= ENERGIE_SPLR;		// Schadenswert
	pLaserschuss->Bilder		= NULL;				// irrelevant

	// Geschwindigkeit für beide Achsen festlegen
	xxxObjekt_Richtung(pLaserschuss);

	// Schuss verschieben, damit er seinen Schöpfer nicht trifft
	pLaserschuss->x	+= 3 * pLaserschuss->vx;
	pLaserschuss->y	+= 3 * pLaserschuss->vy;

	if(xxx_List_Insert(&pLaserliste,pLaserschuss) != XXX_OK)
	{
		MessageBox(hwnd,"Objekt Fehler: Laserschuss erzeugen","XXX",MB_OK);
		return FALSE;
	}
	anz_Laser++;
	return TRUE;
}//x

/*
 *	Sortiert die Objektliste nach aufsteigenden y Positionen der Objekte
 *	um sie durch den Painter-Algorithmuskorrekt zu malen
 */
BOOL xxxObjektliste_sortieren(void)
{
	// S O R T I E R E N mit quicksort
	qsort((void*)Objektliste,				// welches Array
		  ANZ_OBJ_GESAMT,					// Anz der Elemente
		  sizeof(XXX_OBJEKT),				// Grösse eines Elements
		  (VERGLEICH)xxxObjektvergleich);	// Vergleichsfunktion
	
	return TRUE;
}//x

/*
 *	Diese Funktion führt den eigentlichen Vergleich der y Positionen zweier Objekte durch
 */
BOOL xxxObjektvergleich(const XXX_OBJEKT *arg1,const XXX_OBJEKT *arg2)
{
	// Tausch der Rückgabewerte 1 bzw. -1 kehrt die Sortierreihenfolge um.
	// So beginnen wir mit dem Kleinsten:
	if(arg1->y > arg2->y)
		return 1;
	else if(arg1->y < arg2->y)
		return -1;
	else
		return 0;
}//x

/*
 *	Malt doe Objektliste (Spieler,Gegner..) auf die übergebene Surface
 */
BOOL xxxObjektliste_malen(LPDIRECTDRAWSURFACE7 lpDDSurf)
{
	char buffer[30];
	for(int i=0;i<ANZ_OBJ_GESAMT;i++)
	{
		// Welt- zu Bildschirmkoordinaten umrechnen
		Objektliste[i].Bilder->x = Objektliste[i].x - x_Scroll;
		Objektliste[i].Bilder->y = Objektliste[i].y - y_Scroll;
		// Aktuelles Bild vom Objekt für das Sprite setzen
		Objektliste[i].Bilder->Akt_Bild = Objektliste[i].Akt_Bild;

		xxxADVS_malen(Objektliste[i].Bilder,lpDDSurf);

		if((Objektliste[i].Typ == OBJ_GEGNER_1) ||
		   (Objektliste[i].Typ == OBJ_LASERTURM))
		{
			sprintf(buffer,"E: %d",Objektliste[i].Energie);
			// Text mit Schattierung malen
			xxxMale_Text(Objektliste[i].x - x_Scroll + 10,
						 Objektliste[i].y - y_Scroll -  2,
						 200,lpDDSBack,buffer);
			xxxMale_Text(Objektliste[i].x - x_Scroll +  9,
						 Objektliste[i].y - y_Scroll -  1,
						 10,lpDDSBack,buffer);
		}
	}
	// Falls Schuesse vorhanden, dann diese auch malen
	if(anz_Laser > 0)
		xxxObjektliste_Laser_malen(lpDDSurf);
	return TRUE;
}//x

/*
 *	Malt die Laserliste (Schüsse von Spieler,Gegner und Türmen) auf die übergene Surface
 */
BOOL xxxObjektliste_Laser_malen(LPDIRECTDRAWSURFACE7 lpDDSurf)
{
	XXXList			*temp;
	XXX_OBJEKT_PTR	pObj;
	int				i=0;
	
	temp = pLaserliste;

	// durchlaufe die Liste bis zum ende
	while(temp != NULL)
	{
		pObj = (XXX_OBJEKT_PTR)temp->data;
		temp = temp->next;
		i++;

		// nur malen, wenn im sichtbaren bereich
		if( (pObj->x - x_Scroll > 0)				&&
			(pObj->x - x_Scroll < SCREEN_BREITE-1)	&&
			(pObj->y - y_Scroll > 0)				&&
			(pObj->y - y_Scroll < SCREEN_HOEHE-1) )
		{
			xxxMale_Rechteck(pObj->x-1 - x_Scroll,		// oben links in
							 pObj->y-1 - y_Scroll,		// Bildschirmkoordinaten
							 pObj->x - x_Scroll,		// unten rechts in
							 pObj->y - y_Scroll,		// Bildschirmkoordinaten
							 pObj->Akt_Bild,			// Farbe
							 lpDDSurf,FALSE);			// Surface, solide
		}
	}
	// Speicher freigeben
	free(temp);
	return TRUE;
}//x

/*
 *	Durchläuft alle Objekte des Arrays und bewegt sie.
 *	Dabei wird auch bereits die KI (KünstlicheDummheit :-)) ausgeführt.
 */
BOOL xxxObjekte_bewegen(void)
{
	for(int i=0;i<ANZ_OBJ_GESAMT;i++)
	{
		switch(Objektliste[i].Typ)
		{
		case OBJ_SPIELER:
			if(!Objektliste[i].Zustand == ZERSTOERT)
			{
				int x,y,Ziel_Entfernung_2;

				// Entweder neuer Zielpunkt gewünscht
				if(Eingabe[AK_KLICK])
				{
					// Zielposition für dieses objekt speichern
					// Scrollkoordinaten nicht vergessen
					Objektliste[i].zx = Maus_x + x_Scroll;
					Objektliste[i].zy = Maus_y + y_Scroll;
					Objektliste[i].Zeitnehmer = 50; // ca. 2 Sekunden
					// Ausrichtung berechnen und setzen
					xxxObjekt_Richtung(&Objektliste[i]);
				}
				// Oder Zeit auf altem Kurs korrigieren
				else if(Objektliste[i].Zeitnehmer > 0)
					Objektliste[i].Zeitnehmer--;
				// Oder nach t=50 Kurs zum alten Ziel neu berechnen
				else
				{
					Objektliste[i].Zeitnehmer = 50;
					xxxObjekt_Richtung(&Objektliste[i]);
				}

				// Wümscht das Objekt eine Drehung?
				if(Objektliste[i].Zielrichtung != Objektliste[i].Akt_Bild)
				{
					xxxObjekt_Drehung(&Objektliste[i]);
				}
				// Wenn keine Drehung,dann bewegen
				else
				{
					// Vektor zum Ziel berechnen (Zielpunkt minus Startpunkt)
					x = Objektliste[i].zx - (Objektliste[i].x + 48);
					y = Objektliste[i].zy - (Objektliste[i].y + 37);
					Ziel_Entfernung_2 = x*x + y*y;

					// Wenn nicht nahe dem Ziel dann bewegen
					if(Ziel_Entfernung_2 > 10000)
					{
						Objektliste[i].x += Objektliste[i].vx;
						Objektliste[i].y += Objektliste[i].vy;
					}
				}

				if((Eingabe[AK_SCHIESSEN]) && (anz_Laser < ANZ_LASER_GESAMT))
				{
					xxxObjekt_Laserschuss_erzeugen(Objektliste[i].x + 48,
												   Objektliste[i].y + 37,
												   Objektliste[i].zx + 48,
												   Objektliste[i].zy + 37, 253);
				}

				Spieler_Energie = Objektliste[i].Energie;
				Spieler_x = Objektliste[i].x+48 + 7*Objektliste[i].vx;
				Spieler_y = Objektliste[i].y+37 + 7*Objektliste[i].vy;
			}
			break;
		case OBJ_HINDERNIS:
			// Häuser bewegen sich nicht!
			break;
		case OBJ_GEGNER_1:
			// Falls nicht zerstoert bewegen
			if(!Objektliste[i].Zustand == ZERSTOERT)
				xxxObjekt_KI_Buggy(&Objektliste[i]);
			break;
		case OBJ_GEGNER_2:
			// nicht implementiert
			break;
		case OBJ_LASERTURM:
			// Falls nicht zerstoert bewegen
			if(!Objektliste[i].Zustand == ZERSTOERT)
				xxxObjekt_KI_Turm(&Objektliste[i]);
			break;
		default:break;
		}
	}

	// Falls Schuesse vorhanden, dann diese auch bewegen
	if(anz_Laser>0)
		xxxObjekte_Laserschuss_bewegen();

	return TRUE;
}//x

/*
 *	Durchläuft die Laserliste und bewegt alle Schüsse und löscht die
 *	Schüsse, die ihre Lebensdauerr hinter sich haben
 */
BOOL xxxObjekte_Laserschuss_bewegen(void)
{
	XXXList			*temp;
	XXX_OBJEKT_PTR	pObj;
	BOOL			bKollision = FALSE;
	int				i=0;

	temp = pLaserliste;

	while(temp != NULL)
	{
		pObj = (XXX_OBJEKT_PTR)temp->data;
		temp = temp->next;
		i++;

		bKollision = xxxObjekt_Laserkollisionstest(pObj);

		// Falls  Lebensdauer zu ende oder Schuss trifft Ziel
		// dann Objekt aus der Liste löschen.
		if((pObj->Zeitnehmer > 20) || (bKollision))
		{
			pObj->Zustand = ZERSTOERT;
			i--;
			anz_Laser--;
			// Wenn wir das letzte Listenelement löschen,dann
			// müssen wir die Liste nau initialisieren!!!
			if(anz_Laser == 0)
			{
				pLaserliste = NULL;
				xxx_List_Create(&pLaserliste);
				temp = NULL;
			}
			else
				xxx_List_Delete(&pLaserliste,i);
		}
		// Schuss bewegen mit doppelter Objektgeschwindigkeit
		else
		{
			pObj->x += 2*pObj->vx;
			pObj->y += 2*pObj->vy;
			pObj->Zeitnehmer++;
		}
	}
	return TRUE;
}//x

/*
 *	Führt die KI für den Laserturm aus. Dieser dreht sich einfach
 *  nur in Richtung des Spielers und feuert wenn dieser nah genug ist.
 */
BOOL xxxObjekt_KI_Turm(XXX_OBJEKT_PTR pObj)
{
	int	Entfernung_2,dx,dy;
	int xl,xr,yl,yr;

	// Zielkoordinaten für Objekt bestimmen
	pObj->zx = Spieler_x;
	pObj->vy = Spieler_y;

	// Gewünschte Ausrichtung zum Ziel festlegen
	xxxObjekt_Richtung(pObj);

	// Drehung falls benötigt ausführen
	if(pObj->Akt_Bild != pObj->Zielrichtung)
		xxxObjekt_Drehung(pObj);

	// xxxObjekt_Richtung hat vx,vy gesetzt, deshalb:
	pObj->vx = 0;
	pObj->vy = 0;

	// Vektor zum Ziel berechnen
	dx = pObj->x - Spieler_x + 48;
	dy = pObj->y - Spieler_y + 37;
	Entfernung_2 = dx*dx + dy*dy;

	// Nah genug für einen Schuss?
	if((Entfernung_2 < 55000) && (anz_Laser < ANZ_LASER_GESAMT) && (rand()%3 == 1))
	{
		// OFFSET der beiden Schuesse bestimmen, damit sie sich nicht überlappen
		// nach oben schiessen
		if((pObj->Zielrichtung > 1) && (pObj->Zielrichtung < 5))
		{
			xl = 3;
			xr = -3;
			yr = yl = -12;
		}
		// nach unten schiessen
		else if((pObj->Zielrichtung > 7) && (pObj->Zielrichtung <11))
		{
			xl = 3;
			xr = -3;
			yr = yl = 12;
		}
		// nach rechts schiessen
		else if((pObj->Zielrichtung >4) && (pObj->Zielrichtung <8))
		{
			xl = xr = 10;
			yl = -8;		// Die Rohre sind nicht ganz in der Mitte
			yr = -2;		// der Grafik => nach oben schieben
		}
		// nach links schiessen
		else if((pObj->Zielrichtung <2) && (pObj->Zielrichtung >10))
		{
			xl = xr = -10;
			yl = -8;		// Die Rohre sind nicht ganz in der Mitte
			yr = -2;		// der Grafik => nach oben schieben
		}

		xxxObjekt_Laserschuss_erzeugen(pObj->x+48+xl,
									   pObj->y+20+yl,
									   pObj->zx+48,
									   pObj->zy+37,
									   253);
		xxxObjekt_Laserschuss_erzeugen(pObj->x+48+xr,
									   pObj->y+20+yr,
									   pObj->zx+48,
									   pObj->zy+37,
									   253);
	}
	return TRUE;
}//x

/*
 *	Führt die KI für den Buggy aus. Dieser kann stur angreifen,
 *	oder einem Hindernis ausweichen
 */
BOOL xxxObjekt_KI_Buggy(XXX_OBJEKT_PTR pObj)
{
	int Ziel_Entfernung_2,x,y;
	int zx,zy;
	int Entfernung_2,dx,dy;

	// Je nach Zustand anderes Verhalten
	if(pObj->Zustand == ANGRIFF)
	{
		// Zielkoordinaten für Objekt bestimmen
		pObj->zx = Spieler_x;
		pObj->zy = Spieler_y;

		if(pObj->Zeitnehmer > 0)
			pObj->Zeitnehmer--;
		// Gewünschte Ausrichtung zum Ziel festlegen
		else
		{
			xxxObjekt_Richtung(pObj);
			// mind. 15 Frames bis zur naechsten Drehung
			pObj->Zeitnehmer = 15;
		}

		// Drehung falls benötigt durchführen
		if(pObj->Akt_Bild != pObj->Zielrichtung)
			xxxObjekt_Drehung(pObj);
		// Wenn keine Drehung,dann bewegen
		else
		{
			x = pObj->zx - (pObj->x + 48);
			y = pObj->zy - (pObj->y + 37);
			
			Ziel_Entfernung_2 = x*x + y*y;
			
			// In der nähe des Zielpunktes?
			if(Ziel_Entfernung_2 < 65000)
			{
				pObj->vx = 0;
				pObj->vy = 0;
			}
			// Wenn nicht, dann bewegen
			else
			{
				pObj->x += pObj->vx;
				pObj->y += pObj->vy;
			}
		}

		// Nah genug für einen Schuss?
		dx = pObj->x - Spieler_x + 48;
		dy = pObj->y - Spieler_y + 37;
		Entfernung_2 = dx*dx + dy*dy;

		if((Entfernung_2 < 75000) && (anz_Laser < ANZ_LASER_GESAMT) && (rand()%4 == 1))
		{
			// Laserschuss mit Offset in Fahrtrichtung, um sich nicht selbst zu treffen!
			xxxObjekt_Laserschuss_erzeugen(pObj->x+48 + 6*pObj->vx,
										   pObj->y+37 + 6*pObj->vy,
										   pObj->zx+48,
										   pObj->zy+37,
										   253);
		}
	}

	// Hindernis ausweichen, wenn Zustand AUSWEICHEN eingeleited
	// word, dann 'Zeitnehmer' auf 35 gesetzt:
	else if(pObj->Zustand == AUSWEICHEN)
	{
		// Ein Stück zurück setzen
		if(pObj->Zeitnehmer > 25)
		{
			pObj->x -= pObj->vx;
			pObj->y -= pObj->vy;
			pObj->Zeitnehmer--;
		}
		// 90 Grad wegdrehen
		else if(pObj->Zeitnehmer == 25)
		{
			// Richtungsvektor: y Vorzeichenwechsel, dann x und y tauschen
			// => ca. 90 Grad nach rechts gedreht!
			zx = -pObj->vy;
			zy = pObj->vx;
			// Neue Zielposition 90 Grad neben dem Objekt
			pObj->zx = pObj->vx + 48 + 5*zx;
			pObj->zy = pObj->vy + 37 + 5*zy;

			// Gewünschte Ausrichtung zum neuen Ziel festlegen
			xxxObjekt_Richtung(pObj);
			pObj->Zeitnehmer--;
		}
		// Im 90 Grad Winkel ein Stück wegfahren
		else
		{
			// Drehung falls benötigt ausführen
			if(pObj->Akt_Bild != pObj->Zielrichtung)
				xxxObjekt_Drehung(pObj);
			else
			{
				pObj->x += pObj->vx;
				pObj->y += pObj->vy;
				pObj->Zeitnehmer--;
			}

			if(pObj->Zeitnehmer <=0)
			{
				pObj->Zustand = ANGRIFF;
				pObj->Zeitnehmer = 0;
			}
		}
	}
	return TRUE;
}//x

/*
 *	Setzt das aktuelle_bild des Objektes von der aktuellen Richtung eins weiter
 *  um sich der Zielausrichtung anzunähern
 */
BOOL xxxObjekt_Drehung(XXX_OBJEKT_PTR pObj)
{
	if(pObj->Zielrichtung > pObj->Akt_Bild)
	{
		// Wenn Differenz grösser als halber Kreis, dann abziehen
		if(pObj->Zielrichtung - pObj->Akt_Bild >= 6)
			pObj->Akt_Bild--;
		else 
			pObj->Akt_Bild++;
	}
	else
	{
		if(pObj->Akt_Bild - pObj->Zielrichtung > 6)
			pObj->Akt_Bild++;
		else
			pObj->Akt_Bild--;
	}

	// Grenze von 12 Bildern einhalten
	if(pObj->Akt_Bild <= -1)
		pObj->Akt_Bild = 11;
	else if(pObj->Akt_Bild >= 12)
		pObj->Akt_Bild = 0;

	return TRUE;
}//x

/*
 *	Kollisionsabfrage über die gesamte Objektliste. Behandelt auch gleich die
 *  folgen für alle kollidierenden Objekte, ist allerdings sehr primitiv,da die
 *	Kollision unabhängig von der Drehung des Objektes anhand eine Bounding Sphere
 *	berechnet wird. Bei einigen Häusergrafiken wirkt das unschön
 */
BOOL xxxObjekt_Kollisionstest(void)
{
	BOOL	Kollision;				// TRUE oder FALSE
	int		Entfernung_2;			// quadrat. Entfernung zweier Objekte
	int		dx,dy;					// Vektor (dx,dy) zw. zwei Objekten
	int		x_offset_i = 48,		// Mittelpunkt der Objekt kann
			y_offset_i = 37,		// sich ändern, wenn Objekte nicht
			x_offset_j = 48,		// die Standardgrösse hat
			y_offset_j = 37;		// Hier nötig für OBJ_HINDERNIS

	// Kollisionsabfrage für alle Objekte der Liste
	for (int i=0;i<ANZ_OBJ_GESAMT;i++)
	{
		if(Objektliste[i].Typ == OBJ_HINDERNIS)
		{
			x_offset_i = 100;
			y_offset_i = 50;
		}
		else
		{
			x_offset_i = 48;
			y_offset_i = 37;
		}
		// Jedes Objekt nur mit den Nachfolgenden testen
		// um reduntdante tests zu vermeiden
		for(int j=i+1;j<ANZ_OBJ_GESAMT;j++)
		{
			if(Objektliste[i].Typ == OBJ_HINDERNIS)
			{
				x_offset_j = 100;
				y_offset_j = 50;
			}
			else
			{
				x_offset_j = 48;
				y_offset_j = 37;
			}

			Kollision = FALSE;

			dx = (Objektliste[i].x + x_offset_i) - 
				 (Objektliste[j].x + x_offset_j);
			dy = (Objektliste[i].y + y_offset_i) -
				 (Objektliste[j].y + y_offset_j);

			Entfernung_2 = dx*dx + dy*dy;

			// Kollision mit Hindernis (Haus)
			if((Objektliste[i].Typ == OBJ_HINDERNIS || 
				Objektliste[j].Typ == OBJ_HINDERNIS)
				&& (Entfernung_2) < 7500)
				Kollision = TRUE;
			// sonstige Kollisionen
			else if(Entfernung_2 < 3500)
				Kollision = TRUE;

			if(Kollision == TRUE)
			{
				// Position zurückversetzen, damit Objekt nicht für immer
				// im Kollisionsradius gefangen ist!
				Objektliste[i].x -= 2*Objektliste[i].vx;
				Objektliste[i].y -= 2*Objektliste[i].vy;
				Objektliste[i].zx = Objektliste[i].x+48;	// Zielposition
				Objektliste[i].zy = Objektliste[i].y+37;	// als erreicht
				// dasselbe für das zweite Objekt
				Objektliste[j].x -= 2*Objektliste[j].vx;
				Objektliste[j].y -= 2*Objektliste[j].vy;
				Objektliste[j].zx = Objektliste[j].x+48;	// Zielposition
				Objektliste[j].zy = Objektliste[j].y+37;	// als erreicht

				// Falls ein Kollisionspartner ein beweglicher Gegner war,
				// dann muss er ausweichen, falls beide dann nur einer
				// sonst Kollision auf Ausweichpfad
				if((Objektliste[i].Typ == OBJ_GEGNER_1) &&
				   (Objektliste[i].Zustand != ZERSTOERT))
				{
					Objektliste[i].Zustand    = AUSWEICHEN;
					Objektliste[i].Zeitnehmer = 35;
				}
				else if((Objektliste[j].Typ == OBJ_GEGNER_1) && 
						(Objektliste[j].Zustand != ZERSTOERT))
				{
					Objektliste[j].Zustand    = AUSWEICHEN;
					Objektliste[j].Zeitnehmer = 35;
				}
			}
		}
	}
	return TRUE;
}//x


/*
 *	Kollisionsabfrage über die gesamte Objektliste mit einem Laserschuss.
 *  Gibt TRUE zurück, falls Kollision stattgefunden hat und behandelt dir Folgen
 *	für das getroffene Objekt,aber nicht den Schuss selber
 */
BOOL xxxObjekt_Laserkollisionstest(XXX_OBJEKT_PTR pObj)
{
	int Entfernung_2;		// quadrat. Entfernung zweier objekte
	int dx,dy;				// Vektor (dx,dy) zw. zwei Objekten
	int x_offset = 48,
		y_offset = 37;		// Mittelpunkt des Objektes
	int Radius_2;			// Quadr. Kollisionsradius

	//Kollisionsabfrage für alle Objekte der Liste
	for(int i=0;i<ANZ_OBJ_GESAMT;i++)
	{
		// Totes Objekt übergeben
		if(Objektliste[i].Zustand == ZERSTOERT)
			break;

		if(Objektliste[i].Typ == OBJ_LASERTURM)
		{
			y_offset = 20;	// Mitte des Objektes anders
			Radius_2 = 200;	// schwerer zu treffen
		}
		else
		{
			Radius_2 = 400;
			y_offset = 37;
		}

		dx = Objektliste[i].x + x_offset - pObj->x;
		dy = Objektliste[i].y + y_offset - pObj->y;
		Entfernung_2 = dx*dx + dy*dy;

		if((Entfernung_2 < Radius_2) && (Objektliste[i].Energie > 0))
		{
			// TREFFER
			Objektliste[i].Energie -= ENERGIE_LSR;

			if(Objektliste[i].Energie <= 0)
			{
				Objektliste[i].Zustand = ZERSTOERT;
				Objektliste[i].vx	   = 0;
				Objektliste[i].vy	   = 0;
			}
			// Ein Schuss kann immer nur ein Ziel treffen
			return TRUE;
		}
	}
	return FALSE;
}//x

/*
 *	Berechnet von der Position des Objektes den Vektor zum Ziel und anhand des
 *	Winkels des Vektors wird die Geschwindigkeit und die gewünschte
 *	Zielausrichtung gesetzt
 */
BOOL xxxObjekt_Richtung(XXX_OBJEKT_PTR pObj)
{
	int		ux,uy;		// Vektor von obj zu dessen Ziel
	float	m;			// Steigung des Vektors (ux,uy)

	// Vektor zum Ziel berechnen: Zielpunkt minus Startpunkt
	ux	= pObj->zx - (pObj->x + 48);
	uy	= pObj->zy - (pObj->y + 37);

	// Steigung berechnen
	if(ux!=0)
		m = (float)uy / ux;		// Steigung berechnen
	else if(uy>0)
		m = 4;					// Hauptsache grö0er als 3,7 -> S
	else if(uy<0)
		m = -4;					// Hauptsache kleiner als -3,7 -> N 

	// 1. Vektor zeigt nach rechts
	if(ux>=0)
	{
		// Steigung benutzen
		if(m>3.7)
		{
			pObj->Zielrichtung = S;
			pObj->vx = 0;
			pObj->vy = 7;
		}
		else if(m>1)
		{
			pObj->Zielrichtung = S_SO;
			pObj->vx = 3;
			pObj->vy = 6;
		}
		else if(m>0.26)
		{
			pObj->Zielrichtung = O_SO;
			pObj->vx = 6;
			pObj->vy = 3;
		}
		else if(m>-0.26)
		{
			pObj->Zielrichtung = O;
			pObj->vx = 7;
			pObj->vy = 0;
		}
		else if(m>-1)
		{
			pObj->Zielrichtung = O_NO;
			pObj->vx = 6;
			pObj->vy = -3;
		}
		else if(m>-3.7)
		{
			pObj->Zielrichtung = N_NO;
			pObj->vx = 3;
			pObj->vy = -6;
		}
		else if(m<-3.7)
		{
			pObj->Zielrichtung = N;
			pObj->vx = 0;
			pObj->vy = -7;
		}

	}
	// 2. Vektor zeigt nach links
	else
	{
		if(m>3.7)
		{
			pObj->Zielrichtung = N;
			pObj->vx = 0;
			pObj->vy = -7;
		}
		else if(m>1)
		{
			pObj->Zielrichtung = N_NW;
			pObj->vx = -3;
			pObj->vy = -6;
		}
		else if(m>0.26)
		{
			pObj->Zielrichtung = W_NW;
			pObj->vx = -6;
			pObj->vy = -3;
		}
		else if(m>-0.26)
		{
			pObj->Zielrichtung = W;
			pObj->vx = -7;
			pObj->vy = 0;
		}
		else if(m>-1)
		{
			pObj->Zielrichtung = W_SW;
			pObj->vx = -6;
			pObj->vy = 3;
		}
		else if(m>-3.7)
		{
			pObj->Zielrichtung = S_SW;
			pObj->vx = -3;
			pObj->vy = 6;
		}
		else if(m<-3.7)
		{
			pObj->Zielrichtung = S;
			pObj->vx = 0;
			pObj->vy = 7;
		}
	}
	return TRUE;
}//x
