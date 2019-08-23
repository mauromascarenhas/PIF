[Setup]
AppId={{A153F592-6FE6-479C-A73C-57C85A269CFC}
AppName=PIF Compiler
AppVersion=0.9.08
AppPublisher=PIF Project
AppContact=https://github.com/mauromascarenhas/PIF/issues
AppPublisherURL=https://github.com/mauromascarenhas/PIF/
AppSupportURL=https://github.com/mauromascarenhas/PIF/issues
AppUpdatesURL=https://github.com/mauromascarenhas/PIF/
AppCopyright=Copyright (C) 2019 PIF Project
DefaultDirName={pf}\Projeto PIF\PIF
DisableDirPage=yes
DefaultGroupName=PIF
AllowNoIcons=yes
OutputDir=C:\Users\Mauro\Desktop\PIF_Compiler
OutputBaseFilename=pif_compiler-0.9.08_x86
SetupIconFile=C:\Users\Mauro\Documents\GitHub\PIF\src\compiler\logo\pif_logo_x64.ico
LicenseFile=C:\Users\Mauro\Documents\Completo\pif_compiler\licence\LGPL_V3.txt
UsePreviousAppDir=yes
UsePreviousGroup=yes
UsePreviousLanguage=yes
UsePreviousSetupType=yes
UsePreviousTasks=yes
UsePreviousUserInfo=yes
BackColor=$090067
BackSolid=yes
ChangesAssociations=yes
UninstallDisplayName=PIF
UninstallDisplayIcon={app}\pifc.exe
Compression=lzma2/max
SolidCompression=yes
ChangesEnvironment=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"

[Types]
Name: "full"; Description: "{cm:FullInstall}"
Name: "custom"; Description: "{cm:CustomInstall}"; Flags: iscustom

[Components]
Name: "program"; Description: "{cm:ProgramFiles}"; Types: full custom; Flags: fixed
Name: "sources"; Description: "{cm:SourceFiles}"; Types: full;

[Tasks]
Name: "envPath"; Description: "{cm:AddToPath}" 

[Files]
Source: "C:\Users\Mauro\Documents\Completo\Extra\vc_redist.x86.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall; AfterInstall: InstMSVSC
Source: "C:\Users\Mauro\Documents\Completo\pif_compiler\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: program
Source: "C:\Users\Mauro\Documents\GitHub\PIF\*"; DestDir: "{app}\sources"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: sources

[Code]
procedure ExitProcess(exitCode:integer);
  external 'ExitProcess@kernel32.dll stdcall';

procedure InstMSVSC;
var
  ResultCode: Integer;       
begin
  if not FileExists(ExpandConstant('{sys}\MSVCP140.DLL'))
  then
    if not Exec(ExpandConstant('{tmp}\vc_redist.x86.exe'), '', '', SW_SHOWNORMAL,
      ewWaitUntilTerminated, ResultCode)
    then
      ExitProcess(1);
  if not FileExists(ExpandConstant('{sys}\MSVCP140.DLL'))
  then
  ExitProcess(1);
end;

const EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure EnvAddPath(Path: string);
var
    Paths: string;
begin
    { Retrieve current path (use empty string if entry not exists) }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Paths := '';

    { Skip if string already found in path }
    if Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';') > 0 then exit;

    { App string to the end of the path variable }
    Paths := Paths + ';'+ Path +';'

    { Overwrite (or create if missing) path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] added to PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while adding the [%s] to PATH: [%s]', [Path, Paths]));
end;

procedure EnvRemovePath(Path: string);
var
    Paths: string;
    P: Integer;
begin
    { Skip if registry entry not exists }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths) then
        exit;

    { Skip if string not found in path }
    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then exit;

    { Update path variable }
    Delete(Paths, P - 1, Length(Path) + 1);

    { Overwrite path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] removed from PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while removing the [%s] from PATH: [%s]', [Path, Paths]));
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and IsTaskSelected('envPath')
      then EnvAddPath(ExpandConstant('{app}') {+'\bin'});
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}') {+'\bin'});
end;

[Icons]
Name: "{group}\{cm:ProgramSources}"; Filename: "{app}\sources"
Name: "{group}\{cm:Licence}"; Filename: "{app}\licence\LGPL_V3.txt"
Name: "{group}\{cm:ProgramOnTheWeb,PIF}"; Filename: "https://github.com/mauromascarenhas/PIF/"
Name: "{group}\{cm:UninstallProgram,PIF Compiler}"; Filename: "{uninstallexe}"

[CustomMessages]
AddToPath=Add PIFC to PATH environment variable (recommended)
CustomInstall=Custom installation
FullInstall=Full installation
Licence=LGPL_v3 Licence
ProgramFiles=Program Files
ProgramSources=PIF Compiler sources
SourceFiles=Source Files

brazilianportuguese.AddToPath=Adicionar PIFC à variável de ambiente PATH (recomendado)
brazilianportuguese.CustomInstall=Instalação personalizada
brazilianportuguese.FullInstall=Instalação completa
brazilianportuguese.Licence=Licença LGPL_v3
brazilianportuguese.ProgramFiles=Arquivos de programa
brazilianportuguese.ProgramSources=Código fonte do compilador PIF
brazilianportuguese.SourceFiles=Código fonte