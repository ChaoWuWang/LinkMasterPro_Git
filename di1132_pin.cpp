//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "di1132_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBUILib_OCX"
#pragma resource "*.dfm"
TformDi1132 *formDi1132;
//---------------------------------------------------------------------------
__fastcall TformDi1132::TformDi1132(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 660, 130);
	
	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-1132_#" +AnsiString(adlinkNo);

	//timer_eps1132->Enabled = false;
	//timer_eps1132->Interval = COE_UPDATE_INTERVAL;

	this->dBool[0] = this->DBoolean1;
	this->dBool[0]->Caption = "DI" +AnsiString(0);
	this->dBool[0]->Style = 3;

	for(int i = 1; i < DI32_NUM; i++)
	{
		this->dBool[i] = new TDBoolean(this);
		this->dBool[i]->Parent = this;
		this->dBool[i]->Caption = "DI" +AnsiString(i);
		this->dBool[i]->TopColor = this->dBool[0]->TopColor;
		this->dBool[i]->BackColor = this->dBool[0]->BackColor;
		this->dBool[i]->Style = this->dBool[0]->Style;
		this->dBool[i]->Font->Size = this->dBool[0]->Font->Size;
		this->dBool[i]->ControlMode = this->dBool[0]->ControlMode;
	}
	
	api->__ArrangeComponent( this->dBool, COMPONENT_DBOOLEAN, DI32_NUM, 34, 34, 2, 16, true );

}
//---------------------------------------------------------------------------
void __fastcall TformDi1132::FormShow(TObject *Sender)
{
    //updateData();
    //state = 2;
    api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformDi1132::readData()
{
    UINT bit;
    UINT DI32 = 0;
    short nErr;
    nErr = api->readDigital(SlaveNo, Slot, _OUTPUT_DATA_ADDRESS_, &DI32);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Read digital fail(EPS-1132)");
        return;
    }
    for(int i = 0; i < DI32_NUM; i++)
    {
        bit = DI32 >> i & 0x01;
        dBool[i]->Value = bit;
    }
}
//---------------------------------------------------------------------------
void __fastcall TformDi1132::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        //state = 0;        
        api->windowsCnt --;
}
//---------------------------------------------------------------------------

