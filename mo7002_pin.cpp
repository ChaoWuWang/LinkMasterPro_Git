//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mo7002_pin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBUILib_OCX"
#pragma resource "*.dfm"
TformMo7002 *formMo7002;
//---------------------------------------------------------------------------
__fastcall TformMo7002::TformMo7002(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int moNum, int slot)
    : TForm(Owner)
{
	this->api = api;
	api->setCommProperties(this, 760, 400);

	static COL_ROW_CONVERT_BIT axis_io_polarity[] =
	{
		{1, 1, 0, 0x01},	//ALM
		{1, 2, 1, 0x01},	//PEL
		{1, 3, 2, 0x01},	//MEL
		{1, 4, 3, 0x01},	//ORG
		{1, 5, 4, 0x01},	//EMG IN
		{1, 6, 5, 0x01},	//EZ
		{1, 7, 6, 0x01},	//INP
		{1, 8, 7, 0x01},	//RDY
		{1, 9, 10, 0x01},	//RST
		{1, 10, 11, 0x01},	//ERC
		{1, 11, 12, 0x01},	//SVON
		{-1, -1, 13, 0x01},	//EMG OUT

		{2, 1, 16, 0x01},	//ALM
		{2, 2, 17, 0x01},	//PEL
		{2, 3, 18, 0x01},	//MEL
		{2, 4, 19, 0x01},	//ORG
		{2, 5, 20, 0x01},	//EMG IN, OUT
		{2, 6, 21, 0x01},	//EZ
		{2, 7, 22, 0x01},	//INP
		{2, 8, 23, 0x01},	//RDY
		{2, 9, 26, 0x01},	//RST
		{2, 10, 27, 0x01},	//ERC
		{2, 11, 28, 0x01},	//SVON
		{-1, -1, 29, 0x01},	//EMG OUT
	};
	p_axis_io_polarity = axis_io_polarity;
	axis_io_polarity_size = sizeof(axis_io_polarity)/sizeof(COL_ROW_CONVERT_BIT);

	static COL_ROW_CONVERT_BIT axis_config[] =
	{
		{1, 12, 0, 0x07},	//encoder mode
		{1, 13, 3, 0x01},	//encoder direction
		{1, 14, 4, 0x01},	//feedback source
		{1, 15, 5, 0x01},	//pulse out direction
		{1, 16, 7, 0x01},	//pulse out mode
		{1, 17, 16, 0x07},	//erc pulse width
		{-1, -1, 20, 0x03},	//erc timer off width sel

		{2, 12, 8, 0x07},	//encoder mode
		{2, 13, 11, 0x01},	//encoder direction
		{2, 14, 12, 0x01},	//feedback source
		{2, 15, 13, 0x01},	//pulse out direction
		{2, 16, 15, 0x01},	//pulse out mode
		{2, 17, 24, 0x07},	//erc pulse width
		{-1, -1, 28, 0x03},	//erc timer off width sel
	};
	p_axis_config = axis_config;
	axis_config_size = sizeof(axis_config)/sizeof(COL_ROW_CONVERT_BIT);

	for(int i = 0; i < INT_SIZE; i++)
	{
		AxisIoPolarity[i] = 0;
		AxisConfig[i] = 0;
	}

	AnsiString signal[MO_NUM] = {"MEL", "PEL", "ORG", "EMG", "ALM", "EZ", "INP", "RDY"};
	AnsiString axisPara[MO_PARA] = {"ALM Logic", "PEL Logic", "MEL Logic", "ORG Logic", "EMG Logic", "EZ Logic", "INP Logic", "RDY Logic",
		"RST Logic", "ERC Logic", "SVON Logic", "Encoder Mode", "Encoder Direction", "Feedback Source", "Pulse Out Direction", "Pulse Out Mode", "ERC Pulse Width"};


	this->SlaveNo = slaveNo;
	this->moNum = moNum;
	this->Slot = slot;
	this->Caption = "EPS-7002_#" +AnsiString(adlinkNo);

	this->edEncoder0->Color = clNone;
	this->edEncoder0->ReadOnly = true;
	this->edEncoder0->Font->Color = clLime;
	this->edEncoder1->Color = clNone;
	this->edEncoder1->ReadOnly = true;
	this->edEncoder1->Font->Color = clLime;

	this->btnSet->OnClick = __OnClick;

	//set StringGrid property
	sgAxisConfig->RowCount = MO_PARA+1;
	sgAxisConfig->ColCount = 3;
	sgAxisConfig->ColWidths[0] = 130;
	sgAxisConfig->ColWidths[1] = 110;
	sgAxisConfig->ColWidths[2] = 110;
	sgAxisConfig->Font->Size = 10;
	
	for(int i = 0; i < MO_PARA; i ++)
	{
		sgAxisConfig->Cells[0][i+1] = axisPara[i];
	}
	sgAxisConfig->Cells[1][0] = "Axis " + AnsiString(moNum*2);
	sgAxisConfig->Cells[2][0] = "Axis " + AnsiString(moNum*2+1);

	this->gbAxisFirst->Caption = "Axis " + AnsiString(moNum*2);
	this->gbAxisSecond->Caption = "Axis " + AnsiString(moNum*2+1);
	
	//set axis combo box contents
	TStringList *configList[MO_PARA];
	for(int i = 0; i < MO_PARA; i ++)
	{
		configList[i] = new TStringList;
	}

	for(int i = 0; i < MO_IO_POLARITY_NUM; i ++)
	{
		if(i == 4)
		{
			configList[i]->Add("Normal Close");
			configList[i]->Add("Normal Open");
		}
		else
		{
			configList[i]->Add("Normal Open");
			configList[i]->Add("Normal Close");
		}
	}
	configList[11]->Add("OUT/DIR");
	configList[11]->Add("CW/CCW");
	configList[11]->Add("1xAB");
	configList[11]->Add("2xAB");
	configList[11]->Add("4xAB");

	configList[12]->Add("Not Inverse");
	configList[12]->Add("Inverse");

	configList[13]->Add("Internal");
	configList[13]->Add("External");

	configList[14]->Add("Not Inverse");
	configList[14]->Add("Inverse");

	configList[15]->Add("OUT/DIR");
	configList[15]->Add("CW/CCW");

	configList[16]->Add("16us");
	configList[16]->Add("106us");
	configList[16]->Add("412us");
	configList[16]->Add("1.6ms");
	configList[16]->Add("13ms");
	configList[16]->Add("52ms");
	configList[16]->Add("104ms");

	for(int i = 0; i < MO_PARA; i ++)
	{
		cbAxisPara[i] = new TComboBox(this);

		cbAxisPara[i]->Parent = this;
		cbAxisPara[i]->Style = csDropDownList;
		cbAxisPara[i]->Visible = false;
		cbAxisPara[i]->Font->Size = 10;
		cbAxisPara[i]->Items->AddStrings(configList[i]);
		cbAxisPara[i]->ItemIndex = 0;
		cbAxisPara[i]->OnSelect = AxisStringGridComboSelect;
	}

	for(int col = 1; col < sgAxisConfig->ColCount; col ++)
	{
		for(int row = 1; row < sgAxisConfig->RowCount; row ++)
		{
			this->sgAxisConfig->Cells[col][row] = this->cbAxisPara[row-1]->Text;
		}
	}

	//axis motion status
	this->stAxis0[0] = this->dbAxis0;
	this->stAxis0[0]->Caption = signal[0];
	this->stAxis0[0]->TopColor = this->dbAxis0->TopColor;
	this->stAxis0[0]->BackColor = this->dbAxis0->BackColor;
	this->stAxis0[0]->Style = 3;
	this->stAxis0[0]->Font->Size = this->dbAxis0->Font->Size;
	this->stAxis0[0]->ControlMode = this->dbAxis0->ControlMode;

	for(int i = 1; i < MO_NUM; i++)
	{
		this->stAxis0[i] = new TDBoolean(this);
		this->stAxis0[i]->Parent = this->gbAxisFirst;
		this->stAxis0[i]->Caption = signal[i];
		this->stAxis0[i]->TopColor = this->stAxis0[0]->TopColor;
		this->stAxis0[i]->BackColor = this->stAxis0[0]->BackColor;
		this->stAxis0[i]->Style = this->stAxis0[0]->Style;
		this->stAxis0[i]->Font->Size = this->stAxis0[0]->Font->Size;
		this->stAxis0[i]->ControlMode = this->stAxis0[0]->ControlMode;
	}
	//api->__ArrangeComponent( this->axis0, COMPONENT_DBOOLEAN, 10, 40, 40, 80, 8, true );
	api->__ArrangeComponent( this->stAxis0, COMPONENT_DBOOLEAN, MO_NUM, 40, 40, 40, 4, false );

	this->stAxis1[0] = this->dbAxis1;
	this->stAxis1[0]->Caption = signal[0];
	this->stAxis1[0]->TopColor = this->dbAxis1->TopColor;
	this->stAxis1[0]->BackColor = this->dbAxis1->BackColor;
	this->stAxis1[0]->Style = 3;
	this->stAxis1[0]->Font->Size = this->dbAxis1->Font->Size;
	this->stAxis1[0]->ControlMode = this->dbAxis1->ControlMode;

	for(int i = 1; i < MO_NUM; i++)
	{
		this->stAxis1[i] = new TDBoolean(this);
		this->stAxis1[i]->Parent = this->gbAxisSecond;
		this->stAxis1[i]->Caption = signal[i];
		this->stAxis1[i]->TopColor = this->stAxis1[0]->TopColor;
		this->stAxis1[i]->BackColor = this->stAxis1[0]->BackColor;
		this->stAxis1[i]->Style = this->stAxis1[0]->Style;
		this->stAxis1[i]->Font->Size = this->stAxis1[0]->Font->Size;
		this->stAxis1[i]->ControlMode = this->stAxis1[0]->ControlMode;
	}
	//api->__ArrangeComponent( this->axis1, COMPONENT_DBOOLEAN, 10, 40, 40, 80, 8, true );
	api->__ArrangeComponent( this->stAxis1, COMPONENT_DBOOLEAN, MO_NUM, 40, 40, 40, 4, false );

	state = 0;
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::FormShow(TObject *Sender)
{
    api->windowsCnt ++;
	state = 0;
}
//---------------------------------------------------------------------------

void __fastcall TformMo7002::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    api->windowsCnt --;    
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::writeData()
{
	UINT ioPolarity = 0, ioPolarityTmp = 0;
	UINT config = 0, configTmp = 0;
	short nErr;

	for(int i = 0; i < INT_SIZE; i ++)
	{
		ioPolarityTmp = AxisIoPolarity[i] << i;
        ioPolarity |= ioPolarityTmp;

		configTmp = AxisConfig[i] << i;
		config |= configTmp;
	}

	nErr = api->writeSpecArea(SlaveNo, moNum, 0x2114, 0x400, ioPolarity);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Write I/O polarity fail");
	}
	nErr = api->writeSpecArea(SlaveNo, moNum, 0x2118, 0x400, config);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Write config fail");
	}
	state = 0;
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::readData()
{
	UINT ioPolarity;
	UINT config;
	short nErr;
	int col, row;

	nErr = api->readSpecArea(SlaveNo, moNum, 0x2114, 0x400, &ioPolarity);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Read I/O polarity fail");
	}
	
	nErr = api->readSpecArea(SlaveNo, moNum, 0x2118, 0x400, &config);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Read config fail");
	}

	for(int i = 0; i < axis_io_polarity_size; i++)
	{
		AxisIoPolarity[p_axis_io_polarity[i].bit] = ioPolarity >> p_axis_io_polarity[i].bit & p_axis_io_polarity[i].bitMap;
		BitIdxToColRowIdx(p_axis_io_polarity, axis_io_polarity_size, p_axis_io_polarity[i].bit, &col, &row);
		if(col == -1 || row == -1)
		{
			continue;
		}
		
		this->cbAxisPara[row-1]->ItemIndex = AxisIoPolarity[p_axis_io_polarity[i].bit];
		this->sgAxisConfig->Cells[col][row] = this->cbAxisPara[row-1]->Text;
	}

	for(int i = 0; i < axis_config_size; i++)
	{
		AxisConfig[p_axis_config[i].bit] = config >> p_axis_config[i].bit & p_axis_config[i].bitMap;
		BitIdxToColRowIdx(p_axis_config, axis_config_size, p_axis_config[i].bit, &col, &row);
		if(col == -1 || row == -1)
		{
			continue;
		}

		this->cbAxisPara[row-1]->ItemIndex = AxisConfig[p_axis_config[i].bit];
		this->sgAxisConfig->Cells[col][row] = this->cbAxisPara[row-1]->Text;
	}

	readByAxis(0x6064, edEncoder0, 0x60FD, stAxis0);
	readByAxis(0x6864, edEncoder1, 0x68FD, stAxis1);
	//readByAxis(0x6064, edEncoder0, 0x60FE, axis0);
	//readByAxis(0x6864, edEncoder1, 0x68FE, axis1);
	//state = 0;
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::readByAxis(UINT posAddr, TEdit *edPos, UINT statusAddr, TDBoolean** dBool)
{
    UINT bit;
    UINT status = 0, pos;
    short nErr;
	nErr = api->readSpecArea(SlaveNo, moNum, posAddr, 0x1000, &pos);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Read position fail");
        return;
    }
	edPos->Text = AnsiString(pos);
	
    nErr = api->readSpecArea(SlaveNo, moNum, statusAddr, 0x1000, &status);
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        api->displayLogMsg(nErr, "Read MO status fail");
        return;
    }

	dBool[0]->Value = status >> 0 & 0x01;	//MEL
	dBool[1]->Value = status >> 1 & 0x01;	//PEL
	dBool[2]->Value = status >> 2 & 0x01;	//ORG
	dBool[3]->Value = status >> 3 & 0x01;	//EMG
	
	dBool[4]->Value = status >> 16 & 0x01;	//ALM
	dBool[5]->Value = status >> 17 & 0x01;	//EZ
	dBool[6]->Value = status >> 18 & 0x01;	//INP
	dBool[7]->Value = status >> 19 & 0x01;	//RDY
	//dBool[8]->Value = status >> 20 & 0x01;	//EA
	//dBool[9]->Value = status >> 21 & 0x01;	//EB
	
}

//---------------------------------------------------------------------------
void __fastcall TformMo7002::__OnClick(TObject *Sender)
{
	state = 2;	//write mode
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::sgAxisConfigMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int ACol = this->sgAxisConfig->Col;
   int ARow = this->sgAxisConfig->Row;

	if(Button == mbLeft)
	{
		if(ACol != 0 && ARow != 0)
		{
			ShowAxisComboBox(ACol , ARow);
			state = 1;
		}
	}    
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::ShowAxisComboBox(int ACol, int ARow)
{
	TRect CellRect = this->sgAxisConfig->CellRect(ACol,ARow);
	int axis, item;
	//int col, row;
	int bit;

	//col = this->sgAxisConfig->Col;
	//row = this->sgAxisConfig->Row;
	//axis = this->sgAxisConfig->Col - 1;
	//item = this->sgAxisConfig->Row - 1;
	axis = ACol - 1;
	item = ARow - 1;
	
	HideAxisComboBox();

	this->cbAxisPara[ARow-1]->Top = CellRect.Top + this->sgAxisConfig->Top+2;
	this->cbAxisPara[ARow-1]->Left = CellRect.Left + this->sgAxisConfig->Left+2;
	this->cbAxisPara[ARow-1]->Height = this->sgAxisConfig->RowHeights[ARow] + 8;
	this->cbAxisPara[ARow-1]->Width = this->sgAxisConfig->ColWidths[ACol];

	//get current item index and display
	if(item < MO_IO_POLARITY_NUM)	//io polarity items
	{
		ColRowIdxToBitIdx(p_axis_io_polarity, axis_io_polarity_size, ACol, ARow, &bit);
		this->cbAxisPara[item]->ItemIndex = AxisIoPolarity[bit];
	}
	else
	{
		ColRowIdxToBitIdx(p_axis_config, axis_config_size, ACol, ARow, &bit);
		this->cbAxisPara[item]->ItemIndex = AxisConfig[bit];
	}

	this->cbAxisPara[ARow-1]->Visible = true;
	this->cbAxisPara[ARow-1]->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::HideAxisComboBox()
{
	for(int i = 0 ; i < MO_PARA ; i++)
	{
		this->cbAxisPara[i]->Visible = false ;
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::AxisStringGridComboSelect(TObject *Sender)
{
	int axis, item;
	int col, row;
	int bit;

	col = this->sgAxisConfig->Col;
	row = this->sgAxisConfig->Row;
	axis = this->sgAxisConfig->Col - 1;
	item = this->sgAxisConfig->Row - 1;

	if(item < MO_IO_POLARITY_NUM)	//io polarity items
	{
		ColRowIdxToBitIdx(p_axis_io_polarity, axis_io_polarity_size, col, row, &bit);
		AxisIoPolarity[bit] = this->cbAxisPara[item]->ItemIndex;
		if(bit == 4 || bit == 20)	//EMG IN
		{
			AxisIoPolarity[bit+9] = this->cbAxisPara[item]->ItemIndex;	//EMG OUT
		}
	}
	else	//config items
	{
		ColRowIdxToBitIdx(p_axis_config, axis_config_size, col, row, &bit);
		AxisConfig[bit] = this->cbAxisPara[item]->ItemIndex;
	}

	this->sgAxisConfig->Cells[col][row] = this->cbAxisPara[item]->Text;

	this->cbAxisPara[item]->Visible = false;
	this->sgAxisConfig->SetFocus();

}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::ColRowIdxToBitIdx(COL_ROW_CONVERT_BIT *p_convert, int size, int col, int row, int *bit)
{
	*bit = -1;
	
	for(int i = 0; i < size; i++)
	{
		if(col == p_convert[i].stringGrid_col && row == p_convert[i].stringGrid_row)
		{
			*bit = p_convert[i].bit;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMo7002::BitIdxToColRowIdx(COL_ROW_CONVERT_BIT *p_convert, int size, int bit, int *col, int *row)
{
	*col = -1;
	*row = -1;
	
	for(int i = 0; i < size; i++)
	{
		if(bit == p_convert[i].bit)
		{
			*col = p_convert[i].stringGrid_col;
			*row = p_convert[i].stringGrid_row;
			break;
		}
	}
}
//---------------------------------------------------------------------------