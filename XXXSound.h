// Datei : xxxsound.h

#include<dsound.h>

// DEFINITIONEN

#define SND_SCHUSS1			0
#define SND_SCHUSS2			1
//----------------------------
#define ANZ_SOUNDS_GESAMT	2

// PROTOTYPEN

BOOL xxxDSound_initialisieren(HWND hwnd);//,HINSTANCE hInst);
void xxxDSound_beenden(void);
BOOL xxxDSound_Buffer_erstellen(const char* fileName,LPDIRECTSOUNDBUFFER *lpDSB);
BOOL xxxDSound_Wave_laden(const char* fileName,WAVEFORMATEX* Wf,DWORD* pdwLaenge,PBYTE* ppbyWDat);
BOOL xxxDSound_Wave_Format(HMMIO hmmio,WAVEFORMATEX* Wf);
BOOL xxxDSound_Wave_Daten(HMMIO hmmio,DWORD* pdwLaenge,PBYTE* ppbyWDat);
void xxxDSound_spielen(LPDIRECTSOUNDBUFFER lpDSB,int pan,int vol);
void xxxDSound_Schleife(LPDIRECTSOUNDBUFFER lpDSB,int pan,int vol,BOOL play);