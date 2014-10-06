!include "MUI2.nsh"

Name "GeanyPy Installer"
OutFile "GeanyPy-Installer.exe"
InstallDir $PROGRAMFILES\Geany
DirText "This will install GeanyPy into your existing Geany directory. Make sure to choose the directory where Geany is installed below before proceeding."
RequestExecutionLevel user

!define _VERSION "0.1.0.0"
VIProductVersion "${_VERSION}"
VIAddVersionKey "ProductName" "GeanyPy Installer"
VIAddVersionKey "CompanyName" "Matthew Brush"
VIAddVersionKey "FileVersion" "${_VERSION}"
VIAddVersionKey "InternalName" "GeanyPy-Installer.exe"
VIAddVersionKey "FileDescription" "Installs GeanyPy into a Geany installation"
VIAddVersionKey "LegalCopyright" "Copyright (c) 2012 Matthew Brush"

!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_LICENSE "COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"


Section ""

SetOutPath $INSTDIR\doc\geanypy
File /oname=README.txt README.markdown
File /oname=COPYING.txt COPYING

SetOutPath $INSTDIR\lib
File src\geanypy.dll

SetOutPath $INSTDIR\lib\geanypy\geany
File geany\*.py

SetOutPath $INSTDIR\lib\geanypy\plugins
File plugins\*.py

WriteUninstaller "$INSTDIR\geanypy-uninstall.exe"

SectionEnd

Section "Uninstall"

SetOutPath $INSTDIR

Delete "$INSTDIR\geanypy-uninstall.exe"
Delete "$INSTDIR\lib\geanypy.dll"
Delete "$INSTDIR\lib\geanypy\geany\__init__.py"
Delete "$INSTDIR\lib\geanypy\geany\__init__.pyc"
Delete "$INSTDIR\lib\geanypy\geany\console.py"
Delete "$INSTDIR\lib\geanypy\geany\loader.py"
Delete "$INSTDIR\lib\geanypy\geany\loader.pyc"
Delete "$INSTDIR\lib\geanypy\geany\logger.py"
Delete "$INSTDIR\lib\geanypy\geany\logger.pyc"
Delete "$INSTDIR\lib\geanypy\geany\manager.py"
Delete "$INSTDIR\lib\geanypy\geany\manager.pyc"
Delete "$INSTDIR\lib\geanypy\geany\plugin.py"
Delete "$INSTDIR\lib\geanypy\geany\plugin.pyc"
Delete "$INSTDIR\lib\geanypy\geany\signalmanager.pyc"
Delete "$INSTDIR\lib\geanypy\geany\console.pyc"
Delete "$INSTDIR\lib\geanypy\geany\demo.pyc"
Delete "$INSTDIR\lib\geanypy\geany\hello.pyc"
Delete "$INSTDIR\lib\geanypy\geany\signalmanager.py"
Delete "$INSTDIR\lib\geanypy\plugins\console.py"
Delete "$INSTDIR\lib\geanypy\plugins\demo.py"
Delete "$INSTDIR\lib\geanypy\plugins\hello.py"
Delete "$INSTDIR\doc\geanypy\README.txt"
Delete "$INSTDIR\doc\geanypy\COPYING.txt"

RMDir "$INSTDIR\lib\geanypy\geany"
RMDir "$INSTDIR\lib\geanypy\plugins"
RMDir "$INSTDIR\lib\geanypy"
RMDir "$INSTDIR\doc\geanypy"


SectionEnd
