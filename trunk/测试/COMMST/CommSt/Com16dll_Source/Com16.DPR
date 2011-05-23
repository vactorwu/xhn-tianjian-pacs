Library COM16;


function GetComAvailable: byte; assembler;
const
  BiosSeg = $0040;
  DPMIInt = $31;
  SegSel = $02;
  Com1Off = 0;
  Com2Off = 2;
  Com3Off = 4;
  Com4Off = 6;

asm
  push es                     
  mov ax,SegSel               
  mov bx,BiosSeg              
  int DPMIInt                 
  mov dx,0                   
  jc  @Error                  
  mov es,ax                  
  mov di,0                    
  cmp di,es:[di+Com4Off]      
  rcl dx,1                    
  cmp di,es:[di+Com3Off]      
  rcl dx,1                    
  cmp di,es:[di+Com2Off]      
  rcl dx,1                    
  cmp di,es:[di+Com1Off]      
  rcl dx,1                    
@Error:
  mov ax,dx                  
  pop es                      
end;

function CheckCom : Byte; export;
var
  ComAvail: byte;
begin
  ComAvail := GetComAvailable;
  Result := ComAvail;
end;

function DllMem(seg:Word;off:Word) : Byte; export;
begin
     Result := Mem[seg:off];
end;

function DllMemW(seg:Word;off:Word) : word; export;
begin
     Result := MemW[seg:off];
end;

exports
 CheckCom index 1 name 'CHECKCOM',
 DllMem   index 2 name 'DLLMEM',
 DllMemW  index 3 name 'DLLMEMW';

Begin
 
End.
