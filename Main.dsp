# Microsoft Developer Studio Project File - Name="Main" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Main - Win32 PowerDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Main.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Main.mak" CFG="Main - Win32 PowerDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Main - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Main - Win32 PowerDebug" (based on "Win32 (x86) Application")
!MESSAGE "Main - Win32 FastDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Main - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main/Release"
# PROP Intermediate_Dir "Main/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /D "VC" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG" /d "VC"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib FeLib/Release/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\Release\Main.exe IVAN.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Main - Win32 PowerDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Main___W"
# PROP BASE Intermediate_Dir "Main___W"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Main/PowerDebug"
# PROP Intermediate_Dir "Main/PowerDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /Zi /Od /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /I /" " /c
# ADD CPP /nologo /W3 /Gm /GR /GX /Zi /Od /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /I /" " /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /D "VC" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG" /d "VC"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Debug/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib FeLib/PowerDebug/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\PowerDebug\Main.exe IVAN.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Main - Win32 FastDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Main___0"
# PROP BASE Intermediate_Dir "Main___0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main/FastDebug"
# PROP Intermediate_Dir "Main/FastDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /Zi /O2 /Ob2 /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /D "VC" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG" /d "VC"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Release/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 dxguid.lib ddraw.lib FeLib/FastDebug/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\FastDebug\Main.exe IVAN.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Main - Win32 Release"
# Name "Main - Win32 PowerDebug"
# Name "Main - Win32 FastDebug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\Main\Source\actionba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\actionde.cpp

!IF  "$(CFG)" == "Main - Win32 Release"

!ELSEIF  "$(CFG)" == "Main - Win32 PowerDebug"

# ADD BASE CPP /W3
# ADD CPP /W3

!ELSEIF  "$(CFG)" == "Main - Win32 FastDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Main\Source\area.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\charba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\charde.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\config.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\cont.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\dungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\fluid.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\godba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\godde.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\id.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\igraph.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\itemba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\itemde.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\level.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lsquare.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lterraba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lterrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\materba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\materde.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\message.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\object.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\pool.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\proto.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\roomba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\roomde.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\script.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\slot.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\square.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\stack.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\team.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\terra.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\worldmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wskill.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wsquare.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wterraba.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wterrade.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# Begin Source File

SOURCE=.\Main\Include\actionba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\actionde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\area.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\charba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\charde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\command.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\config.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\cont.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\dungeon.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\entity.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\fluid.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\game.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\godba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\godde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\id.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\igraph.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\itemba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\itemde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\ivandef.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\level.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lsquare.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lterraba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lterrade.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\main.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\materba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\materde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\message.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\object.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\pool.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\proto.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\roomba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\roomde.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\script.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\slot.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\square.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\stack.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\team.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\terra.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\worldmap.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wskill.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wsquare.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wterraba.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wterrade.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main\Resource\Logo.ico
# End Source File
# Begin Source File

SOURCE=.\Main\Resource\main.rc

!IF  "$(CFG)" == "Main - Win32 Release"

!ELSEIF  "$(CFG)" == "Main - Win32 PowerDebug"

!ELSEIF  "$(CFG)" == "Main - Win32 FastDebug"

!ENDIF 

# End Source File
# End Target
# End Project
