//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "do2032_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBUILib_OCX"
#pragma resource "*.dfm"
TformDo2032 *formDo2032;
//---------------------------------------------------------------------------
__fastcall TformDo2032::TformDo2032(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot, AnsiString title)
        : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 660, 130);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->title = title;
	this->Caption = title +"_#"+AnsiString(adlinkNo);

	this->dBool[0] = this->DBoolean1;
	this->dBool[0]->Caption = "DO" +AnsiString(0);
	this->dBool[0]->Style = 4;
	this->dBool[0]->OnClick = __OnClick;

	for(int i = 1; i < DO32_NUM; i++)
	{
		this->dBool[i] = new TDBoolean(this);
		this->dBool[i]->Parent = this;
		this->dBool[i]->Caption = "DO" +AnsiString(i);
		this->dBool[i]->TopColor = this->dBool[0]->TopColor;
		this->dBool[i]->BackColor = this->dBool[0]->BackColor;
		this->dBool[i]->Style = this->dBool[0]->Style;
		this->dBool[i]->Font->Size = this->dBool[0]->Font->Size;
		this->dBool[i]->ControlMode = this->dBool[0]->ControlMode;
		this->dBool[i]->OnClick = __OnClick;
	}

	api->__ArrangeComponent( this->dBool, COMPONENT_DBOOLEAN, DO32_NUM, 34, 34, 2, 16, true );
	
	state = 0;

}
//---------------------------------------------------------------------------
void __fastcall TformDo2032::FormShow(TObject *Sender)
{
	//readData();
	//state = 2;
	api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformDo2032::__OnClick(TObject *Sender)
{
	state = 1;
/*
    UINT bit;
    UINT DO32 = 0;
    short nErr;
	for(int i = 0; i < DO32_NUM; i++)
	{
		if(dBool[i] == Sender)
		{
			for(int i = 0; i < DO32_NUM; i++)
			{
				bit = dBool[i]->Value;
				DO32 = DO32 | (bit << i);
			}
			nErr = api->writeDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, DO32);
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
				api->displayLogMsg(nErr, "Write digital fail("+title+")");
			}
		}
		else
		{
			continue;
		}
	}
*/
}
//---------------------------------------------------------------------------
void __fastcall TformDo2032::writeData()
{
    UINT bit;
    UINT DO32 = 0;
    short nErr;

    for(int i = 0; i < DO32_NUM; i++)
    {
        bit = dBool[i]->Value;
		DO32 = DO32 | (bit << i);
    }
    nErr = api->writeDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, DO32);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Write digital fail("+title+")");
    }
	state = 0;
}
//---------------------------------------------------------------------------

void __fastcall TformDo2032::readData()
{
    UINT bit;
    UINT DO32 = 0;
    short nErr;
    nErr = api->readDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, &DO32);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Read digital fail("+title+")");
        return;
    }
    for(int i = 0; i < DO32_NUM; i++)
    {
        bit = DO32 >> i & 0x01;
        dBool[i]->Value = bit;
    }

}
//---------------------------------------------------------------------------

void __fastcall TformDo2032::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	api->windowsCnt --;
}
//---------------------------------------------------------------------------

