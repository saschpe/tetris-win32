// Datei : xxxinput.cpp

/*
 *	Disclaimer: DirectInput Initialisierung und Steuerungsbibliothek
 */

// INCLUDES
#include<dinput.h>			// DirectInput Bibliothek
#include"XXXGlobal.h"		// globale Definitionen & Strukturen


// GLOBALS
extern HWND hwnd;			// Handle des Hauptfensters

// DirectInput Objekte
LPDIRECTINPUT7			lpDI;			// DirectInput Hauptinterface
LPDIRECTINPUTDEVICE7	lpDIKeyboard;	// Keyboard Device Interface
LPDIRECTINPUTDEVICE7	lpDIMaus;		// Maus		Device Interface
LPDIRECTINPUTDEVICE7	lpDIJoystick;	// Joystick Device Interface
GUID					guidJoy_Dev;	// GUID des Joystick Device

// Variablen für den Status der Eingabegeräte
BOOL bGepufferte_Maus;					// Mauseigabe gepuffert ?
int	Maus_x = 0;							// Maus Koordinaten
int Maus_y = 0;							// Maus Koordinaten
BOOL bMaus_Buttons[MAX_MAUS_BUTTONS];	// Mausknopf gedrückt ? 
BOOL bJoystick = FALSE;					// Joystick vorhanden ?
int Joystick_x = 0;						// Joystick Koordinaten
int Joystick_y = 0;						// Joystick Koordinaten
BOOL bJoystick_Buttons[MAX_JOYSTICK_BUTTONS];	// Feuerknöpfe gedrückt ?
char strJoy_Name[50];					// Joystick Produktname
char Tasten_Puffer[256];				// Keyboard Status

// hat Eingabe-Aktion stattgefunden (egal ob MS,Keybrd od Joy)?
BOOL Eingabe[AK_MAXIMAL];

// FUNKTIONEN

// Initialisierung des DirectInput Interfaces sowie
// des Keyboard-, Maus- und Joystick-Gerätes
BOOL xxxDInput_initialisieren(HINSTANCE hinst,BOOL Mauspuffer)
{
	HRESULT		dirval;

	// Zuerst: DirectInput Haupt-Objekt erstellen
	dirval = DirectInputCreateEx(hinst,DIRECTINPUT_VERSION,IID_IDirectInput7,
								(LPVOID*)&lpDI,NULL);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler: DInputCreateEx()","XXX",MB_OK);
		return FALSE;
	}

	// HIER KOMMT DAS KEYBOARD
	// 1.Schritt: Keyboard Objekt erstellen
	dirval = lpDI->CreateDeviceEx(GUID_SysKeyboard,IID_IDirectInputDevice7,
								 (LPVOID*)&lpDIKeyboard,NULL);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler KB: CreateDeviceEx()","XXX",MB_OK);
		return FALSE;
	}
	// 2.Schritt: Datenformat setzen
	dirval = lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler KB: SetDataFormat()","XXX",MB_OK);
		return FALSE;
	}
	// 3.Schritt: Zusammenarbeit mit Windows regeln
	dirval = lpDIKeyboard->SetCooperativeLevel(hwnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler KB: SetCooperativeLevel()","XXX",MB_OK);
		return FALSE;
	}
	// 4.Schritt: Keyboard Objekt aktivieren
	if(lpDIKeyboard)
	{
		dirval = lpDIKeyboard->Acquire();
		if(dirval != DI_OK)
		{
			MessageBox(hwnd,"DInput Fehler KB: Acquire()","XXX",MB_OK);
			return FALSE;
		}
	}
	else return FALSE;

	// HIER KOMMT DIE MAUS

	if(Mauspuffer == TRUE)
		bGepufferte_Maus = TRUE;
	else
		bGepufferte_Maus = FALSE;

	// 1.Schritt: Maus Objekt erstellen
	dirval = lpDI->CreateDeviceEx(GUID_SysMouse,IID_IDirectInputDevice7,
								 (LPVOID*)&lpDIMaus,NULL);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler M: CreateDeviceEx()","XXX",MB_OK);
		return FALSE;
	}
	// 2.Schritt: Datenformat
	dirval = lpDIMaus->SetDataFormat(&c_dfDIMouse);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler M: SetDataFormat()","XXX",MB_OK);
		return FALSE;
	}
	// 3.Schritt: Zusammenarbeit mit Windows regeln
	dirval = lpDIMaus->SetCooperativeLevel(hwnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler M: SetCooperativeLevel()","XXX",MB_OK);
		return FALSE;
	}
	// 4.Schritt: Puffer für Maus Aktivitäten erzeugen
	DIPROPDWORD		dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DINPUT_BUFFERSIZE;

	dirval = lpDIMaus->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler M: SetProperty()","XXX",MB_OK);
		return FALSE;
	}
	// 5.Schritt: Maus Objekt aktivieren
	if(lpDIMaus)
	{
		dirval = lpDIMaus->Acquire();
		if(dirval != DI_OK)
		{
			MessageBox(hwnd,"DInput Fehler M: Acquire()","XXX",MB_OK);
			return FALSE;
		}
	}
	else return FALSE;

	// HIER KOMMT DER JOYSTICK
	// 1.Schritt: Ersten angeschlossenen Joystick enumerieren
	dirval = lpDI->EnumDevices(DIDEVTYPE_JOYSTICK,(LPDIENUMDEVICESCALLBACK )xxxEnum_JoyDev,
							  &guidJoy_Dev,DIEDFL_ATTACHEDONLY);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: EnumDevices()","XXX",MB_OK);
		return FALSE;
	}
	// 2.-6.Schritt: erzeugen in eigener Funktionen
	if(bJoystick)		// nur wenn Joystick vorhanden
	{
		if(!xxxJoystick_erzeugen())
		{
			MessageBox(hwnd,"DInput Fehler J: xxxJoystick_erzeugen()","XXX",MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}// x

// Fragt den Status des Keyboardsa ab und speichert die Zustände
// der einzelnen Tsten in dem globalen Array 'Tasten_Puffer[]'
BOOL xxxDInput_Keyboard_Update(void)
{
	HRESULT		dirval;			// Rückgabewert

	// Status der Tastatur abfragen und speichern
	dirval = lpDIKeyboard->GetDeviceState(sizeof(Tasten_Puffer),(LPVOID)&Tasten_Puffer);

	if(dirval != DI_OK)
	{
		if(dirval == DIERR_INPUTLOST)
		{
			if(lpDIKeyboard)
			{
				lpDIKeyboard->Acquire();
				dirval = lpDIKeyboard->GetDeviceState(sizeof(Tasten_Puffer),
													  (LPVOID)&Tasten_Puffer);
				if(dirval != DI_OK)
				{
					MessageBox(hwnd,"DInput Fehler KB: GetDeviceState()","XXX",MB_OK);
					return FALSE;
				}
			}
		}
	else
	{
		MessageBox(hwnd,"DInput Fehler KB: InvalidParam()","XXX",MB_OK);
		return FALSE;
	}
	}
	return TRUE;
}

// Fragt den Status der Maus ab und speichert die bewegung und 
// den Status der Tasten in den globalen Variablen Maus_x,Maus_y und bMaus_Buttons[4]
BOOL xxxDInput_Maus_Update(void)
{
	HRESULT			dirval;
	BOOL			bFertig = FALSE;
	DIMOUSESTATE	ms;

	// Gepuffert: Langsam, bearbeited aber alle Maus-Aktionen
	if(bGepufferte_Maus == TRUE)
	{
		while(!bFertig)
		{
			DIDEVICEOBJECTDATA		od;		// Speichert Mauszustand
			DWORD		dwElemente = 1;		// Elemente die abgefragt werden sollen
		
			dirval = lpDIMaus->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),&od,&dwElemente,0);

			if(dirval == DIERR_INPUTLOST)
			{
				lpDIMaus->Acquire();
				if(dirval != DI_OK)
				{
					MessageBox(hwnd,"DInput Fehler M: reacquire()","XXX",MB_OK);
					return FALSE;
				}
				else
				{
					dirval = lpDIMaus->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),&od,&dwElemente,0);
					if(dirval == DIERR_INPUTLOST)
					{
						MessageBox(hwnd,"DInput Fehler M: GetData2()","XXX",MB_OK);
						return FALSE;
					}
				}
			}
			else if(dirval != DI_OK)
			{
				bFertig = TRUE;
				return FALSE;
			}

			// keine Elemente wurden verändert
			if(dwElemente == 0)
				bFertig = TRUE;
	
			// Feld 'dwOfs' enthällt Maus-Aktion:
			switch(od.dwOfs)
			{
			case DIMOFS_X:				// horizontale Bewegung
				Maus_x += od.dwData;
				bFertig = TRUE;
				break;
			case DIMOFS_Y:				// vertikale Bewegung
				Maus_y += od.dwData;
				bFertig = TRUE;
				break;
			case DIMOFS_BUTTON0:		
				if(od.dwData & 0x80)	// Knopf gedrückt
				{
					bMaus_Buttons[0] = TRUE;
					bFertig = TRUE;
				}
				else					// Knopf losgelassen
				{
					bMaus_Buttons[0] = FALSE;
					bFertig = TRUE;
				}
				break;
			case DIMOFS_BUTTON1:
				if(od.dwData & 0x80)	// Knopf gedrückt
				{
					bMaus_Buttons[1] = TRUE;
					bFertig = TRUE;
				}
				else					// Knopf losgelassen
				{
					bMaus_Buttons[1] = FALSE;
					bFertig = TRUE;
				}
				break;
			}
		}
	}

	// Ungepuffert: Schnell, verpasst aber ab und zu Maus-Aktionen
	else
	{
		dirval = lpDIMaus->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&ms);

		if(dirval == DIERR_INPUTLOST)
		{
			lpDIMaus->Acquire();
			if(dirval != DI_OK)
			{
				MessageBox(hwnd,"DInput Fehler M: ReAcquire()","XXX",MB_OK);
				return FALSE;
			}
			else
			{
				dirval = lpDIMaus->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&ms);
				if(dirval == DIERR_INPUTLOST)
				{
					MessageBox(hwnd,"DInput Fehler M: GetDeviceState2()","XXX",MB_OK);
					return FALSE;
				}
			}
		}
		else if(dirval != DI_OK)
		{
			return FALSE;
		}// [anderer Fehler]

		Maus_x += ms.lX;
		Maus_y += ms.lY;

		if(ms.rgbButtons[0] & 0x80)
			bMaus_Buttons[0] = TRUE;
		else
			bMaus_Buttons[0] = FALSE;
		if(ms.rgbButtons[1] & 0x80)
			bMaus_Buttons[1] = TRUE;
		else
			bMaus_Buttons[1] = FALSE;
	}
	// die Maus nicht verschwinden lassen
	if(Maus_x <0 ) Maus_x = 0;
	if(Maus_x > SCREEN_BREITE) Maus_x = SCREEN_BREITE;
	if(Maus_y <0 ) Maus_y = 0;
	if(Maus_y > SCREEN_HOEHE)  Maus_y = SCREEN_HOEHE;
	return TRUE;
}//x

// Diese Funktion sucht nach dem 1. angeschlossenen Joystick
// und speichert dessen GUID in der globalen Variable
BOOL CALLBACK xxxEnum_JoyDev(LPDIDEVICEINSTANCE lpddi,LPVOID pv)
{
	GUID *pguidDevice = NULL;
	// Speichert Instance-GUID des gefundenen Device(Joy)
	// in 'pguidDevice' = 'pv' = 'pguidDevice'
	if(pv)
	{
		// lokaler Pointer zeigt auf gleiche Adresse wie globaler
		pguidDevice = (GUID*)pv;
		// Wert des Pointers wird gefundene GUID
		*pguidDevice = lpddi->guidInstance;
	}

	strcpy(strJoy_Name,(char*)lpddi->tszProductName);
	bJoystick = TRUE;			// wir haben einen!

	// Halte die EnumDevice() Funktion an
	return DIENUM_STOP;
}//x

// Hier wird das eigentliche Joystick Device erzeugt und aktiviert
BOOL xxxJoystick_erzeugen(void)
{
	HRESULT			dirval;			// rückgabwert (:-))
	DIPROPRANGE		diprg;			// joystick eigenschaften

	// 2.Schritt: Joystick für enumerierte GUID erstellen
	dirval = lpDI->CreateDeviceEx(guidJoy_Dev,IID_IDirectInputDevice7,
								 (LPVOID*)&lpDIJoystick,NULL);
	if(dirval !=DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: CreateDeviceEx()","XXX",MB_OK);
		return FALSE;
	}
	
	// 3.Schritt: Datenformat für joystick festlegen
	dirval = lpDIJoystick->SetDataFormat(&c_dfDIJoystick);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: SetDataDormat()","XXX",MB_OK);
		return FALSE;
	}

	// 4.Schritt: Zusammenarbeit mit windows reglen
	dirval = lpDIJoystick->SetCooperativeLevel(hwnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: SetCooperativeLevel)","XXX",MB_OK);
		return FALSE;
	}

	// 5.Schritt: Joystick objekt eigenschaften festlegen
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = DIJOFS_X;			// x-Achse
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;		// wertebereich
	diprg.lMax =  1000;		// von , bis

	dirval = lpDIJoystick->SetProperty(DIPROP_RANGE,&diprg.diph);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: SetProperty() X","XXX",MB_OK);
		return FALSE;
	}

	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = DIJOFS_Y;			// y-Achse
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;		// wertebereich
	diprg.lMax =  1000;		// von , bis

	dirval = lpDIJoystick->SetProperty(DIPROP_RANGE,&diprg.diph);
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: SetProperty() Y","XXX",MB_OK);
		return FALSE;
	}

	// 6.Schritt: joystick object aktivieren
	if(lpDIJoystick)
	{
		dirval = lpDIJoystick->Acquire();
		if(dirval !=DI_OK)
		{
			MessageBox(hwnd,"DInput Fehler J: Acquire()","XXX",MB_OK);
			return FALSE;
		}
	}
	else return FALSE;
	return TRUE;
}//x

// Fragt den status des joysticks ab und speichert die bewegung und status der knöpfe
// in den globalen variablen Joystick_x,Joystick_y und bJoystick_Buttons[4]
BOOL xxxDInput_Joystick_Update(void)
{
	DIJOYSTATE		js;			// struktur für die joystick daten
	HRESULT			dirval;		// rückgabewert (:-)!!!)

	// führe ein update für allen werte de joystick aus
	dirval = lpDIJoystick->Poll();
	if(dirval != DI_OK)
	{
		MessageBox(hwnd,"DInput Fehler J: Poll()","XXX",MB_OK);
		return FALSE;
	}

	// speichere alle joystick daten in struktur 'js'
	dirval = lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE),&js);
	if(dirval != DI_OK)
	{
		if(dirval == DIERR_INPUTLOST)
		{
			dirval = lpDIJoystick->Acquire();
			if(dirval != DI_OK)
			{
				MessageBox(hwnd,"DInput Fehler J: ReAcquire()","XXX",MB_OK);
				return FALSE;
			}
			else
			{
				dirval = lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE),(LPVOID)&js);
				if(dirval != DI_OK)
				{
					return FALSE;
				}
			}
		}
		else if(dirval != DI_OK)
		{
			return FALSE;
		}
	}

	// fragt alle feuerknöpfe ab uns speichere sie in unserem eigenen array ab
	for(int i=0;i<MAX_JOYSTICK_BUTTONS;i++)
	{
		if(js.rgbButtons[i] & 0x80)
			bJoystick_Buttons[i] = TRUE;
		else
			bJoystick_Buttons[i] = FALSE;
	}

	// speichere die joystick stellung der beiden achsen in variablen ab
	Joystick_x = js.lX;
	Joystick_y = js.lY;
	return TRUE;
}//x

// aufruf aller einzelnen update funktionen für keyboard, maus und joystick.
// festlegen der aktionen Eingabe[AK_*] je nach input des spielers an keyb,joy,maus
BOOL xxxDInput_Update(void)
{
	if(xxxDInput_Keyboard_Update() == FALSE || xxxDInput_Maus_Update() == FALSE)
		return FALSE;

	if(bJoystick && xxxDInput_Joystick_Update() == FALSE)
		return FALSE;
	
	if(bMaus_Buttons[0])
		Eingabe[AK_KLICK] = TRUE;
	else
		Eingabe[AK_KLICK] = FALSE;

	if(TASTE(DIK_SPACE) ||(bJoystick && bJoystick_Buttons[0]) || (bMaus_Buttons[1]))
		Eingabe[AK_SCHIESSEN] = TRUE;
	else
		Eingabe[AK_SCHIESSEN] = FALSE;

	if(TASTE(DIK_ESCAPE) || (bJoystick && bJoystick_Buttons[3]))
		Eingabe[AK_BEENDEN] = TRUE;
	else
		Eingabe[AK_BEENDEN] = FALSE;

	// Scrolling wenn Maus im Randbereich
	if((Maus_x < 50) || (TASTE(DIK_LEFTARROW)))
		Eingabe[AK_SCROLL_L] = TRUE;
	else
		Eingabe[AK_SCROLL_L] = FALSE;

	if((Maus_x > (SCREEN_BREITE-50)) || (TASTE(DIK_RIGHTARROW)))
		Eingabe[AK_SCROLL_R] = TRUE;
	else
		Eingabe[AK_SCROLL_R] = FALSE;
	
	if((Maus_y < 50) || (TASTE(DIK_UPARROW)))
		Eingabe[AK_SCROLL_U] = TRUE;
	else
		Eingabe[AK_SCROLL_U] = FALSE;

	if((Maus_y > (SCREEN_HOEHE-50)) || (TASTE(DIK_DOWNARROW)))
		Eingabe[AK_SCROLL_D] = TRUE;
	else
		Eingabe[AK_SCROLL_D] = FALSE;

	if(TASTE(DIK_RETURN) || bMaus_Buttons[0])
		Eingabe[AK_STARTEN] = TRUE;
	else
		Eingabe[AK_STARTEN] = FALSE;

	return TRUE;
}//x

// deaktivieren und freigeben aller directinput objekte
void xxxDInput_beenden(void)
{
	if(lpDI != NULL)
	{
		if(lpDIKeyboard != NULL)
		{
			lpDIKeyboard->Unacquire();
			lpDIKeyboard->Release();
			lpDIKeyboard = NULL;
		}
		if(lpDIMaus != NULL)
		{
			lpDIMaus->Unacquire();
			lpDIMaus->Release();
			lpDIMaus = NULL;
		}
		if(lpDIJoystick != NULL)
		{
			lpDIJoystick->Unacquire();
			lpDIJoystick->Release();
			lpDIJoystick = NULL;
			bJoystick = FALSE;
		}
		lpDI->Release();
		lpDI = NULL;
	}
}//x

