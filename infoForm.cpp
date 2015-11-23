//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "infoForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformInfo *formInfo;
//---------------------------------------------------------------------------
__fastcall TformInfo::TformInfo(TComponent* Owner)
        : TForm(Owner)
{
	AnsiString systemPath;
	systemPath = ExtractFilePath(Application->ExeName);
    imgLogo->Picture->LoadFromFile(systemPath+"\\icon\\ADLINK_Inc.jpg");
    Label1->Caption = "LinkMasterPro Version:" + AnsiString(LINKMASTERPRO_VER);
    Label2->Caption = "Copyright ADLINK Technology Inc.";
}
//---------------------------------------------------------------------------
void __fastcall TformInfo::btnOkClick(TObject *Sender)
{
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TformInfo::FormClose(TObject *Sender, TCloseAction &Action)
{
    this->Hide();
}
//---------------------------------------------------------------------------
