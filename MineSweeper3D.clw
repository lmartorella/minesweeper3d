; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COptionsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MineSweeper3D.h"

ClassCount=7
Class1=CMineSweeper3DApp
Class2=CMineSweeper3DDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_HALLSOFFAME
Resource2=IDR_MAINFRAME
Resource3=IDD_RECORD
Resource4=IDD_ABOUTBOX
Class4=CHallsOfFameDlg
Class5=CWonDlg
Resource5=IDD_MINESWEEPER3D_DIALOG
Class6=CLanguage
Class7=COptionsDlg
Resource6=IDD_DIALOG1

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
HeaderFile=aboutdlg.h
ImplementationFile=aboutdlg.cpp
BaseClass=CDialog
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_VERSION,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342177283
Control5=IDC_LINK,static,1342308352

[DLG:IDD_MINESWEEPER3D_DIALOG]
Type=1
Class=CMineSweeper3DDlg
ControlCount=0

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
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC1,static,1342312961
Control3=IDC_LIST2,SysListView32,1350664961

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
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC1,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CLanguage]
Type=0
HeaderFile=Language.h
ImplementationFile=Language.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CLanguage

[DLG:IDD_DIALOG1]
Type=1
Class=COptionsDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LANG_STATIC,static,1342308352
Control4=IDC_COMBO1,combobox,1344340226
Control5=IDC_SLIDER1,msctls_trackbar32,1342242836
Control6=IDC_SENS_STATIC,static,1342308352

[CLS:COptionsDlg]
Type=0
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=COptionsDlg
VirtualFilter=dWC

