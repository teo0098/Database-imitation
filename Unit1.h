//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TDB : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel10;
	TStringGrid *Table;
	TGroupBox *GroupBox1;
	TEdit *Title;
	TEdit *ReleaseYear;
	TEdit *Amount;
	TCheckBox *Rewarded;
	TEdit *Price;
	TEdit *Rating;
	TEdit *Description;
	TButton *Add;
	TGroupBox *GroupBox2;
	TEdit *IDD;
	TButton *Delete;
	TButton *DeleteAll;
	TGroupBox *GroupBox3;
	TButton *Export;
	TButton *Import;
	TEdit *Error;
	TGroupBox *GroupBox4;
	TEdit *Drawing;
	TButton *Draw;
	TComboBox *Performer;
	TButton *Search;
	TComboBox *Searchings;
	TButton *Back;
	TButton *DeletePer;
	TButton *BSearch;
	TButton *MostR;
	TButton *Inverted;
	TButton *ChainList;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall DeleteAllClick(TObject *Sender);
	void __fastcall ImportClick(TObject *Sender);
	void __fastcall ExportClick(TObject *Sender);
	void __fastcall DrawClick(TObject *Sender);
	void __fastcall SearchClick(TObject *Sender);
	void __fastcall BackClick(TObject *Sender);
	void __fastcall DeletePerClick(TObject *Sender);
	void __fastcall BSearchClick(TObject *Sender);
	void __fastcall MostRClick(TObject *Sender);
	void __fastcall InvertedClick(TObject *Sender);
	void __fastcall ChainListClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDB(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDB *DB;
//---------------------------------------------------------------------------
#endif
