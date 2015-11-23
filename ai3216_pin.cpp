//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ai3216_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformAi3216 *formAi3216;
//---------------------------------------------------------------------------
__fastcall TformAi3216::TformAi3216(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{

	this->api = api;
	api->setCommProperties(this, 520, 150);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-3216_#" +AnsiString(adlinkNo);

	
	//timer_eps3216->Enabled = false;
	//timer_eps3216->Interval = COE_UPDATE_INTERVAL;
	
	this->LbInfo = new TLabel(this);
	this->LbInfo->Parent = this;
	this->LbInfo->Caption = "Current Range:\n 0 (mA) -> 20 (mA)";
	this->LbInfo->Top = 10;
	this->LbInfo->Left = 10;
	this->LbInfo->Font->Color = clPurple;
	this->LbInfo->Font->Charset = ARABIC_CHARSET;
	this->LbInfo->Font->Size = 10;

	//initial component property
	for(int i = 0; i < AI16_NUM; i++)
	{
		this->Lb[i] = new TLabel(this);
		this->Edit[i] = new TEdit(this);

		this->Lb[i]->Parent = this;
		this->Edit[i]->Parent = this;

		this->Lb[i]->Caption = "Ch " + AnsiString(i);
		this->Lb[i]->Font->Size = 10;

		this->Edit[i]->Color = clNone;
		this->Edit[i]->ReadOnly = true;
		this->Edit[i]->Font->Color = clLime;
	}

	//initial component position
	//this->Lb[0]->Left = 10;
	//this->Lb[0]->Top = 45;
	//this->Edit[0]->Left = 50;
	//this->Edit[0]->Top = 45;

	api->__ArrangeComponent( this->Lb, COMPONENT_ILABEL, AI16_NUM, 20, 40, 45, 10, false );
	api->__ArrangeComponent( this->Edit, COMPONENT_EDIT, AI16_NUM, 20, 60, 45, 50, false );

}
//---------------------------------------------------------------------------
void __fastcall TformAi3216::FormShow(TObject *Sender)
{
    //updateData();
    //state = 2;
    api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformAi3216::readData()
{
	float CurrentAi16[AI16_NUM];
	short nErr;

	nErr = api->readCurrent(SlaveNo, Slot, AI16_NUM, _OUTPUT_DATA_ADDRESS_, CurrentAi16);
	//nErr = api->readRaw(SlaveNo,Slot, AI16_NUM, _OUTPUT_DATA_ADDRESS_, raw);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Read current fail(EPS-3216)");
		return;
	}

	for(int i = 0; i < AI16_NUM; i++)
	{
		this->Edit[i]->Text = FloatToStrF(CurrentAi16[i], ffFixed, 4, 3);
	}
}
//---------------------------------------------------------------------------
void __fastcall TformAi3216::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        //state = 0;
		api->windowsCnt --;
}
//---------------------------------------------------------------------------

