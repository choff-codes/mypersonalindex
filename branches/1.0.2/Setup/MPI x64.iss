[Setup]
AppId={{5B21E6C6-04C8-4131-8556-08CC6CCE1DE0}
AppName=MyPersonalIndex
AppVerName=MyPersonalIndex 1.0.3
AppPublisher=Matthew Wikler
AppPublisherURL=http://code.google.com/p/mypersonalindex/
AppSupportURL=http://code.google.com/p/mypersonalindex/
AppUpdatesURL=http://code.google.com/p/mypersonalindex/downloads/list
DefaultDirName={pf}\MyPersonalIndex
DefaultGroupName=MyPersonalIndex
AllowNoIcons=yes
LicenseFile=Info.txt
;InfoBeforeFile=
OutputDir=..\Setup
OutputBaseFilename=Setup_x64
Compression=lzma
SolidCompression=yes
MinVersion=0,5.01.2600sp2
AppMutex=MyPersonalIndex
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\MyPersonalIndex\bin\x64\Release\MyPersonalIndex.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlceca35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlcecompact35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlceer35EN.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlceme35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlceoledb35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlceqp35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\sqlcese35.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\SQLCe\x64\System.Data.SqlServerCe.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\ZedGraph.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\MyPersonalIndex\bin\x64\Release\MPI.sdf; DestDir: {app}; Flags: ignoreversion
Source: ..\Setup\License.txt; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\MyPersonalIndex; Filename: {app}\MyPersonalIndex.exe
Name: {commondesktop}\MyPersonalIndex; Filename: {app}\MyPersonalIndex.exe; Tasks: desktopicon

;[Run]
;Filename: {app}\MyPersonalIndex.exe; Description: {cm:LaunchProgram,MyPersonalIndex}; Flags: nowait postinstall skipifsilent runasoriginaluser

[Code]
//procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
//begin
//  case CurUninstallStep of
//    usPostUninstall:
//      begin
//        if MsgBox('Would you like to remove your portfolio data?', mbConfirmation, MB_YESNO) = IDYES then
//        begin
//          DeleteFile(ExpandConstant('{userappdata}\MyPersonalIndex\MPI.sdf'));
//          RemoveDir(ExpandConstant('{userappdata}\MyPersonalIndex'));
//        end;
//      end;
//  end;
//end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ErrorCode: Integer;
begin
  case CurStep of
    ssInstall:
      begin
        if RegKeyExists(HKLM, 'Software\Microsoft\NET Framework Setup\NDP\v2.0.50727') = false then
          begin
            if MsgBox('You must install Microsoft .NET. Press OK to go to Microsoft''s website or Cancel to continue.', mbConfirmation, MB_OKCANCEL) = IDOK then
            begin
				      ShellExec('open','http://msdn.microsoft.com/en-us/netframework/','','',SW_SHOWNORMAL,ewNoWait,ErrorCode);
            end;
          end;
      end;
  end;
end;
