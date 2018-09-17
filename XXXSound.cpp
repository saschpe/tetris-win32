// Datei : xxxsound.cpp

// INCLUDES

#include<stdio.h>				// für sprintf
#include<windows.h>				// mmio() Funktionen
#include<mmsystem.h>			// mmio() Funktionen
#include"XXXGlobal.h"			// globale Definitionen & Strukturen
#include"XXXSound.h"			// Header dieser Datei

// GLOBALS

LPDIRECTSOUND		lpDS;
LPDIRECTSOUNDBUFFER lpDSB[ANZ_SOUNDS_GESAMT];

extern HWND			hwnd;		// HWND des Hauptfensters

// FUNCTIONS

/*
 *	DirectSound initialisieren	
 */	
BOOL xxxDSound_initialisieren(HWND hwnd)
{
	HRESULT		dsrval;
	char		buffer[12];

	// 1. Schritt: DirectSound Interface erstellen
	dsrval = DirectSoundCreate(NULL,&lpDS,NULL);
	if(FAILED(dsrval))
	{
		MessageBox(hwnd,"DSound Fehler: DirectSoundCreate","XXX",MB_OK);
		xxxDSound_beenden();
		return FALSE;
	}

	// 2. Schritt: Coop-Level festlegen
	dsrval = lpDS->SetCooperativeLevel(hwnd,DDSCL_NORMAL);
	if(FAILED(dsrval))
	{
		MessageBox(hwnd,"DSound Fehler: SetCooperativeLevel()","XXX",MB_OK);
		xxxDSound_beenden();
		return FALSE;
	}

	// 3. Schritt: Secondary Sound Buffer erstellen
	for(int i=0;i<ANZ_SOUNDS_GESAMT;i++)
	{
		sprintf(buffer,"sound%d.wav",i);
		xxxDSound_Buffer_erstellen(buffer,&lpDSB[i]);
		if(lpDSB[i] == NULL)
			return FALSE;
	}
	
	return TRUE;
}//x

/*
 *	Deaktivieren und Freigeben aller DirectSound Objekte
 */
void xxxDSound_beenden()
{
	if(lpDS != NULL)
		lpDS->Release();

	for(int i=0;i<ANZ_SOUNDS_GESAMT;i++)
	{
		if(lpDSB[i] != NULL)
			lpDSB[i]->Release();
	}
}//x

/*
 *	Erzeugen eines Secondary SoundBuffer und laden
 *	der angegebenen Wave Datei in diesen SoundBuffer
 */
BOOL xxxDSound_Buffer_erstellen(const char* pfileName,LPDIRECTSOUNDBUFFER* lpDSB)
{
	DSBUFFERDESC	dsbd;				// SB Description
	BYTE		   *pDSBuffData;		// SB Daten Adresse
	WAVEFORMATEX	waveFormat;			// Wave Format
	DWORD			dwDataLength;		// Laenge der Wave Daten
	PBYTE			pbyWaveDaten;		// Eigentliche Wave Daten
	HRESULT			dsrval;				// Rückgabewert

	pbyWaveDaten = NULL;

	// 1.Schritt: Waveformat,Wavelänge und Wavedaten laden
	if(!xxxDSound_Wave_laden(pfileName,&waveFormat,&dwDataLength,&pbyWaveDaten))
	{
		MessageBox(hwnd,"DSound Fehler: xxxDSound_Wave_laden()","XXX",MB_OK);
		return FALSE;
	}

	// 2.Schritt: Datenstruktur initialisieren
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize			= sizeof(DSBUFFERDESC);
	dsbd.dwFlags		= DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | 
						  DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC;
	dsbd.dwBufferBytes	= dwDataLength;
	dsbd.lpwfxFormat	= &waveFormat;

	// 3.Schritt: SoundBuffer Objekt erstellen
	dsrval = lpDS->CreateSoundBuffer(&dsbd,&(*lpDSB),NULL);
	if(FAILED(dsrval))
	{
		MessageBox(hwnd,"DSound Fehler: CreateSoundBuffer()","XXX",MB_OK);
		return FALSE;
	}

	// 4.Schritt: SoundBuffer verriegeln um Daten zu speichern
	dsrval = (*lpDSB)->Lock(0,dwDataLength,(LPVOID *)&pDSBuffData,&dwDataLength,NULL,0,0);
	if(FAILED(dsrval))
	{
		MessageBox(hwnd,"DSound Fehler: Lock()","XXX",MB_OK);
		return FALSE;
	}

	// 5.Schritt: Wavedaten in SoundBuffer-Speicher kopieren
	memcpy(pDSBuffData,pbyWaveDaten,dwDataLength);
	free(pbyWaveDaten);

	// 6.Schritt: SoundBuffer entriegeln
	dsrval = (*lpDSB)->Unlock(pDSBuffData,dwDataLength,NULL,0);
	if(FAILED(dsrval))
	{
		MessageBox(hwnd,"DSound Fehler: UnLock()","XXX",MB_OK);
		return FALSE;
	}
	return TRUE;
}//x

/*
 *	Füllen der angegebenen Parameter mit Daten aus
 *	der zu ladenden Wave Datei
 */
BOOL xxxDSound_Wave_laden(const char* fileName,		// zu öffnende Datei
						  WAVEFORMATEX* pWf,		// für den Header
						  DWORD* pdwLaenge,			// für die Wave-Laenge
						  PBYTE* ppbyWDat)			// für die Wave Daten
{
	HMMIO	hmmio;

	// 1.Schritt: Datei öffnen
	hmmio = mmioOpen((char*)((LPCSTR)fileName),NULL,MMIO_READ);
	if(!hmmio)
	{
		MessageBox(hwnd,"DSound Fehler: mmioOpen()","XXX",MB_OK);
		return FALSE;
	}

	// 2.Schritt: Daten für WAVEFORMATEX Struktur aus Datei lesen
	if(!xxxDSound_Wave_Format(hmmio,pWf))
	{
		MessageBox(hwnd,"DSound Fehler: xxxDSound_Wave_Format()","XXX",MB_OK);
		return FALSE;
	}

	// 3.Schritt: Wave-Datenlänge und Wave Daten aus Datei lesen
	if(!xxxDSound_Wave_Daten(hmmio,pdwLaenge,ppbyWDat))
	{
		MessageBox(hwnd,"DSound Fehler: xxxDSound_Wave_Daten()","XXX",MB_OK);
		return FALSE;
	}

	// 4.Schritt: Datei schliessen
	mmioClose(hmmio,0);
	return TRUE;
}//x

/*
 *	Füllen der angegebenen Struktur mit den Formatdaten
 *	(Header) der zu ladenen Wave Datei
 */
BOOL xxxDSound_Wave_Format(HMMIO hmmio,WAVEFORMATEX *pWf)
{
	MMCKINFO	mmWave;
	MMCKINFO	mmFmt;
	MMRESULT	mmResult;

	ZeroMemory(&mmWave,sizeof(mmWave));
	ZeroMemory(&mmFmt,sizeof(mmFmt));

	// Pointer an den Anfang des Wave Files setzen
	mmResult = mmioSeek(hmmio,0,SEEK_SET);

	mmWave.fccType = mmioStringToFOURCC("WAVE",0);
	mmFmt.ckid	   = mmioStringToFOURCC("fmt",0);

	// Finde den Anfang des Hauptchunks
	mmResult = mmioDescend(hmmio,&mmWave,NULL,MMIO_FINDRIFF);
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioDescend()","XXX",MB_OK);
		return FALSE;
	}

	// Finde den Anfang des FMT-Sub Chunks im Hauptchunk
	// und setze dort Pointer-Position
	mmResult = mmioDescend(hmmio,&mmFmt,&mmWave,MMIO_FINDCHUNK);
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioDescend() CHUNK","XXX",MB_OK);
		return FALSE;
	}

	// Lies von Pointer-Position in Param1 die Daten in Param2
	// über eine Länge von Param3.
	mmResult = mmioRead(hmmio,(HPSTR)pWf,sizeof(WAVEFORMATEX));
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioRead() WAVEFORMATEX","XXX",MB_OK);
		return FALSE;
	}
	return TRUE;
}//x

/*
 *	Auslesen der Sounddaten-Länge und der eigentlichen 
 *	Sounddaten der zu ladenden Wave Datei
 */
BOOL xxxDSound_Wave_Daten(HMMIO hmmio,DWORD *pdwLaenge,PBYTE* ppbyWDat)
{
	MMCKINFO	mmWave;
	MMCKINFO	mmData;
	MMRESULT	mmResult;

	ZeroMemory(&mmWave,sizeof(mmWave));
	ZeroMemory(&mmData,sizeof(mmData));

	// Pointer an den Anfang des Wave Files setzen
	mmResult = mmioSeek(hmmio,0,SEEK_SET);

	// Finde den Anfang des WAVE-Chunks
	// und setze doort Pointer-Position
	mmWave.fccType	= mmioStringToFOURCC("WAVE",0);
	mmData.ckid		= mmioStringToFOURCC("data",0);

	mmResult = mmioDescend(hmmio,&mmWave,NULL,MMIO_FINDRIFF);
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioDescend() RIFF","XXX",MB_OK);
		return FALSE;
	}

	// Finde den Anfang des Data-SubChunks im WAVE-Chunk
	// und setze dort Pointer-Position
	mmResult = mmioDescend(hmmio,&mmData,&mmWave,MMIO_FINDCHUNK);
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioDescend() CHUNK","XXX",MB_OK);
		return FALSE;
	}

	// Länge der Wave Daten speichern
	*pdwLaenge = mmData.cksize;

	*ppbyWDat = (PBYTE)malloc(*pdwLaenge);
	if(!*ppbyWDat)
	{
		MessageBox(hwnd,"DSound Fehler: malloc()","XXX",MB_OK);
		return FALSE;
	}

	ZeroMemory(*ppbyWDat,*pdwLaenge);

	// Wave Daten in die Variabel lesen
	mmResult = mmioRead(hmmio,(HPSTR)*ppbyWDat,*pdwLaenge);
	if(FAILED(mmResult))
	{
		MessageBox(hwnd,"DSound Fehler: mmioRead() DATA","XXX",MB_OK);
		return FALSE;
	}
	
	return TRUE;			
}//x

/*
 *	Einfaches Abspielen eines SoundBuffers mit rechts/links
 *	bzw. in gesamter Laenge.
 */
void xxxDSound_spielen(LPDIRECTSOUNDBUFFER lpDSB,int pan,int vol)
{
	if(lpDSB)
	{
		lpDSB->SetPan(pan);
		lpDSB->SetVolume(vol);
		lpDSB->Play(0,0,0);
	}
}//x

/*
 *	Abspielen eines SoundBuffers in einer Schleife die bei TRUE
 *	als Parameter startet und bei FALSE stoppt.
 */
void xxxDSound_Schleife(LPDIRECTSOUNDBUFFER lpDSB,int pan,int vol,BOOL play)
{
	DWORD	ret;
	lpDSB->GetStatus(&ret);

	if((lpDSB)&&(play))
	{
		if(ret != DSBSTATUS_LOOPING)
		{
			lpDSB->SetPan(pan);
			lpDSB->SetVolume(vol);
			lpDSB->Play(0,0,DSBPLAY_LOOPING);
		}
	}
	else if(lpDSB)
		lpDSB->Stop();
}//x
