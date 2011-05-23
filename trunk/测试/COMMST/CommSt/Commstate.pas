(**************************     TCOMMStatus     *******************************

 This component is ONLY DESIGNED FOR WINDOWS 95 AND WINDOWS 98 !!!
 WINDOWS NT 4 don't know QT_THUNK Procedure ( see Thunk.pas )!!!

 This component can be freely used and distributed in commercial and private
 environments, provided this notice is not modified in any way.
 Feel free to contact us if you have any questions, comments or suggestions
 at TRSOFT@Menden.net

    Copyright © 1998 by TRSOFT  All Rights Reserved.
		Thomas Radtke Software Entwicklung.
		http://www.abcnet.de/TRSOFT/

	THIS SOFTWARE IS PROVIDED AS IS AND WITHOUT WARRANTY OF ANY KIND,
	EITHER EXPRESSED OR IMPLIED.

------------------------------- History --------------------------------------
 Ver 1.0 04.04.98
			 Main Component Build.

 Ver 1.1 05.04.98
			 Threaded Timer added. ( Copyright Carlos Barbosa )

 Ver 1.2 08.04.98
			 Registry check for physicaly COMM Port Address added;

 Ver 1.3 09.04.98
			 property Available , public function CheckAvailable
			 public function PhysAddress added;

 Ver 2.0 10.04.98
			 Registry Check removed ( Too many Errors !)
			 Com port available check via QT_Thunk from Com16.dll ( see Thunk.pas Unit)
			 TAddress  changed from (COMM1, COMM2, COMM3, COMM4 )
				   to (COM1, COM2, COM3, COM4 )

 Ver 2.1 14.04.98
			 Optional can be using of Com16.dll removed via Compiler setting $NODLL
 				   
------------------------------------------------------------------------------
******************************************************************************)

unit Commstate;

{x$DEFINE DESIGNUPDATE} // Remove 'x' for Update in Design Mode;

{x$DEFINE NODLL}        // Remove 'x' if QT_THUNK procedure not avaiable or
					// if You don't wont to use Com16.Dll


interface

uses
	Windows, Messages, SysUtils, Classes, Graphics, Controls,
	Forms, Dialogs, ExtCtrls
	{$IFNDEF NODLL}
	,Thunk
	{$ENDIF}
	;

type
(******************************   Threaded Timer    ***************************)
		TThreadedTimer = class;

		TTimerThread = class(TThread)
		  OwnerTimer: TThreadedTimer;
		  Interval: DWord;
		  procedure Execute; override;
		  procedure DoTimer;
		end;

	TThreadedTimer = class(TComponent)
  private
    FEnabled: Boolean;
    FInterval: Word;
    FOnTimer: TNotifyEvent;
    FTimerThread: TTimerThread;
		FThreadPriority: TThreadPriority;
		procedure UpdateTimer;
    procedure SetEnabled(Value: Boolean);
    procedure SetInterval(Value: Word);
    procedure SetOnTimer(Value: TNotifyEvent);
		procedure SetThreadPriority(Value: TThreadPriority);
		procedure Timer; dynamic;
		constructor Create(AOwner: TComponent); override;
		destructor Destroy; override;
		property Enabled: Boolean read FEnabled write SetEnabled default True;
    property Interval: Word read FInterval write SetInterval default 1000;
		property OnTimer: TNotifyEvent read FOnTimer write SetOnTimer;
		property ThreadPriority: TThreadPriority read FThreadPriority
		  write SetThreadPriority;
  end;
(********************************* TCOMMStatus ********************************)

	TCOMMState = (csOn, csOff);
	TInterval  = 0..10000;
	TUpdate    = procedure(Sender: TObject) of object;
	TAddress   = (COM1, COM2, COM3, COM4 );
	TCOMMStatus = class(TComponent)
	private
		Adr1,
		Adr2,
		Adr3,
		Adr4 : Word;
		FAdr : Word;
		FDCD : TCOMMState;
		FRI  : TCOMMState;
		FDSR : TCOMMState;
		FCTS : TCOMMState;
		FRxD : TCOMMState;
		FTxD : TCOMMState;
		FRTS : TCOMMState;
		FDTR : TCOMMState;
		FAddress   : TAddress;
		FEnabled   : Boolean;
		FAvail     : Boolean;
		Dummy      : Boolean;
		FOnUpdate  : TUpdate;
		FInterval  : TInterval;
		FTimer     : TThreadedTimer;
		FThreadPriority : TThreadPriority;
		function  SetAdr(value: TAddress): Word;
		procedure SetAddress(value: TAddress);
		procedure SetInterval(value: TInterval);
		procedure SetEnabled(value: Boolean);
		procedure SetThreadPriority(Value: TThreadPriority);
		procedure BuildList;
  protected
		DummyCOMMState: TCOMMState;
    procedure Update;
    procedure TimerTimer(Sender: TObject);
    procedure Loaded; override;
  public
    constructor Create(AOwner: TComponent); override;
		destructor Destroy; override;
		function CheckAvailable ( value :TAddress):Boolean;
		function PhysAddress (value :TAddress ) : word;
	published
		property COMM_DCD : TCOMMState read FDCD write DummyCOMMState;
		property COMM_RI  : TCOMMState read FRI  write DummyCOMMState;
		property COMM_DSR : TCOMMState read FDSR write DummyCOMMState;
		property COMM_CTS : TCOMMState read FCTS write DummyCOMMState;
		property COMM_RxD : TCOMMState read FRxD write DummyCOMMState;
		property COMM_TxD : TCOMMState read FTxD write DummyCOMMState;
		property COMM_RTS : TCOMMState read FRTS write DummyCOMMState;
		property COMM_DTR : TCOMMState read FDTR write DummyCOMMState;

		property COMM_NR      : TAddress  read FAddress  write SetAddress  default COM1;
		property Available    : Boolean   read FAvail    write Dummy;
		property Enabled      : Boolean   read FEnabled  write SetEnabled  default True;
		property Interval     : TInterval read FInterval write SetInterval default 5;
		property OnUpdate     : TUpdate   read FOnUpdate write FOnUpdate;
		property ThreadPriority: TThreadPriority
			     read FThreadPriority  write SetThreadPriority default tpNormal;

 end;

procedure Register;

implementation


function ReadPort(Port: word): byte; assembler; register;
asm
	 mov dx,Port
	 cli
	 in al,dx
	 sti
end;

(******************************   Threaded Timer    ***************************)
(*     Copyright 1996, 1997 Carlos Barbosa   			              *)
(*		 email: cmab@digiserve.com                                            *)
(*	     Home Page: http://www.digiserve.com/cmab                             *)
(*    Threaded Timer is Freeware .                                                                        *)
(******************************************************************************)
procedure TTimerThread.Execute;
begin
   repeat
      SleepEx(Interval, False);
      Synchronize(DoTimer);
   until Terminated;
end;

procedure TTimerThread.DoTimer;
begin
   OwnerTimer.Timer;
end;

constructor TThreadedTimer.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FEnabled := True;
	FInterval := 1000;
	FThreadPriority := tpNormal;
  FTimerThread := TTimerThread.Create(False);
  FTimerThread.OwnerTimer := Self;
  FTimerThread.Interval := FInterval;
  FTimerThread.Priority := FThreadPriority;
end;

destructor TThreadedTimer.Destroy;
begin
  FEnabled := False;
  UpdateTimer;
  FTimerThread.Free;
  inherited Destroy;
end;

procedure TThreadedTimer.UpdateTimer;
begin
   if FTimerThread.Suspended = False then
      FTimerThread.Suspend;

   if (FInterval <> 0) and FEnabled and Assigned(FOnTimer) then
      FTimerThread.Resume;
end;

procedure TThreadedTimer.SetEnabled(Value: Boolean);
begin
   if Value <> FEnabled then
   begin
      FEnabled := Value;
		  UpdateTimer;
   end;
end;

procedure TThreadedTimer.SetInterval(Value: Word);
begin
   if Value <> FInterval then
   begin
		  FInterval := Value;
      FTimerThread.Interval := FInterval;
      UpdateTimer;
   end;
end;

procedure TThreadedTimer.SetOnTimer(Value: TNotifyEvent);
begin
   FOnTimer := Value;
   UpdateTimer;
end;

procedure TThreadedTimer.SetThreadPriority(Value: TThreadPriority);
begin
   if Value <> FThreadPriority then
	 begin
      FThreadPriority := Value;
		  FTimerThread.Priority := Value;
		  UpdateTimer;
	 end;
end;

procedure TThreadedTimer.Timer;
begin
   if Assigned(FOnTimer) then
      FOnTimer(Self);
end;

(*****************************************************************************)
(********************************* TCOMMStatus *******************************)
(*****************************************************************************)

procedure TCOMMStatus.Update;
begin
	if assigned(FOnUpdate) then FOnUpdate(self);
end;

procedure TCOMMStatus.SetInterval(value: TInterval);
begin
  if Value <> FInterval then
  begin
		FInterval := Value;
		{$IFNDEF DESIGNUPDATE}
		if not (csDesigning in ComponentState) then
		{$ENDIF}
		FTimer.FInterval:= FInterval;
	end;
end;

procedure TCOMMStatus.SetEnabled(value: boolean);
begin
  if (value <> FEnabled) then
  begin
		FEnabled := value;
		{$IFNDEF DESIGNUPDATE}
		if not (csDesigning in Componentstate) then
		{$ENDIF}
		FTimer.Enabled := FEnabled;
	end;
end;

procedure TCOMMStatus.SetAddress(value: TAddress);
begin
		 if Value <> FAddress then
		 begin
			  FAddress := Value;
			  FAdr := SetAdr(FAddress);
			  if FAdr <> 0
			    then FAvail := True
			    else FAvail := False;
          Update;
		 end;
end;

procedure TCOMMStatus.SetThreadPriority(Value: TThreadPriority);
begin
   if Value <> FThreadPriority then
	 begin
		  FThreadPriority := Value;
		  {$IFNDEF DESIGNUPDATE}
		  if not (csDesigning in ComponentState) then
		  {$ENDIF}
		  FTimer.ThreadPriority := Value;
	 end;
end;

procedure TCOMMStatus.TimerTimer(Sender: TObject);
const
        { Offset from base COM port }
      MODEMCONTROL = 4;
      LINESTATUS   = 5;
      MODEMSTATUS  = 6;

       { Masks }
		  _DCD = $80; { Data Carrier Detect    }
		  _RI  = $40; { Ring Indicator         }
		  _DSR = $20; { Data Set Ready         }
		  _CTS = $10; { Clear To Send          }
		  _RxD = $01; { Received Data Ready    }
		  _TxD = $40; { Data Transmitter Empty }
	//  _TxD = $20; { Data Hold Transmitter Empty }
		  _RTS = $02; { Request To Send        }
		  _DTR = $01; { Data Terminal Ready    }
var
	 Status : Byte;
	 Adr    : Word;
begin
		 FTimer.Enabled := False;
		 try
			 Adr := FAdr;
			 if Adr = 0 then Exit;    (* don't read Port if COM not Available*)

			 Status := ReadPort(Adr + MODEMCONTROL);
			  if (Status and _DTR > 0)
			    then FDTR := csOn
			    else FDTR := csOff;
			  if (Status and _RTS > 0)
			    then FRTS := csOn
			    else FRTS := csOff;

			 Status := ReadPort(Adr + LINESTATUS);
			  if (Status and _RxD > 0)
			    then FRxD := csOn
			    else FRxD := csOff;

 (*  TXD status reading is possible in two Ways
		   1:  if (Status and _TxD = 0) <-- _TxD = $ 40
		   2:  if (Status and _TxD > 0) <-- _TxD = $ 20

	Just try one of this ! *)

		   (* if (Status and $20 > 0)
			  then FTxD := csOn
			  else FTxD := csOff; *)

			  if (Status and _TxD = 0)
			    then FTxD := csOn
			    else FTxD := csOff;

			 Status := ReadPort(Adr + MODEMSTATUS);
			  if (Status and _RI > 0)
			    then FRI := csOn
			    else FRI := csOff;
			  if (Status and _CTS > 0)
			    then FCTS := csOn
			    else FCTS := csOff;
			  if (Status and _DSR > 0)
			    then FDSR := csOn
			    else FDSR := csOff;
			  if (Status and _DCD > 0)
			    then FDCD := csOn
			    else FDCD := csOff;
			 Update;
		  finally
			    FTimer.Enabled := True;
		  end;
end;

procedure TCOMMStatus.Loaded;
begin
		 {$IFNDEF DESIGNUPDATE}
		 if not (csDesigning in Componentstate) then
		 {$ENDIF}
		 FTimer.Enabled := FEnabled;
end;

procedure TCOMMStatus.BuildList;
{$IFNDEF NODLL}
var
	 X : Word;
	 Y : Byte;
{$ENDIF}	 
begin
		 {$IFNDEF NODLL}
		 Y := ComNum;
		 if (Y AND 1) = 1 then
		   begin
			    X := MemW($040,$0);
			    Adr1 := StrToInt('$'+ IntToHex(X,3));
		   end
			  else Adr1 := 0;

		 if (Y AND 2) = 2 then
		   begin
			    X := MemW($040,$2);
			    Adr2 := StrToInt('$'+ IntToHex(X,3));
		   end
			  else Adr2 := 0;

		 if (Y AND 4) = 4 then
		   begin
			    X := MemW($040,$4);
			    Adr3 := StrToInt('$'+ IntToHex(X,3));
		   end
			  else Adr3 := 0;

		 if (Y AND 8) = 8 then
		   begin
			    X := MemW($040,$6);
			    Adr4 := StrToInt('$'+ IntToHex(X,3));
		   end
			  else Adr4 := 0;
		{$ELSE}

			 Adr1 := $3F8;
			 Adr2 := $2F8;
			 Adr3 := $3E8;
			 Adr4 := $2E8;

		{$ENDIF}
end;
function TCOMMStatus.SetAdr ( value : TAddress) : Word;
begin
		 case value
			  of
			    COM1 : SetAdr := Adr1;
			    COM2 : SetAdr := Adr2;
			    COM3 : SetAdr := Adr3;
			    COM4 : SetAdr := Adr4;
			    else
				SetAdr := 0;
			  end;
end;

function TCOMMStatus.CheckAvailable (value :TAddress):Boolean;
var
	 x : Word;
begin
		 x := SetAdr( value );
		 Result := x <> 0;
end;

function TCOMMStatus.PhysAddress (value :TAddress ) : word;
begin
		 Result := SetAdr( value );
end;

constructor TCOMMStatus.Create(AOwner: TComponent);
begin
  inherited;
	FEnabled  := True;
	FInterval := 5;
	BuildList;
	FAddress  := COM1;
	FAdr := SetAdr(FAddress);
	if FAdr <> 0
		then FAvail := True
		else FAvail := False;
	FThreadPriority := tpNormal;

	{$IFNDEF DESIGNUPDATE}
	if not (csDesigning in ComponentState) then
	{$ENDIF}
	begin
		FTimer := TThreadedTimer.Create(self);
		FTimer.Interval := FInterval;
		FTimer.ThreadPriority := FThreadPriority;
		FTimer.OnTimer := TimerTimer;
		FTimer.Enabled := FEnabled;
	end;
end;

destructor TCOMMStatus.Destroy;
begin
  FTimer.Free;
  inherited;
end;

(****************************** Register Procedure ***************************)

procedure Register;
begin
	RegisterComponents('TRSOFT', [TCOMMStatus]);
end;

end.
