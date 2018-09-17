// Datei: xxxd3dinit.h
// Basierend auf D3DEnum von Microsoft aus dem DirectX SDK Framework

#include<d3d.h>

// DEFINITIONEN

#define D3DENUM_SOFTWAREONLY			0x00000001	// Software-devs only
#define D3DENUMERR_NODIRECTDRAW			0x81000001	// create DDraw failed
#define	D3DENUMERR_ENUMERATIONFAILED	0x81000002	// Enumeration failed
#define D3DENUMERR_SUGGESTREFRAST		0x81000003	// Suggest the RefRast
#define D3DENUMERR_NOCOMPATIBLEDEVICES	0x81000004	// No Devs found that meet
													// apps desired capablities

// PROTOTYPEN

struct D3DDEVICEINFO
{
	// D3D Device Info
	CHAR			strDesc[40];
	GUID*			pDeviceGUID;
	D3DDEVICEDESC7	ddDeviceDesc;
	BOOL			bHardware;
	// DDraw Driver Info
	GUID*			pDriverGUID;
	DDCAPS			ddDriverCaps;
	DDCAPS			ddHELCaps;
	// DDraw Mode Info
	DDSURFACEDESC2	ddsdMode;
	BOOL			bFullScreen;
	// For internal use (apps should not need these)
	GUID			guidDevice;
	GUID			guidDriver;
	DDSURFACEDESC2	ddsdModes[100];
	DWORD			dwNumModes;
	DWORD			dwCurrentMode;
	BOOL			bDesktopCompatible;
};

BOOL			xxxD3D_initialisieren(void);
BOOL			xxxD3D_DDraw_initialisieren(GUID* pDriverGUID);
BOOL			xxxD3D_Direct3D_initialisieren(GUID* pDeviceGUID);
HRESULT	WINAPI	xxxD3DEnum_ZFormate_Callback(DDPIXELFORMAT* pddpf,VOID *pddpfDesired);
HRESULT			xxxD3DEnum_Devices(HRESULT(*fktOkay)(DDCAPS*,D3DDEVICEDESC7*));
BOOL	WINAPI	xxxD3DEnum_DDraw_Driver_Callback(GUID *pGUID,LPSTR strDesc,LPSTR strName,VOID*,HMONITOR);
HRESULT	WINAPI	xxxD3DEnum_Displaymodes_Callback(DDSURFACEDESC2* pddsd,VOID* pParentInfo);
HRESULT	WINAPI	xxxD3DEnum_D3D_Device_Callback(LPSTR strDesc,LPSTR strName,D3DDEVICEDESC7* pDesc,VOID* pParentInfo);
HRESULT			xxxD3DEnum_Device_waehlen(D3DDEVICEINFO** ppDevice,DWORD dwFlags);
HRESULT			xxxD3DEnum_Device_Okay(DDCAPS* pddDriverCaps,D3DDEVICEDESC7* pd3dDeviceDesc);
BOOL			xxxD3D_Ausgabe_Devices(LPDIRECTDRAWSURFACE7 lpDDS);
void			xxxD3D_beenden(void);