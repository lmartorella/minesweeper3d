; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMineSweeper3DDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MineSweeper3D.h"

ClassCount=5
Class1=CMineSweeper3DApp
Class2=CMineSweeper3DDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_MINESWEEPER3D_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_RECORD
Resource4=IDD_ABOUTBOX
Class4=CHallsOfFameDlg
Resource5=IDD_HALLSOFFAME
Class5=CWonDlg
Resource6=IDR_MENU1

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
ControlCount=3
Control1=IDC_VERSION,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

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
Command5=IDM_FILE_EXIT
Command6=IDM_HALLSOFFAME
Command7=IDM_FILTERING
Command8=IDM_ABOUTBOX
CommandCount=8

[CLS:CHallsOfFameDlg]
Type=0
HeaderFile=HallsOfFameDlg.h
ImplementationFile=HallsOfFameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CHallsOfFameDlg
VirtualFilter=dWC

[DLG:IDD_HALLSOFFAME]
Type=1
Class=CHallsOfFameDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PUN1,static,1342308352
Control7=IDC_PUN2,static,1342308352
Control8=IDC_PUN3,static,1342308352
Control9=IDC_PUN4,static,1342308352

[CLS:CWonDlg]
Type=0
HeaderFile=WonDlg.h
ImplementationFile=WonDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CWonDlg

[DLG:IDD_RECORD]
Type=1
Class=CWonDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

