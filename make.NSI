OutFile Installer\mine.exe
Name "Minesweeper 3D - Version 0.62 beta 1"
Caption "Minesweeper3D setup"
#Icon c:\work\puzzlebobble\work.ico
WindowIcon off

SilentInstall normal
SilentUnInstall normal

BGGradient 0000ff eeeeee
CRCCheck on

#MiscButtonText Indietro Avanti Annulla Chiudi
#InstallButtonText Installa
#FileErrorText "Non posso scrivere il file $0"

LicenseText "This software is freeware"
LicenseData Installer\license.txt

InstallDir '$PROGRAMFILES\Noinet\Minesweeper3D\'

DirShow show
DirText "Minesweeper 3D Installation$\n(C) Noinet Corp. 2001-02"
AllowRootDirInstall false
AutoCloseWindow true
CompletedText "Installation terminated!"

SetCompress force
SetDatablockOptimize on
SetDateSave on


 Function .onInstFailed
    MessageBox MB_OK "Installation failed!" 
 FunctionEnd

 Function un.onInit
    MessageBox MB_YESNO "This will uninstall MineSweeper 3D. Continue?" IDYES NoAbort
    Abort
    NoAbort:
 FunctionEnd

Section

#Installa i files
SetOutPath $INSTDIR
File Release\Minesweeper3D.exe
File texts\Readme.txt
File config.txt
SetOutPath $INSTDIR\Data
File /r Data\*.*
SetOutPath $INSTDIR\Help
File /r Help\*.*
SetOutPath $INSTDIR\Plugins
File /r Plugins\*.*

#Uninstall
WriteUninstaller Uninstall.exe

#Menu avvio
SetOutPath $INSTDIR
CreateDirectory "$SMPROGRAMS\Noinet"
CreateDirectory "$SMPROGRAMS\Noinet\Minesweeper 3D"
CreateShortCut "$SMPROGRAMS\Noinet\Minesweeper 3D\Minesweeper3D.lnk" "$INSTDIR\minesweeper3d.exe"
CreateShortCut "$SMPROGRAMS\Noinet\Minesweeper 3D\Help.lnk" "$INSTDIR\Help\index.html"
CreateShortCut "$SMPROGRAMS\Noinet\Minesweeper 3D\Readme.lnk" "$INSTDIR\Readme.txt"
CreateShortCut "$SMPROGRAMS\Noinet\Minesweeper 3D\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

MessageBox MB_OK "Successfully installed!"

SectionEnd





Section Uninstall
SetOutPath $INSTDIR
Delete $INSTDIR\minesweeper3d.exe
Delete $INSTDIR\Uninstall.exe
Delete $INSTDIR\readme.txt
Delete $INSTDIR\config.txt
Delete $INSTDIR\plugins\standard.dll
Delete $INSTDIR\plugins\toroid.dll
Rmdir $INSTDIR\plugins
Delete $INSTDIR\help\index.html
Rmdir $INSTDIR\help
Delete $INSTDIR\data\italian.lng
Delete $INSTDIR\data\english.lng
Delete $INSTDIR\data\textures.raw
Rmdir $INSTDIR\data
Rmdir $INSTDIR

Rmdir /r "$SMPROGRAMS\Noinet\Minesweeper 3D"
Rmdir "$SMPROGRAMS\Noinet"
SectionEnd





