[Setup]
AppId={{5B21E6C6-04C8-4131-8556-08CC6CCE1DE0}
AppName=MyPersonalIndex
AppVerName=MyPersonalIndex 3.1.0
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
ChangesAssociations=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; Flags: unchecked
Name: mpifileassoc; Description: "{cm:AssocFileExtension,MyPersonalIndex,.mpi}"

[Files]
Source: ..\mypersonalindex-build-desktop\release\mypersonalindex.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\libgcc_s_dw2-1.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\mingwm10.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\QtCore4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\QtGui4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\QtNetwork4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\QtSql4.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\sqldrivers\qsqlite4.dll; DestDir: {app}\sqldrivers; Flags: ignoreversion
Source: ..\mypersonalindex-build-desktop\release\qwt5.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\database\MPI.sqlite; DestDir: {app}; Flags: ignoreversion
Source: License.txt; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\MyPersonalIndex; Filename: {app}\mypersonalindex.exe
Name: {commondesktop}\MyPersonalIndex; Filename: {app}\mypersonalindex.exe; Tasks: desktopicon
[Registry]
Root: HKCR; SubKey: .mpi; ValueType: string; ValueData: MyPersonalIndex File; Flags: uninsdeletekey; Tasks: mpifileassoc
Root: HKCR; SubKey: MyPersonalIndex File; ValueType: string; ValueData: Portfolio data for use with MyPersonalIndex; Flags: uninsdeletekey; Tasks: mpifileassoc
Root: HKCR; SubKey: MyPersonalIndex File\Shell\Open\Command; ValueType: string; ValueData: """{app}\mypersonalindex.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: mpifileassoc
Root: HKCR; Subkey: MyPersonalIndex File\DefaultIcon; ValueType: string; ValueData: {app}\mypersonalindex.exe,0; Flags: uninsdeletevalue; Tasks: mpifileassoc
