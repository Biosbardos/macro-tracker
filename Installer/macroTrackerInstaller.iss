; ============================
; Instalador MacroTracker
; ============================

[Setup]
AppName=macroTracker
AppVersion=1.2.0
DefaultDirName={localappdata}\macroTracker
OutputBaseFilename=MacroTrackerInstaller
SetupIconFile=assets\Icono_Moderno.ico
Compression=lzma2
SolidCompression=yes
LicenseFile=license.txt
PrivilegesRequired=lowest
Uninstallable=no
DisableProgramGroupPage=yes

[Files]

Source: "assets\Icono_Moderno.bmp";  Flags: dontcopy

; Copiar el ejecutable
Source: "macro-tracker.exe"; DestDir: "{app}"; Flags: ignoreversion

; Copiar archivo objetos.txt
Source: "objetos.txt"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist

; Copiar el archivo license.txt
Source: "license.txt"; DestDir: "{app}"; Flags: ignoreversion

; Copiar el archivo README.txt
Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion

; Copiar carpeta assets completa
Source: "assets\*"; DestDir: "{app}\assets"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Acceso directo en el escritorio
Name: "{userdesktop}\MacroTracker"; Filename: "{app}\macro-tracker.exe"; IconFilename: "{app}\assets\Icono_Moderno.ico"

; Acceso directo en el menú inicio
Name: "{userdesktop}\Carpeta_macroTracker"; Filename: "{app}"; WorkingDir: "{app}"; IconFilename: "{app}\assets\Icono_Retro.ico"

[Code]

var
  BienvenidaPage: TWizardPage;
  BienvenidaImage: TBitmapImage;

procedure InitializeWizard();
begin
  BienvenidaPage := CreateCustomPage(
    wpWelcome,
    'Bienvenido al instalador de MacroTracker',
    'Gracias por instalar MacroTracker. Este asistente te guiará durante el proceso.'
  );

  BienvenidaImage := TBitmapImage.Create(BienvenidaPage.Surface);
  BienvenidaImage.Parent := BienvenidaPage.Surface;

  ExtractTemporaryFile('Icono_Moderno.bmp');
  
  // Cargar la imagen desde {tmp}
  BienvenidaImage.Bitmap.LoadFromFile(ExpandConstant('{tmp}\Icono_Moderno.bmp'));

  // Tamaño del icono (ajústalo a tu gusto)
  BienvenidaImage.Stretch := True;
  BienvenidaImage.Center := True;

  BienvenidaImage.Width := ScaleX(512);
  BienvenidaImage.Height := ScaleY(300);
  
  BienvenidaImage.Left := (BienvenidaPage.SurfaceWidth - BienvenidaImage.Width) div 2;
  BienvenidaImage.Top := 0;
end;