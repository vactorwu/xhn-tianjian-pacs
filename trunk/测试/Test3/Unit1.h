//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtnrs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TOleContainer *OleContainer1;
  TMainMenu *MainMenu1;
  TMenuItem *qwer1;
  TMenuItem *qwer2;
  TMenuItem *qwer3;
  TMenuItem *qwer4;
  TMenuItem *asdf1;
  TMenuItem *asdfasdf1;
  TMenuItem *asdf2;
  TMenuItem *asdf3;
  TMenuItem *asdf4;
  TMenuItem *asdf5;
  TMenuItem *asdfasdf2;
  TMenuItem *zxcv1;
  TMenuItem *asdf6;
  TMenuItem *asdfasdfasdf1;
  TMenuItem *asdfasdf3;
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
