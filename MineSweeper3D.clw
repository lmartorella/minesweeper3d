; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MineSweeper3D.h"

ClassCount=3
Class1=CMineSweeper3DApp
Class2=CMineSweeper3DDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_MINESWEEPER3D_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDR_MENU1

[CLS:CMineSweeper3DApp]
Type=0
HeaderFile=MineSweeper3D.h
ImplementationFile=MineSweeper3D.cpp
Filter=N

[CLS:CMineSweeper3DDlg]
Type=0
HeaderFile=MineSweeper3DDlg.h
ImplementationFile=MineSweeper3DDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMineSweeper3DDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MineSweeper3DDlg.h
ImplementationFile=MineSweeper3DDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_VERSION,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MINESWEEPER3D_DIALOG]
Type=1
Class=CMineSweeper3DDlg
ControlCount=0

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_ICO1
Command2=IDM_ICO2
Command3=IDM_ICO3
Command4=IDM_ICO4
Command5=ID_FILE_EXIT
Command6=IDM_ABOUTBOX
CommandCount=6

