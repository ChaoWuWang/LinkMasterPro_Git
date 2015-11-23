//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ai3032_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformAi3032 *formAi3032;
//---------------------------------------------------------------------------
__fastcall TformAi3032::TformAi3032(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{

	this->api = api;
	api->setCommProperties(this, 510, 240);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-3032_#" +AnsiString(adlinkNo);

	this->Height = 510;
	this->Width = 240;
	this->LbInfo = new TLabel(this);
	this->LbInfo->Parent = this;
	this->LbInfo->Caption = "Voltage Range: -10 (V) -> +10 (V)";
	this->LbInfo->Top = 10;
	this->LbInfo->Left = 10;
	this->LbInfo->Font->Color = clPurple;
	this->LbInfo->Font->Charset = ARABIC_CHARSET;
	this->LbInfo->Font->Size = 10;

	//initial component property
	for(int i = 0; i < AI32_NUM; i++)
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

	api->__ArrangeComponent( this->Lb, COMPONENT_ILABEL, AI32_NUM, 20, 40, 45, 10, true );
	api->__ArrangeComponent( this->Edit, COMPONENT_EDIT, AI32_NUM, 20, 60, 45, 50, true );

}
//---------------------------------------------------------------------------
void __fastcall TformAi3032::FormShow(TObject *Sender)
{
    //updateData();
    //state = 2;
	api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformAi3032::readData()
{
	float VolAi32[AI32_NUM];
	short nErr;

	nErr = api->readVoltage(SlaveNo, Slot, AI32_NUM, _OUTPUT_DATA_ADDRESS_, VolAi32);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Read voltage fail(EPS-3032)");
		return;
	}

	for(int i = 0; i < AI32_NUM; i++)
	{/*
		nErr = api->readSingleRaw(SlaveNo, Slot, i+1, 0x3000, &raw);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "Read analog fail(EPS-3216)");
			return;
		}
		this->Edit[i]->Text = IntToHex((int)raw, 4);*/

		this->Edit[i]->Text = FloatToStrF(VolAi32[i], ffFixed, 4, 3);

	}
}
//---------------------------------------------------------------------------
void __fastcall TformAi3032::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	api->windowsCnt --;
}
//---------------------------------------------------------------------------

