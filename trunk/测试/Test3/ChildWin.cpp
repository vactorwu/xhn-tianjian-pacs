//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
//--------------------------------------------------------------------- 
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
}
//--------------------------------------------------------------------- 
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------

void __fastcall TMDIChild::Button1Click(TObject *Sender)
{
  (OleContainer1->OleObject).Exec(Procedure("Open") << "C:\\WINNT\\CIBA1B.bmp");
}
//---------------------------------------------------------------------------

