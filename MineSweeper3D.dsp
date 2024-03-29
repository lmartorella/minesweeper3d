# Microsoft Developer Studio Project File - Name="MineSweeper3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MineSweeper3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MineSweeper3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MineSweeper3D.mak" CFG="MineSweeper3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MineSweeper3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MineSweeper3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MineSweeper3D - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile /nodefaultlib

!ELSEIF  "$(CFG)" == "MineSweeper3D - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MineSweeper3D - Win32 Release"
# Name "MineSweeper3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\HallsOfFameDlg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\MineSweeper3DDlg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\modules.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\String.cpp
# End Source File
# Begin Source File

SOURCE=.\vars.cpp
# End Source File
# Begin Source File

SOURCE=.\WonDlg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Programmi\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\HallsOfFameDlg.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\library.h
# End Source File
# Begin Source File

SOURCE=.\MineSweeper3D.h
# End Source File
# Begin Source File

SOURCE=.\MineSweeper3DDlg.h
# End Source File
# Begin Source File

SOURCE=.\modules.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\strings.h
# End Source File
# Begin Source File

SOURCE=.\vars.h
# End Source File
# Begin Source File

SOURCE=.\WonDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\mainico.ico
# End Source File
# Begin Source File

SOURCE=.\MineSweeper3D.rc
# End Source File
# Begin Source File

SOURCE=.\MineSweeper3D.rc2
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\config.txt
# End Source File
# Begin Source File

SOURCE=.\data\english.lng
# End Source File
# Begin Source File

SOURCE=.\data\italian.lng
# End Source File
# End Group
# Begin Source File

SOURCE=.\texts\alfatesting.txt
# End Source File
# Begin Source File

SOURCE=.\texts\alfatesting2.txt
# End Source File
# Begin Source File

SOURCE=.\texts\library.txt
# End Source File
# Begin Source File

SOURCE=.\texts\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\texts\todo.txt
# End Source File
# Begin Source File

SOURCE=.\OPENGL32.LIB
# End Source File
# Begin Source File

SOURCE=.\GLU32.LIB
# End Source File
# End Target
# End Project
