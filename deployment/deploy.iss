; -- Example3.iss --
; Same as Example1.iss, but creates some registry entries too.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=Genius
AppVersion=1.1
DefaultDirName={pf}\Genius
DefaultGroupName=Genius
UninstallDisplayIcon={app}\Genius.exe
AppCopyright=Copyright (C) 2015-2020 Shanaka Rusith
SetupIconFile=genius.ico

[Files]
Source: "Genius.exe"; DestDir: "{app}"
Source: "genius.ico"; DestDir: "{app}"
Source: "libEGL.dll"; DestDir: "{app}"
Source: "libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "libGLESV2.dll"; DestDir: "{app}"
Source: "libstdc++-6.dll"; DestDir: "{app}"
Source: "libwinpthread-1.dll"; DestDir: "{app}"
Source: "Qt5Core.dll"; DestDir: "{app}"
Source: "Qt5Gui.dll"; DestDir: "{app}"
Source: "Qt5Svg.dll"; DestDir: "{app}"
Source: "Qt5Widgets.dll"; DestDir: "{app}"
Source: "platforms/qwindows.dll"; DestDir: "{app}/platforms"

[Icons]
Name: "{group}\Genius.exe"; Filename: "{app}\Genius.exe"

[Registry]
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Genius"; ValueData: """{app}\Genius.exe"""; Flags: uninsdeletevalue
