//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <stdio.h>


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  for(int i = 0 ; i < 256 ; i++)
    sg_Rtl->Cells[0][i] = i;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToolButton1Click(TObject *Sender)
{
  if (dlg_OpenFile->Execute())
  {
    for (int I = 0; I < dlg_OpenFile->Files->Count; I ++)
    {
      FILE *stream;
      stream = fopen(dlg_OpenFile->Files->Strings[I].c_str(), "rb");

      sg_Rtl->Cells[sg_Rtl->ColCount-1][256] = "0";
      if (stream)
      {
        while(!feof(stream))
        {
          unsigned char cCur;
          fread(&cCur,1,1,stream);
          int nLine = cCur;
          if (sg_Rtl->Cells[sg_Rtl->ColCount-1][nLine] == "")
            sg_Rtl->Cells[sg_Rtl->ColCount-1][nLine] = "0";
          sg_Rtl->Cells[sg_Rtl->ColCount-1][nLine] = sg_Rtl->Cells[sg_Rtl->ColCount-1][nLine].ToInt() + 1;
          sg_Rtl->Cells[sg_Rtl->ColCount-1][256] = sg_Rtl->Cells[sg_Rtl->ColCount-1][256].ToInt() + 1;

        }
        fclose(stream);
      }
      sg_Rtl->ColCount++;
    }
  }
}
//---------------------------------------------------------------------------
