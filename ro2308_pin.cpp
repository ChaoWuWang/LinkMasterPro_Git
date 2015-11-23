//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ro2308_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBUILib_OCX"
#pragma resource "*.dfm"
TformRo2308 *formRo2308;
//---------------------------------------------------------------------------
__fastcall TformRo2308::TformRo2308(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 430, 140);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-2308_#" +AnsiString(adlinkNo);

	this->dBool[0] = this->DBoolean1;
	this->dBool[0]->Caption = "RO" +AnsiString(0);
	this->dBool[0]->TopColor = this->dBool[0]->TopColor;
	this->dBool[0]->BackColor = this->dBool[0]->BackColor;
	this->dBool[0]->Style = this->dBool[0]->Style;
	this->dBool[0]->Font->Size = this->dBool[0]->Font->Size;
	this->dBool[0]->ControlMode = this->dBool[0]->ControlMode;
	this->dBool[0]->OnClick = __OnClick;

	for(int i = 1; i < RO_NUM; i++)
	{
		this->dBool[i] = new TDBoolean(this);
		this->dBool[i]->Parent = this;
		this->dBool[i]->Caption = "RO" +AnsiString(i);
		this->dBool[i]->TopColor = this->dBool[0]->TopColor;
		this->dBool[i]->BackColor = this->dBool[0]->BackColor;
		this->dBool[i]->Style = this->dBool[0]->Style;
		this->dBool[i]->Font->Size = this->dBool[0]->Font->Size;
		this->dBool[i]->ControlMode = this->dBool[0]->ControlMode;
		this->dBool[i]->OnClick = __OnClick;
	}

	api->__ArrangeComponent( this->dBool, COMPONENT_SWITCH, RO_NUM, 40, 40, 8, 40, false );

	//isWrite = false;
	state = 0;

}
//---------------------------------------------------------------------------

void __fastcall TformRo2308::FormShow(TObject *Sender)
{
        //readData();
        //state = 2;
		api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformRo2308::__OnClick(TObject *Sender)
{
	//isWrite = true;
	state = 1;
/*    UINT bit;
    UINT RO = 0;
    short nErr;
	for(int i = 0; i < RO_NUM; i++)
	{
		if(dBool[i] == Sender)
		{
			for(int i = 0; i < RO_NUM; i++)
			{
				bit = dBool[i]->Value;
				RO = RO | (bit << i);
			}
			nErr = api->writeDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, RO);
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
				api->displayLogMsg(nErr, "Write RO fail(EPS-2308)");
			}
			else
			{
				//readState();
			}
		}
		else
		{
			continue;
		}
	}*/

}

//---------------------------------------------------------------------------
void __fastcall TformRo2308::writeData()
{
    UINT bit;
    UINT RO = 0;
    short nErr;

	for(int i = 0; i< RO_NUM; i++)
	{
		bit = dBool[i]->Value;
		RO = RO | (bit << i);
	}
	nErr = api->writeDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, RO);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Write RO fail(EPS-2308)");
	}

	//isWrite = false;
	state = 0;
}
//---------------------------------------------------------------------------

void __fastcall TformRo2308::readData()
{
    UINT bit;
    UINT RO = 0;
    short nErr;
    nErr = api->readDigital(SlaveNo, Slot, _INPUT_DATA_ADDRESS_, &RO);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Read RO fail(EPS-2308)");
        return;
    }
    for(int i = 0; i < RO_NUM; i++)
    {
        bit = RO >> i & 0x01;
        dBool[i]->Value = bit;
    }

}

//---------------------------------------------------------------------------

void __fastcall TformRo2308::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        //state = 0;        
        api->windowsCnt --;
}
//---------------------------------------------------------------------------

