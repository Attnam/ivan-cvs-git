# Microsoft Developer Studio Project File - Name="FEEL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FEEL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FEEL.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FEEL.MAK" CFG="FEEL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FEEL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FEEL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "FEEL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FEEL/Release"
# PROP Intermediate_Dir "FEEL/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "Include" /I "FeDX/Include" /I "FEEL/Include" /I "FeFile/Include" /I "FeIO/Include" /I "FELL/Include" /I "FeWin/Include" /I "FeMath/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FEEL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FEEL/Debug"
# PROP Intermediate_Dir "FEEL/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /Z7 /Od /I "Include" /I "FeDX/Include" /I "FEEL/Include" /I "FeFile/Include" /I "FeIO/Include" /I "FELL/Include" /I "FeWin/Include" /I "FeMath/Include" /D "_DEBUG" /D "VC" /D "WIN32" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "FEEL - Win32 Release"
# Name "FEEL - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\Feel\Source\error.cpp
# End Source File
# Begin Source File

SOURCE=.\Feel\Source\femain.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# Begin Source File

SOURCE=.\Feel\Include\error.h
# End Source File
# End Group
# End Target
# End Project
