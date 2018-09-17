// Datei : xxxglobal.h


/*
 *	Gemeinsamer Header für alle Quellcode-Dateien des Projektes IsoDD_nearly_finished
 */

// INCLUDES
#include<ddraw.h>		// DirectDraw Bibliothek
#include<dinput.h>


// DEFINITIONEN
#define WIN32_LEAN_AND_MEAN



// Spielzustände
#define SPIEL_AUSWAHL		0
#define SPIEL_START			1
#define SPIEL_LAEUFT		2
#define SPIEL_NEUSTART		3
#define SPIEL_BEENDEN		4

// bildschirmabmessungen
#define SCREEN_BREITE		800
#define SCREEN_HOEHE		600
#define SCREEN_BPP			8
#define SCROLL_RATE			12
// Anzahl der Knöpfe
#define MAX_JOYSTICK_BUTTONS	4
#define MAX_MAUS_BUTTONS		4

// Aktionen
#define AK_SCHIESSEN		0
#define AK_RAKETE			1
#define AK_FLIP				2
#define AK_MALEN			3
#define AK_BEENDEN			4
#define AK_KLICK			5
#define AK_SCROLL_L			6
#define AK_SCROLL_R			7
#define AK_SCROLL_U			8
#define AK_SCROLL_D			9
#define AK_STARTEN			10
//---------------------------
#define AK_MAXIMAL			12

// TestFlags
#define	GDIvsDIRECTTEXT		0
#define GDIvsDIRECTRECT		0
#define GDIvsDIRECTLINE		0
#define JOYSTICK_TEST		1

#define XXX_RESULT			int
#define XXX_OK				0
#define XXX_MEMORY_ERROR	1
#define XXX_OUT_OF_BOUNDS	2


#define	MAX_SPRITE_BILDER 50
#define MAX_FARBEN 256
#define MAX_BILDER 12


#define DINPUT_BUFFERSIZE	32
#define XXX_RESULT			int
#define XXX_OK				0
#define XXX_MEMORY_ERROR	1
#define XXX_OUT_OF_BOUNDS	2

// DEFINITIONEN
// Anzahl der Objekte
#define ANZ_HINDERNIS		7
#define ANZ_GEGNER_1		2
#define ANZ_GEGNER_2		0
#define ANZ_LASERTURM		4
//---------------------------
#define ANZ_OBJ_GESAMT	   14	// 1 mehr wegen Spieler
#define ANZ_LASER_GESAMT 1000

// möglich Objekttypen
#define OBJ_SPIELER		0
#define OBJ_GEGNER_1	1
#define OBJ_GEGNER_2	2
#define OBJ_LASERTURM	3
#define OBJ_HINDERNIS	4
#define OBJ_LASERSCHUSS 5

// Energiewerte der Objekte
#define ENERGIE_SPLR	500
#define ENERGIE_GGN1	100
#define ENERGIE_GGN2	 90
#define ENERGIE_TRM		 75
#define ENERGIE_HDNS	 20
#define ENERGIE_LSR		  1

// mögliche Objektzustände
#define ZERSTOERT	0
#define ANGRIFF		1
#define AUSWEICHEN	2

#define SUCHEN		9

// Himmelsrichtungen als Nr. des Bildes
#define W		0	// 270		Westen
#define W_NW	1	// 300		West-Nordwest
#define N_NW	2	// 330		Nord-Nordwest
#define N		3	// 0  Grad	Norden
#define N_NO	4	// 30		Nord-Nordost
#define O_NO	5	// 60		Ost-Nordost
#define O		6	// 90		Osten
#define O_SO	7	// 120		Ost-Südost
#define S_SO	8	// 150		Süd-Südost
#define S		9	// 180		Süden
#define S_SW	10	// 210		Süd-Südwest
#define W_SW	11	// 240		West-Südwest


// DEFINITIONEN

#define FELDER_SPALTE		30		// Felder in einer Spalte
#define FELDER_ZEILE		30		// Felder in einer Zeile
#define ANZ_FELDER			 5		// Anzahl verschiedener Felder
#define FELD_BREITE		   100		// Breite eines Feldes
#define FELD_HOEHE			50		// Hoehe eines Feldes


// MAKROS

	typedef int (*VERGLEICH)  (const void*arg1,const void*arg2);
	#define TASTE(Taste)	(Tasten_Puffer[Taste] & 0x80)

// STRUKTUREN

	typedef struct xxxListItem
	{
		void *data;
		struct xxxListItem *next;
	}XXXList;

	typedef struct XXX_ADVSPRITE_TYP
	{
		int		x,y;			// Position (Schirmkoordinaten)
		int		Breite,Hoehe;	// Breite und Hoehe des Sprites
		int		Anz_Bilder;		// Aktuelle Gesamtanzahl Bilder
		int		Max_Bilder;		// Max. mögl. Gesamtanz. Bilder
		int		Akt_Bild;		// Aktuelles Bild
		DWORD	Startzeit;		// Stopuhr für Bildwechsel
		LPDIRECTDRAWSURFACE7	lpDDSBilder[MAX_SPRITE_BILDER];
	} XXX_ADVSPRITE, *XXX_ADVSPRITE_PTR;

	typedef struct XXX_OBJEKT_TYP
	{
		int		x,y;			// Real-Position (Weltkoordinaten)
		int		zx,zy;			// Ziel-Position (Weltkoordinaten)
		int		vx,vy;			// Geschwindigkeit getrennt nach Achsen
		int		Energie;		// Energie Punkte
		int		Typ;			// Welche Art Objekte
		int		Zustand;		// Zustand des Objektes
		int		Zielrichtung	;//Gewünschte Ausrichtung(N,NW..)
		int		Akt_Bild;		// Akt. Bild (0 bis Anz_Bilder -1)
		DWORD	Zeitnehmer;		// Stopuhr
		XXX_ADVSPRITE_PTR	Bilder;	// Sprite Struktur mit Grafiken
	} XXX_OBJEKT, *XXX_OBJEKT_PTR;

// PROTOTYPEN 

BOOL xxxBMP_in_Surface(const char* BMPDatei,				// dateiname
					   LPDIRECTDRAWSURFACE7 lpDDSurf,		// zielsurface
					   BOOL Palette_laden);					// für prim. surf.

BOOL xxxADVS_erstellen(XXX_ADVSPRITE_PTR pAdvs,				// sprite struktur
					   int x, int y,						// startposition
					   int Breite,int Hoehe,				// abmessungen
					   int Max_Bilder);						// max. An. Bilder

BOOL xxxADVS_Bilder_laden(XXX_ADVSPRITE_PTR pAdvs,			// speichert bilder
						  LPDIRECTDRAWSURFACE7 lpDDS,		// liefert BMP daten
						  int Anzahl,						// zu ladene bilder
						  int Anim_Reihe);					// y offset im BMP

BOOL xxxADVS_malen(XXX_ADVSPRITE_PTR pAdvs,					// welches sprite
				   LPDIRECTDRAWSURFACE7 lpDDS);				// wohin malen

HANDLE xxxBMP_oeffne_Datei(const char* Datei_name);



BOOL xxxMale_Pixel(int x,int y,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf);
BOOL xxxMale_Text(int x,int y,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf,const char *Text);
BOOL xxxMale_Rechteck(int xLinks,int yOben,int xRechts,int yUnten,UCHAR Farbe,
					  LPDIRECTDRAWSURFACE7 lpDDSurf,int solide);
BOOL xxxMale_Linie(int sx,int sy,int ex,int ey,UCHAR Farbe,LPDIRECTDRAWSURFACE7 lpDDSurf);


BOOL xxxDInput_initialisieren(HINSTANCE hinst,BOOL Mauspuffer);
BOOL xxxDInput_Keyboard_Update(void);
BOOL xxxDInput_Maus_Update(void);
BOOL CALLBACK xxxEnum_JoyDev(LPDIDEVICEINSTANCE lpddi,LPVOID pv);
BOOL xxxJoystick_erzeugen(void);
BOOL xxxDInput_Joystick_Update(void);
BOOL xxxDInput_Update(void);
void xxxDInput_beenden(void);
void xxxJoystick_Test(void);

XXX_RESULT xxx_List_Create(XXXList **pp_list);
XXX_RESULT xxx_List_Insert(XXXList **pp_list,void *p_data);
XXX_RESULT xxx_List_Delete(XXXList **pp_list,int index);
int		   xxx_List_Length(XXXList *p_list);
void	   xxx_List_Show(XXXList *p_list);

BOOL Spiel_Initialisieren();
BOOL Spiel_Beenden(void);
BOOL xxxDDraw_initialisieren(void);
BOOL xxxDDraw_Puffer_wechseln(void);
BOOL xxxDDraw_Puffer_leeren(LPDIRECTDRAWSURFACE7 lpDDSurf,UCHAR Farbe);
BOOL xxxDDraw_beenden(void);
BOOL xxxDDraw_Clipper_erzeugen(LPRECT lpRect,int Anz_Rect);


BOOL xxxObjektliste_erstellen(LPDIRECTDRAWSURFACE7 lpDDSurf);
BOOL xxxObjekt_Laserschuss_erzeugen(int x,int y,int zx,int zy,int Farbe);
BOOL xxxObjektliste_malen(LPDIRECTDRAWSURFACE7 lpDDSurf);
BOOL xxxObjektliste_Laser_malen(LPDIRECTDRAWSURFACE7 lpDDSurf);
BOOL xxxObjektliste_sortieren(void);
int  xxxObjektvergleich(const XXX_OBJEKT *arg1,const XXX_OBJEKT *arg2);
BOOL xxxObjekte_bewegen(void);
BOOL xxxObjekte_Laserschuss_bewegen(void);
BOOL xxxObjekt_KI_Turm(XXX_OBJEKT_PTR pObj);
BOOL xxxObjekt_KI_Buggy(XXX_OBJEKT_PTR pObj);

BOOL xxxObjekt_Richtung(XXX_OBJEKT_PTR pObj);

BOOL xxxObjekt_Drehung(XXX_OBJEKT_PTR pObj);			
BOOL xxxObjekt_Kollisionstest(void);
BOOL xxxObjekt_Laserkollisionstest(XXX_OBJEKT_PTR pObj);

BOOL xxxSFeld_generieren(LPDIRECTDRAWSURFACE7 lpDDS);
BOOL xxxSFeld_malen(LPDIRECTDRAWSURFACE7 lpDDS);
