;--------------------------------
;Clockwork Editor NSIS Installation Script
;--------------------------------

!ifndef CLOCKWORK_ROOT
!error "CLOCKWORK_ROOT NOT DEFINED"
!endif

!ifndef EDITOR_VERSION
!error "EDITOR_VERSION NOT DEFINED"
!endif

!ifndef INSTALLER_NAME
!error "INSTALLER_NAME NOT DEFINED"
!endif

!define prodname "Clockwork Editor"
!define coname "THUNDERBEAST GAMES LLC"
!define outfile "${CLOCKWORK_ROOT}\Artifacts\Build\WindowsInstaller\${INSTALLER_NAME}"
!define appexe "ClockworkEditor.exe"
!define produrl "http://www.clockworkgameengine.com/"

;--------------------------------
;Include Modern UI
;--------------------------------

!include "MUI.nsh"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "Windows_Installer_Header.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Windows_Installer_LeftImage.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "Windows_Installer_Header.bmp"
!define MUI_HEADER_TRANSPARENT_TEXT

;--------------------------------
;General
;--------------------------------

;Name and file
Name "${prodname}"
OutFile "${outfile}"
RequestExecutionLevel admin

;Default installation folder
InstallDir "$PROGRAMFILES64\${prodname}"

;Get installation folder from registry if available
InstallDirRegKey HKLM "Software\${coname}\InstallDir" "${prodname}"

;--------------------------------
;Interface Settings
;--------------------------------

!define MUI_ABORTWARNING

;--------------------------------
;Pages
;--------------------------------

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
	; These indented statements modify settings for MUI_PAGE_FINISH
	!define MUI_FINISHPAGE_NOAUTOCLOSE
	!define MUI_FINISHPAGE_RUN "$INSTDIR\ClockworkEditor\${appexe}"
	!define MUI_FINISHPAGE_RUN_CHECKED
	!define MUI_FINISHPAGE_RUN_TEXT "Launch the Clockwork Editor"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
;--------------------------------

!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections
;--------------------------------

Section "${prodname}" SecMain

	SetShellVarContext all

	SetOutPath "$INSTDIR"

  File /r "${CLOCKWORK_ROOT}\Artifacts\Build\WindowsEditor\*.*"

	;Store installation folder
	WriteRegStr HKLM "Software\${coname}\InstallDir" "${prodname}" $INSTDIR

	; Create shortcut
	CreateShortCut "$DESKTOP\${prodname}.lnk" "$INSTDIR\ClockworkEditor\${appexe}"
	CreateShortCut "$SMPROGRAMS\${prodname}.lnk" "$INSTDIR\ClockworkEditor\${appexe}"

	; Update Add/Remove Programs
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "DisplayName" "${prodname}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "UninstallString" "$INSTDIR\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "DisplayIcon" "$INSTDIR\ClockworkEditor\${appexe},0"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "Publisher" "${coname}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "URLInfoAbout" "${produrl}"
	WriteRegDWord HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoRepair" 1
	WriteRegDWord HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoModify" 1

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section
;--------------------------------

Section "Uninstall"

	SetShellVarContext all

	; Remove short cut
	Delete "$SMPROGRAMS\${prodname}.lnk"
	Delete "$DESKTOP\${prodname}.lnk"

	; Remove installation folder
  RMDir /r $INSTDIR

	; Remove from Add/Remove Programs
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"

	; Remove installation folder
	DeleteRegValue HKLM "Software\${coname}\InstallDir" "${prodname}"
	DeleteRegKey /ifempty HKLM "Software\${coname}\InstallDir"
	DeleteRegKey /ifempty HKLM "Software\${coname}"

SectionEnd
