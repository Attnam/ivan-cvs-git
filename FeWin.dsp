# Microsoft Developer Studio Project File - Name="FeWin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FeWin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FeWin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FeWin.mak" CFG="FeWin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FeWin - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FeWin - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "FeWin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FeWin/Release"
# PROP Intermediate_Dir "FeWin/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "Include" /I "Main/Include" /I "Main/Resource" /I "FeDX/Include" /I "FEEL/Include" /I "FeFile/Include" /I "FeIO/Include" /I "FELL/Include" /I "FeWin/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FeWin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FeWin/Debug"
# PROP Intermediate_Dir "FeWin/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /Z7 /Od /Ob1 /I "Include" /I "Main/Include" /I "Main/Resource" /I "FeDX/Include" /I "FEEL/Include" /I "FeFile/Include" /I "FeIO/Include" /I "FELL/Include" /I "FeWin/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "FeWin - Win32 Release"
# Name "FeWin - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\FeWin\Source\whandler.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# Begin Source File

SOURCE=.\FeWin\Include\whandler.h
# End Source File
# End Group
# End Target
# End Project
