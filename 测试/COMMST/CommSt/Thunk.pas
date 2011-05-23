(**************************    Thunking Unit     *******************************

 This component can be freely used and distributed in commercial and private
 environments, provided this notice is not modified in any way.
 Feel free to contact us if you have any questions, comments or suggestions
 at TRSOFT@Menden.net

    Copyright © 1998 by TRSOFT  All Rights Reserved.
		Thomas Radtke Software Entwicklung.
		http://www.abcnet.de/TRSOFT/

	THIS SOFTWARE IS PROVIDED AS IS AND WITHOUT WARRANTY OF ANY KIND,
	EITHER EXPRESSED OR IMPLIED.

******************************************************************************)

unit Thunk;

interface

function Mem(seg,off : Word):Byte; // Like Delphi 1 Mem , but read only;
(* Syntax Result := Mem ( $040, $060); *)

function MemW(seg,off :Word):Word; // Like Delphi 1 MemW , but read only;
(* Syntax Result := MemW ( $040, $060); *)

function ComNum: Byte; // Com Port available check

(*
**  ComNum is Routine to return a 4-bit pattern to determine which COM ports
**  are available for use as far as Windows was concerned at startup.
**  Bit set means COM port available, unset means not available.
**  bit    8    4    2    1
**       COM4 COM3 COM2 COM1
**  E.G. If COM1 and COM3 are available, then 5 would be returned (0101)*)

implementation

uses SysUtils, Windows;

{ Undocumented Kernel32 calls. }
function LoadLibrary16(LibraryName: PChar): THandle; stdcall; external kernel32 index 35;
procedure FreeLibrary16(HInstance: THandle); stdcall; external kernel32 index 36;
function GetProcAddress16(Hinstance: THandle; ProcName: PChar): Pointer; stdcall; external kernel32 index 37;
procedure QT_Thunk; cdecl; external kernel32 name 'QT_Thunk';

var
  hInst16: THandle; 
  pFunc1: Pointer; 
  pFunc2: Pointer; 
  pFunc3: Pointer; 

{ QT_Thunk needs a stack frame. }
{$StackFrames On}

function MemW(seg,off :Word):Word;
var
  ThunkTrash: array[0..$20] of Word;
begin
  ThunkTrash[0] := hInst16;

  hInst16 := LoadLibrary16('COM16.DLL');
  if hInst16 < 32 then
    raise Exception.Create('Cannot load COM16.DLL');
  
  pFunc1 := GetProcAddress16(hInst16, 'DLLMEMW');
  if not Assigned(pFunc1) then
    begin
	 FreeLibrary16(hInst16);
	 raise Exception.Create('Can not get address of DLLMEMW in COM16.DLL');
    end;

  asm
    push seg 
    push off  
    mov edx, pFunc1   
    call QT_Thunk     
    mov Result, ax    
  end;

  FreeLibrary16(hInst16);    
end;

function Mem(seg,off :Word):Byte;
var
  ThunkTrash: array[0..$20] of Word;
  Res :Word;
begin
  ThunkTrash[0] := hInst16;

  hInst16 := LoadLibrary16('COM16.DLL');
  if hInst16 < 32 then
    raise Exception.Create('Cannot load COM16.DLL');
  
  pFunc2 := GetProcAddress16(hInst16, 'DLLMEM');
  if not Assigned(pFunc2) then
    begin
	 FreeLibrary16(hInst16);
	 raise Exception.Create('Can not get address of DLLMEM in COM16.DLL');
    end;

  asm
    push seg 
    push off  
    mov edx, pFunc2   
    call QT_Thunk     
    mov Res, ax    
  end;

  FreeLibrary16(hInst16); 
  Result := Lo(Res);
end;

function ComNum: Byte;
var
  ThunkTrash: array[0..$20] of Word;
  Res :Word;
begin
  ThunkTrash[0] := hInst16;

  hInst16 := LoadLibrary16('COM16.DLL');
  if hInst16 < 32 then
    raise Exception.Create('Cannot load COM16.DLL');
  
  pFunc3 := GetProcAddress16(hInst16, 'CHECKCOM');
  if not Assigned(pFunc3) then
    begin
	 FreeLibrary16(hInst16);
	 raise Exception.Create('Can not get address of CHECKCOM in COM16.DLL');
    end;

  asm
    mov edx, pFunc3   
    call QT_Thunk     
    mov Res, ax    
  end;

  FreeLibrary16(hInst16);   
  Result := Lo(Res);   
end;

end.

