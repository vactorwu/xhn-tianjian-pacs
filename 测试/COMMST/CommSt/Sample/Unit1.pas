unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
	Dialogs, StdCtrls,Commstate, ExtCtrls, Buttons;

type
	TMainForm = class(TForm)
    Panel1: TPanel;
    Panel2: TPanel;
    Panel3: TPanel;
    Panel4: TPanel;
    Panel5: TPanel;
    Panel6: TPanel;
    Panel7: TPanel;
    Panel8: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    CommComponent: TCOMMStatus;
    procedure SpeedButton1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure SpeedButton2MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure SpeedButton3MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure SpeedButton4MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure CommComponentUpdate(Sender: TObject);
    procedure FormCreate(Sender: TObject);

	end;

var
  MainForm: TMainForm;

implementation

{$R *.DFM}

procedure TMainForm.SpeedButton1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
		 if SpeedButton1.Down then Exit;
		 CommComponent.COMM_NR := COM1;
		 Caption := 'TCOMMStatus Sample  COM Address = $'
			      +IntToHex(CommComponent.PhysAddress(COM1),3);
end;

procedure TMainForm.SpeedButton2MouseDown(Sender: TObject;
	Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
		 if SpeedButton2.Down then Exit;
		 CommComponent.COMM_NR := COM2;
		 Caption := 'TCOMMStatus Sample  COM Address = $'
			      +IntToHex(CommComponent.PhysAddress(COM2),3);
end;

procedure TMainForm.SpeedButton3MouseDown(Sender: TObject;
	Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
		 if SpeedButton3.Down then Exit;
		 CommComponent.COMM_NR := COM3;
		 Caption := 'TCOMMStatus Sample  COM Address = $'
			      +IntToHex(CommComponent.PhysAddress(COM3),3);
end;

procedure TMainForm.SpeedButton4MouseDown(Sender: TObject;
	Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
		 if SpeedButton4.Down then Exit;
		 CommComponent.COMM_NR := COM4;
		 Caption := 'TCOMMStatus Sample  COM Address = $'
			      +IntToHex(CommComponent.PhysAddress(COM4),3);
end;

procedure TMainForm.CommComponentUpdate(Sender: TObject);
begin
		 if	CommComponent.COMM_DCD = csOff
		   then Panel1.Color := clBlack
		   else Panel1.Color := clRed;
		 if CommComponent.COMM_RI  = csOff
		   then Panel2.Color := clBlack
		   else Panel2.Color := clYellow;
		 if CommComponent.COMM_DSR = csOff
		   then Panel3.Color := clBlack
		   else Panel3.Color := clRed;
		 if CommComponent.COMM_CTS = csOff
		   then Panel4.Color := clBlack
		   else Panel4.Color := clRed;
		 if CommComponent.COMM_RxD = csOff
		   then Panel5.Color := clBlack
		   else Panel5.Color := clLime;
		 if CommComponent.COMM_TxD = csOff
		   then Panel6.Color := clBlack
		   else Panel6.Color := clLime;
		 if CommComponent.COMM_RTS = csOff
		   then Panel7.Color := clBlack
		   else Panel7.Color := clRed;
		 if CommComponent.COMM_DTR = csOff
		   then Panel8.Color := clBlack
		   else Panel8.Color := clRed;
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
		 SpeedButton1.Enabled := CommComponent.CheckAvailable (COM1);
		 SpeedButton2.Enabled := CommComponent.CheckAvailable (COM2);
		 SpeedButton3.Enabled := CommComponent.CheckAvailable (COM3);
		 SpeedButton4.Enabled := CommComponent.CheckAvailable (COM4);
		 Caption := 'TCOMMStatus Sample  COM Address = $'
			      +IntToHex(CommComponent.PhysAddress(COM1),3);
end;

end.
