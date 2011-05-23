program ModTest;
{$APPTYPE CONSOLE}

uses
  SysUtils,
  CPort,
  Windows;

var
  ComPort: TComPort;
  Events: TComEvents;
  Answer, Data: string;
  Step: Integer;

begin
  try
    ComPort := TComPort.Create(nil);
    try
      if ParamCount > 0 then
        ComPort.Port := ParamStr(1)
      else
        ComPort.Port := 'COM1';
      ComPort.Events := [];
      ComPort.FlowControl.ControlDTR := dtrEnable;
      ComPort.FlowControl.ControlRTS := rtsEnable;
      ComPort.Open; // open port
      ComPort.WriteStr('AT'#13#10); // send test command
      Answer := '';
      Step := 0;
      repeat
        Events := [evRxChar];
        ComPort.WaitForEvent(Events, 2000);  // wait for charachters
        if evRxChar in Events then
        begin
          ComPort.ReadStr(Data, ComPort.InputCount);
          Answer := Answer + Data;
          if Pos('OK', Answer) > 0 then
            Break;
        end;
        Inc(Step)
      until (Events = []) or (Step = 20);
      if Pos('OK', Answer) > 0 then
        WriteLn('Modem found on ' + ComPort.Port)
      else
        WriteLn('Modem NOT found on ' + ComPort.Port);
    finally
      ComPort.Free;
    end;
  except
    on E: Exception do
      WriteLn('Error: ' + E.Message);
  end;
end.
