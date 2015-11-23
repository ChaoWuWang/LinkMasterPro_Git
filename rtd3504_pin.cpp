//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "rtd3504_pin.h"
#include "RTD_Pt100.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformRtd3504 *formRtd3504;
//---------------------------------------------------------------------------
__fastcall TformRtd3504::TformRtd3504(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot)
        : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 220, 270);

	this->SlaveNo = slaveNo;
	this->Slot = slot;
	this->Caption = "EPS-3504_#" +AnsiString(adlinkNo);


	this->LbInfo = new TLabel(this);
	this->LbInfo->Parent = this;
	//this->LbInfo->Caption = "Resistance Range:\n0(ohm) -> 4300(ohm)";
	this->LbInfo->Caption = AnsiString("Resistance(£[)      Temperature(¢XC)");
	this->LbInfo->Top = 45;
	this->LbInfo->Left = 50;
	this->LbInfo->Font->Color = clPurple;
	this->LbInfo->Font->Charset = CHINESEBIG5_CHARSET;
	this->LbInfo->Font->Size = 10;

	this->LbIndex = new TLabel(this);
	this->LbIndex->Parent = this;
	this->LbIndex->Caption = "RTD Type";
	this->LbIndex->Top = 15;
	this->LbIndex->Left = 10;
	this->LbIndex->Font->Color = clPurple;
	this->LbIndex->Font->Charset = CHINESEBIG5_CHARSET;
	this->LbIndex->Font->Size = 10;

	this->CbIndex = new TComboBox(this);
	this->CbIndex->Parent = this;
	this->CbIndex->Top = 10;
	this->CbIndex->Left = 80;
    this->CbIndex->Style = csDropDownList;
    this->CbIndex->Items->Add("Pt 100");
    this->CbIndex->Items->Add("Pt 500");
    this->CbIndex->Items->Add("Pt 1000");
    this->CbIndex->ItemIndex = 0;
    this->CbIndex->Width = 160;

	//initial component property
	for(int i = 0; i < RTD_NUM; i++)
	{
		this->Lb[i] = new TLabel(this);
		this->EditOhm[i] = new TEdit(this);
		this->EditTemp[i] = new TEdit(this);

		this->Lb[i]->Parent = this;
		this->EditOhm[i]->Parent = this;
		this->EditTemp[i]->Parent = this;
/*
		if(i == 0)
		{
			this->Lb[i]->Caption = "Data";
		}
		else
		{
			this->Lb[i]->Caption = "Ch " + AnsiString(i-1);
		}
		*/
		this->Lb[i]->Caption = "Ch " + AnsiString(i);
		this->Lb[i]->Font->Size = 10;

		this->EditOhm[i]->Color = clNone;
		this->EditTemp[i]->Color = clNone;
		this->EditOhm[i]->ReadOnly = true;
		this->EditTemp[i]->ReadOnly = true;
		this->EditOhm[i]->Font->Color = clLime;
		this->EditTemp[i]->Font->Color = clLime;
	}

	//initial component position
	this->Lb[0]->Left = 10;
	this->Lb[0]->Top = 50;
	this->EditOhm[0]->Left = 50;
	this->EditOhm[0]->Top = 50;
	this->EditTemp[0]->Left = 120;
	this->EditTemp[0]->Top = 50;

	api->__ArrangeComponent( this->Lb, COMPONENT_ILABEL, RTD_NUM, 20, 40, 70, 10, false );
	api->__ArrangeComponent( this->EditOhm, COMPONENT_EDIT, RTD_NUM, 20, 70, 70, 50, false );
	api->__ArrangeComponent( this->EditTemp, COMPONENT_EDIT, RTD_NUM, 20, 100, 70, 140, false );

}
//---------------------------------------------------------------------------

void __fastcall TformRtd3504::FormShow(TObject *Sender)
{
    //updateData();
    api->windowsCnt ++;
}
//---------------------------------------------------------------------------

void __fastcall TformRtd3504::readData()
{
	short nErr;
	UINT raw;
	USHORT wire;
	ULONG len;
	double thermo, thermo_tmp;

	for(int i = 0; i < RTD_NUM; i++)
	{
		nErr = api->readSingleRaw(SlaveNo, Slot, i+1, _OUTPUT_DATA_ADDRESS_, &raw);
		//nErr = api->writeWire(SlaveNo, Slot, i+3, OD_BASE_INDEX_MODULE_SETTINGS, 4);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "Read ch "+AnsiString(i)+" ohm fail(EPS-3504)");
		}
		else
		{
			nErr = api->readWire(SlaveNo, Slot, i+3, OD_BASE_INDEX_MODULE_SETTINGS, &wire);
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
				api->displayLogMsg(nErr, "Read ch "+AnsiString(i)+" wire fail(EPS-3504)");
			}
			else
			{
				this->EditOhm[i]->Text = FloatToStrF(api->rawToOhm(raw, wire), ffFixed, 4, 2);

				nErr = ADC_to_Thermo(api->rawToOhm(raw, wire), &thermo_tmp);
				if(nErr == NoThermoError)
				{
					switch(this->CbIndex->ItemIndex)
					{
						case 0:
							thermo = thermo_tmp;
							break;
						case 1:
							thermo = thermo_tmp * 5;
							break;
						case 2:
							thermo = thermo_tmp * 10;
							break;
						default:
							break;
					}
					this->EditTemp[i]->Text = FloatToStrF(thermo, ffFixed, 4, 2);
				}
				else if(nErr == ErrorOutThermoRange)
				{
					this->EditTemp[i]->Text = "Out of range";
					//api->displayLogMsg(nErr, "Temperature out of range");
				}
				else
				{
					this->EditTemp[i]->Text = "Temp. transfer fail";
					//api->displayLogMsg(nErr, "Ohm transfer to thermo fail");
				}
			}

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TformRtd3504::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        api->windowsCnt --;
}
//---------------------------------------------------------------------------
short __fastcall TformRtd3504::ADC_to_Thermo( double fScaledADC, double* pfTemp )
{

	double* fData;
	double fMax, fMin, fTemp;

	unsigned int dwStart, dwEnd, dwMiddle, dwArraySize;
	int dwIndex;


	fMax = Usb_RTDPt100_Max; 
	fMin = Usb_RTDPt100_Min; 
	dwStart = 0;
	dwEnd = (Usb_RTDPt100_Length-1);
	fData = Usb_RTDPt100;

	if( ( fScaledADC < fData[dwStart] ) || ( fScaledADC > fData[dwEnd] ) )
	{
		return ErrorOutThermoRange;
	}


	dwArraySize = dwEnd;
	dwMiddle = (dwStart + dwEnd)/2;
	dwIndex = -1;

	while(true)
	{
		if( fData[dwMiddle] > fScaledADC )
		{
			dwEnd = (dwMiddle - 1);
		}
		else if( fData[dwMiddle] < fScaledADC )
		{
	        dwStart = (dwMiddle + 1);
		}
		else if( fData[dwMiddle] == fScaledADC )
		{
	        dwIndex = dwMiddle;
	        break;
		}

		if( ((dwStart + 1) == dwEnd) || ( dwStart == dwEnd ) )
		{
	        dwIndex = dwStart;
	        break;
		}

		if( dwStart > dwEnd )
		{
	        dwIndex = -1;
	        break;
		}

		dwMiddle = (dwStart + dwEnd)/2;
	}

	if( dwIndex >= 0 )
	{
		fTemp = ((double)dwIndex + fMin);
	//                   fTemp =  (fTemp + fColdJuncTemp );

		*pfTemp = fTemp;

	}
	else
	{
		return ErrorThermoTable;
	}

	return NoThermoError;
}

