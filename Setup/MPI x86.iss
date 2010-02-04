[Setup]
AppId={{5B21E6C6-04C8-4131-8556-08CC6CCE1DE0}
AppName=MyPersonalIndex
AppVerName=MyPersonalIndex 3.0.0
AppPublisher=Matthew Wikler
AppPublisherURL=http://code.google.com/p/mypersonalindex/
AppSupportURL=http://code.google.com/p/mypersonalindex/
AppUpdatesURL=http://code.google.com/p/mypersonalindex/downloads/list
DefaultDirName={pf}\MyPersonalIndex
DefaultGroupName=MyPersonalIndex
AllowNoIcons=yes
LicenseFile=Info.txt
OutputDir=..\Setup
OutputBaseFilename=Setup_x32
Compression=lzma
SolidCompression=yes
MinVersion=0,5.0.2195
AppMutex=MyPersonalIndexApp

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\MyPersonalIndex\release\mypersonalindex.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\libgcc_s_dw2-1.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\mingwm10.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\QtCore4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\QtGui4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\QtNetwork4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\QtSql4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\sqldrivers\qsqlite4.dll; DestDir: {app}\sqldrivers; Flags: ignoreversion
Source: ..\MyPersonalIndex\release\qwt5.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\MPI.sqlite; DestDir: {app}; Flags: ignoreversion
Source: ..\Setup\License.txt; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\MyPersonalIndex; Filename: {app}\mypersonalindex.exe
Name: {commondesktop}\MyPersonalIndex; Filename: {app}\mypersonalindex.exe; Tasks: desktopicon
