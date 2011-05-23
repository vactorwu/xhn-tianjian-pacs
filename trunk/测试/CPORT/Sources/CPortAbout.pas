(*************************************************
 * ComPort Library ver. 2.61                     *
 *   for Delphi 3, 4, 5 and C++ Builder 3, 4, 5  *
 * written by Dejan Crnila, 1998 - 2000          *
 * email: dejancrn@yahoo.com                     *
 *                                               *
 * Unit: CPortAbout.pas                          *
 *************************************************)

unit CPortAbout;

{$I CPort.inc}

interface

uses Windows, SysUtils, Classes, Graphics, Forms, Controls, StdCtrls,
  Buttons, ExtCtrls;

type
  // ComPort Library about box
  TAboutBox = class(TForm)
    Panel1: TPanel;
    ProgramIcon: TImage;
    ProductName: TLabel;
    Version: TLabel;
    Copyright: TLabel;
    OKButton: TButton;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

// show about box
procedure ShowAbout;

const
  CPortLibraryVersion = '2.61';

implementation

{$R *.DFM}

procedure ShowAbout;
begin
  with TAboutBox.Create(nil) do
  begin
    ShowModal;
    Free;
  end;
end;

procedure TAboutBox.FormCreate(Sender: TObject);
begin
  Version.Caption := 'version ' + CPortLibraryVersion;
end;

end.

