(*************************************************
 * ComPort Library ver. 2.61                     *
 *   for Delphi 3, 4, 5 and C++ Builder 3, 4, 5  *
 * written by Dejan Crnila, 1998 - 2000          *
 * email: dejancrn@yahoo.com                     *
 *                                               *
 * Unit: CPortSetup.pas                          *
 *************************************************)

unit CPortSetup;

{$I CPort.inc}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, CPort, CPortCtl;

type
  // TComPort setup dialog
  TComSetupFrm = class(TForm)
    Button1: TButton;
    Button2: TButton;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Combo2: TComComboBox;
    Combo3: TComComboBox;
    Combo4: TComComboBox;
    Combo5: TComComboBox;
    Combo6: TComComboBox;
    Combo1: TComComboBox;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

procedure EditComPort(ComPort: TCustomComPort);

implementation

{$R *.DFM}

// show setup dialog
procedure EditComPort(ComPort: TCustomComPort);
begin
  with TComSetupFrm.Create(nil) do
  begin
    Combo1.ComPort := ComPort;
    Combo2.ComPort := ComPort;
    Combo3.ComPort := ComPort;
    Combo4.ComPort := ComPort;
    Combo5.ComPort := ComPort;
    Combo6.ComPort := ComPort;
    Combo1.UpdateSettings;
    Combo2.UpdateSettings;
    Combo3.UpdateSettings;
    Combo4.UpdateSettings;
    Combo5.UpdateSettings;
    Combo6.UpdateSettings;
    if ShowModal = mrOK then
    begin
      ComPort.BeginUpdate;
      Combo1.ApplySettings;
      Combo2.ApplySettings;
      Combo3.ApplySettings;
      Combo4.ApplySettings;
      Combo5.ApplySettings;
      Combo6.ApplySettings;
      ComPort.EndUpdate;
    end;
    Free;
  end;
end;

end.
