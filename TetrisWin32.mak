# Microsoft Developer Studio Generated NMAKE File, Based on TetrisWin32.dsp
!IF "$(CFG)" == ""
CFG=TetrisWin32 - Win32 Debug
!MESSAGE Keine Konfiguration angegeben. TetrisWin32 - Win32 Debug wird als Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "TetrisWin32 - Win32 Release" && "$(CFG)" != "TetrisWin32 - Win32 Debug"
!MESSAGE UngÅltige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie kînnen beim AusfÅhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "TetrisWin32.mak" CFG="TetrisWin32 - Win32 Debug"
!MESSAGE 
!MESSAGE FÅr die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "TetrisWin32 - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "TetrisWin32 - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 
!ERROR Eine ungÅltige Konfiguration wurde angegeben.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TetrisWin32 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TetrisWin32.exe"


CLEAN :
	-@erase "$(INTDIR)\tetris.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XXX2DGrafik.obj"
	-@erase "$(INTDIR)\XXXBitmap.obj"
	-@erase "$(INTDIR)\XXXInput.obj"
	-@erase "$(INTDIR)\XXXList.obj"
	-@erase "$(OUTDIR)\TetrisWin32.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\TetrisWin32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TetrisWin32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TetrisWin32.pdb" /machine:I386 /out:"$(OUTDIR)\TetrisWin32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\tetris.obj" \
	"$(INTDIR)\XXX2DGrafik.obj" \
	"$(INTDIR)\XXXBitmap.obj" \
	"$(INTDIR)\XXXInput.obj" \
	"$(INTDIR)\XXXList.obj"

"$(OUTDIR)\TetrisWin32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TetrisWin32 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TetrisWin32.exe"


CLEAN :
	-@erase "$(INTDIR)\tetris.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XXX2DGrafik.obj"
	-@erase "$(INTDIR)\XXXList.obj"
	-@erase "$(OUTDIR)\TetrisWin32.exe"
	-@erase "$(OUTDIR)\TetrisWin32.ilk"
	-@erase "$(OUTDIR)\TetrisWin32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\TetrisWin32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TetrisWin32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=dxguid.lib ddraw.lib dinput.lib winmm.lib d3dim.lib dsound.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TetrisWin32.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TetrisWin32.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\tetris.obj" \
	"$(INTDIR)\XXX2DGrafik.obj" \
	"$(INTDIR)\XXXList.obj"

"$(OUTDIR)\TetrisWin32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TetrisWin32.dep")
!INCLUDE "TetrisWin32.dep"
!ELSE 
!MESSAGE Warning: cannot find "TetrisWin32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TetrisWin32 - Win32 Release" || "$(CFG)" == "TetrisWin32 - Win32 Debug"
SOURCE=.\tetris.cpp

"$(INTDIR)\tetris.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\XXX2DGrafik.cpp

"$(INTDIR)\XXX2DGrafik.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\XXXBitmap.cpp

!IF  "$(CFG)" == "TetrisWin32 - Win32 Release"


"$(INTDIR)\XXXBitmap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TetrisWin32 - Win32 Debug"

!ENDIF 

SOURCE=.\XXXInput.cpp

!IF  "$(CFG)" == "TetrisWin32 - Win32 Release"


"$(INTDIR)\XXXInput.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TetrisWin32 - Win32 Debug"

!ENDIF 

SOURCE=.\XXXList.cpp

"$(INTDIR)\XXXList.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\XXXMain.cpp
SOURCE=.\XXXObjekt.cpp
SOURCE=.\XXXSound.cpp
SOURCE=.\XXXSpielfeld.cpp

!ENDIF 

