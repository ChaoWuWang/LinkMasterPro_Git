//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ao4008_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformAo4008 *formAo4008;
//---------------------------------------------------------------------------
__fastcall TformAo4008::TformAo4008(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 310, 290);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-4008_#" +AnsiString(adlinkNo);

	this->LbInfo = new TLabel(this);
	this->LbInfo->Parent = this;
	this->LbInfo->Caption = "Voltage Range: -10 (V) -> +10 (V)";
	this->LbInfo->Top = 10;
	this->LbInfo->Left = 40;
	this->LbInfo->Font->Color = clPurple;
	this->LbInfo->Font->Charset = ARABIC_CHARSET;
	this->LbInfo->Font->Size = 10;
	//this->btnOk->Left = 100;
	//this->btnOk->Top = 240;

	//initial component property
	for(int i = 0; i < AO_NUM; i++)
	{
		this->Lb[i] = new TLabel(this);
		this->ScrBar[i] = new TScrollBar(this);
		this->Edit[i] = new TEdit(this);

		this->Lb[i]->Parent = this;
		this->ScrBar[i]->Parent = this;
		this->Edit[i]->Parent = this;

		this->Lb[i]->Caption = "Ch " + AnsiString(i);
		this->Lb[i]->Font->Size = 10;

		this->ScrBar[i]->Max = 65535;
		this->ScrBar[i]->Min = 1;
		//this->ScrBar[i]->Position = 32767;
		this->ScrBar[i]->OnChange  = __ScrollBar_OnChange;
		this->ScrBar[i]->LargeChange = 1000;
		this->ScrBar[i]->SmallChange = 1;

		this->Edit[i]->Color = clNone;
		this->Edit[i]->ReadOnly = true;
		this->Edit[i]->Font->Color = clLime;
	}

	//initial component position
	//this->Lb[0]->Left = 10;
	//this->Lb[0]->Top = 30;
	//this->ScrBar[0]->Left = 50;
	//this->ScrBar[0]->Top = 30;
	//this->Edit[0]->Left = 200;
	//this->Edit[0]->Top = 30;
	
	api->__ArrangeComponent( this->Lb, COMPONENT_ILABEL, AO_NUM, 20, 40, 30, 10, false );
	api->__ArrangeComponent( this->ScrBar, COMPONENT_SCROBAR, AO_NUM, 20, 140, 30, 50, false );
	api->__ArrangeComponent( this->Edit, COMPONENT_EDIT, AO_NUM, 20, 60, 30, 200, false );

	//isWrite = false;
	state = 0;

}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::btnApplyClick(TObject *Sender)
{
	//writeData();
	//isWrite = true;
	state = 2;
}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::FormShow(TObject *Sender)
{
	//readData();
	state = 0;
	api->windowsCnt ++;
}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::barPosToVol(int i)
{
	float vol;
	USHORT raw = barPosToRawTransfer(this->ScrBar[i]->Position);
	vol = api->rawToVol(raw);
	this->Edit[i]->Text = FloatToStrF(vol, ffFixed, 4, 3);
}
//---------------------------------------------------------------------------
USHORT __fastcall TformAo4008::barPosToRawTransfer(USHORT input)
{
	//do invert on highest bit
	USHORT output;
	output = (1 << 15) + input;
	return output;
}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::__ScrollBar_OnChange( TObject *Sender )
{
    if(Sender == NULL)
    {
        return;
    }
    for( int i = 0; i < AO_NUM; i++ )
    {
        if( this->ScrBar[i] == Sender )
        {
        	barPosToVol(i);
			state = 1;
			//Raw[i] = this->ScrBar[i]->Position;
        }
		else
		{
			continue;
		}
    }
}
//---------------------------------------------------------------------------

void __fastcall TformAo4008::btnResetClick(TObject *Sender)
{
    for(int i = 0; i < AO_NUM; i++)
    {
            this->ScrBar[i]->Position = 32768;
    }
	//writeData();
	//isWrite = true;
	state = 2;
}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::writeData()
{
	short nErr;
	USHORT Raw[AO_NUM];
    for(int i = 0; i < AO_NUM; i++)
    {
		//scroll bar range: 0~65535, need to transfer to raw data
		Raw[i] = barPosToRawTransfer(this->ScrBar[i]->Position);
    }
	nErr = api->writeRaw(SlaveNo, Slot, AO_NUM, _INPUT_DATA_ADDRESS_, Raw);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Write analog fail(EPS-4008)");
	}

	//isWrite = false;
	state = 0;

}
//---------------------------------------------------------------------------

void __fastcall TformAo4008::readData()
{
	short nErr;
	USHORT Raw[AO_NUM];
	nErr = api->readRaw(SlaveNo, Slot, AO_NUM, _INPUT_DATA_ADDRESS_, Raw);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Read analog fail(EPS-4008)");
		return;
	}

	for(int i = 0; i < AO_NUM; i++)
	{
		//scroll bar range: 0~65535, raw data need to transfer
		USHORT tmp = *(Raw + i);
		USHORT pos = barPosToRawTransfer(tmp);
		this->ScrBar[i]->Position = pos;
		barPosToVol(i);
	}

}
//---------------------------------------------------------------------------
void __fastcall TformAo4008::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	api->windowsCnt --;
}
//---------------------------------------------------------------------------

