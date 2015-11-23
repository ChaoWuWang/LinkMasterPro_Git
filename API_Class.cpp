//---------------------------------------------------------------------------
//for general function

#pragma hdrstop

#include "API_Class.h"
#pragma package(smart_init)
#pragma link "DBUILib_OCX"


//---------------------------------------------------------------------------
__fastcall API_Class::API_Class(TRichEdit *edErr)
{
	this->richEdErr = edErr;
	this->windowsCnt = 0;
}
//---------------------------------------------------------------------------
void __fastcall API_Class::delay(unsigned long iMilliSeconds)
{
  unsigned long time;

  time = GetTickCount();
  while (GetTickCount() - time < iMilliSeconds)
  {
  	if((GetTickCount() - time) <= 0 )
  	{
  		time = GetTickCount();
  	}
  }
    //Application->ProcessMessages();
}
//---------------------------------------------------------------------------
void __fastcall API_Class::displayMark(AnsiString mark)
{
	this->richEdErr->Lines->Append(mark);
}
//---------------------------------------------------------------------------
void __fastcall API_Class::displayLogMsg(short err, AnsiString msg)
{
	long ecErr;
	//time_t raw_time;
	AnsiString datetime;
	USHORT year, month, day, hour, min, sec, msec;

	TDateTime now = TDateTime::CurrentDateTime();
	now.DecodeDate(&year, &month, &day);
	now.DecodeTime(&hour, &min, &sec, &msec);
	datetime = now.DateTimeString();

	if(err == ECMaster_ERROR_SUCCESS)
	{
		this->richEdErr->Lines->Append(datetime +" "+ msg);
	}
	else if(err == ECMaster_ERROR_EXTERNAL_ERROR)	//no error code
	{
		this->richEdErr->Lines->Append(datetime+" [Error] " + msg);
	}
	else if(err == ECMaster_ERROR_INTERNAL_ERROR)
	{
		LastEcError_Get(&ecErr);
		this->richEdErr->Lines->Append(datetime+" [Error] 0x" + IntToHex((int)ecErr, 4)+": "+ msg);
	}
	else
	{
		this->richEdErr->Lines->Append(datetime+" [Error] " + AnsiString(err)+": "+ msg);
	}
}
//---------------------------------------------------------------------------
void __fastcall API_Class::clearErrMsg()
{
	this->richEdErr->Clear();
}
//---------------------------------------------------------------------------
void __fastcall API_Class::displayPrg(TForm *frm, TProgressBar *prgBar, AnsiString title, int percent, int no, UINT total)
{
	int currPrg = (float)(percent +100*no)/total * 100;
	//frm->Show();
	prgBar->Max = total;
	prgBar->Position = percent +100*no;
	//frm->FormStyle = fsStayOnTop;
	frm->Caption = title + AnsiString(currPrg) + "%";
	if(prgBar->Position >= prgBar->Max)
	{
		prgBar->Position = 0;
		frm->Caption = title + AnsiString(prgBar->Position) + "%";
		frm->Hide();
	}

}
//---------------------------------------------------------------------------
void __fastcall API_Class::setCommProperties(TForm *frm, int height, int width)
{
    frm->BorderStyle = bsDialog;
    frm->FormStyle = fsStayOnTop;
    //frm->FormStyle = fsMDIChild;
    //frm->FormStyle = fsNormal;
    //frm->FormStyle = fsMDIForm;

    //frm->Height = height;
    //frm->Width = width;
    frm->SetBounds(20, 20, width, height);
	frm->Position = poMainFormCenter;
}

//---------------------------------------------------------------------------
short __fastcall API_Class::writeSpecArea(USHORT wSlaveNo, int slot , UINT baseAddr, UINT area, UINT digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), baseAddr+slot*area, 0, &digital);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readSpecArea(USHORT wSlaveNo, int slot, UINT baseAddr, UINT area, UINT *digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), baseAddr+slot*area, 0, digital, &dwDataLen);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::writeMoDigital(USHORT wSlaveNo, int moNum, UINT baseAddr, UINT digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), baseAddr+moNum*CONFIGURATION_MO_AREA, 0, &digital);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readMoDigital(USHORT wSlaveNo, int moNum, UINT baseAddr, UINT *digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), baseAddr+moNum*CONFIGURATION_MO_AREA, 0, digital, &dwDataLen);
	return nErr;
}
//---------------------------------------------------------------------------

short __fastcall API_Class::writeDigital(USHORT wSlaveNo, int slot, UINT baseAddr, UINT digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, 0, &digital);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readDigital(USHORT wSlaveNo, int slot, UINT baseAddr, UINT *digital)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, 0, digital, &dwDataLen);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::writeVoltage(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *vol)
{
	short nErr = 0;
	USHORT analog;
	for(int i = 0; i < subTotal; i++)
	{
		analog = volToRaw(*(vol+i));
		nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(USHORT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, i+1, &analog);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			return nErr;
		}

	}
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readVoltage(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *vol)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	USHORT raw;
	for(int i = 0; i < subTotal; i++)
	{
		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(USHORT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, i+1, &raw, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			return nErr;
		}
		*(vol+i) = rawToVol(raw);

	}

	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readCurrent(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *current)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	USHORT raw;
	for(int i = 0; i < subTotal; i++)
	{
		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(USHORT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, i+1, &raw, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			return nErr;
		}
		*(current+i) = rawToCurrent(raw)*1000;	//to mA

	}

	return nErr;

}
//---------------------------------------------------------------------------
short __fastcall API_Class::writeRaw(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, USHORT *raw)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	USHORT tmp;
	for(int i = 0; i < subTotal; i++)
	{
		tmp = *(raw+i);
		nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(USHORT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, i+1, &tmp);
		//LastEcError_Get(&err);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			return nErr;
		}

	}
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::readRaw(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, USHORT *raw)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	USHORT tmp;
	for(int i = 0; i < subTotal; i++)
	{
		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(USHORT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, i+1, &tmp, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			return nErr;
		}
		*(raw+i) = tmp;

	}

	return nErr;

}
//---------------------------------------------------------------------------
short __fastcall API_Class::writeSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, UINT raw)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(UINT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, subIdx, &raw);
	return nErr;

}
//---------------------------------------------------------------------------
short __fastcall API_Class::readSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, UINT *raw)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(UINT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, subIdx, raw, &dwDataLen);
	return nErr;
}
//---------------------------------------------------------------------------
short __fastcall API_Class::writeWire(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT wire)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(UINT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, subIdx, &wire);
	return nErr;

}
//---------------------------------------------------------------------------
short __fastcall API_Class::readWire(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT *wire)
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG dwDataLen;
	nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(UINT), baseAddr+slot*CONFIGURATION_OBJECTS_AREA, subIdx, wire, &dwDataLen);
	return nErr;

}
//---------------------------------------------------------------------------
/*
//raw-->scroll bar : do inverse on highest bit
raw			scroll bar			vol
0x7FFF		0xFFFF			10
...			...				...
0x0001		0x8001			0.0003...
0x0000		0x8000			0
0xFFFF		0x7FFF			-0.0003...
...			...				...
0x8000		0x0000			-10
*/
float __fastcall API_Class::rawToVol(USHORT raw)
{
	float vol;
	USHORT tmp;
	tmp = (1 << 15) + raw;
	if(tmp > 32768)
	{
		vol = (float)(tmp - 32768)*10/32767;
	}
	else
	{
		vol = (float)(tmp - 32768)*10/32768;
	}
	
	return vol;
}
//---------------------------------------------------------------------------
USHORT __fastcall API_Class::volToRaw(float vol)
{
	USHORT raw;
	USHORT tmp;
	if(vol > 0)
	{
		tmp = vol * 32767/10 + 32768;
	}
	else
	{
		tmp = vol * 32768/10 + 32768;
	}
	raw = (1 << 15) + tmp;
	
	return raw;
}
//---------------------------------------------------------------------------
float __fastcall API_Class::rawToOhm(UINT raw, USHORT wire)
{
	float ohm;
	if(wire == 3)
	{
		ohm = (float)raw / (pow(2, 23)-1) * 4300 * 2;
	}
	else
	{
		ohm = (float)raw / (pow(2, 23)-1) * 4300;
	}
	
	return ohm;
}
//---------------------------------------------------------------------------
UINT __fastcall API_Class::ohmToRaw(float ohm)
{
	UINT raw = (pow(2, 23)-1) * ohm / 4300;
	return raw;
}
//---------------------------------------------------------------------------
float __fastcall API_Class::rawToCurrent(USHORT raw)
{
	float current;	//ohm is 249
	USHORT tmp;
	tmp = (1 << 15) + raw;
	if(tmp > 32768)
	{
		current = (float)(tmp - 32768)*10/32767/249;
	}
	else
	{
		current = (float)(tmp - 32768)*10/32768/249;
	}

	return current;
}
//---------------------------------------------------------------------------
USHORT __fastcall API_Class::CurrentToRaw(float current)
{
	USHORT raw;
	USHORT tmp;
	if(current > 0)
	{
		tmp = current * 32767*249/10 + 32768;
	}
	else
	{
		tmp = current * 32768*249/10 + 32768;
	}
	raw = (1 << 15) + tmp;
	
	return raw;

}
//---------------------------------------------------------------------------
ULONG __fastcall API_Class::convertEndian(ULONG num)
{
	ULONG num_convert = 0;
	BYTE element[4];
	for(int i = 0; i < 4; i ++)
	{
		element[i] = num >> (i*8) & 0xFF;
	}
	for(int i = 0; i < 4; i ++)
	{
		num_convert = element[3-i] << (i*8) | num_convert;
	}
	return num_convert;
}
//---------------------------------------------------------------------------

static
template < typename T >
void __template_func( T** Object, int Type, int Count, int height, int width, int top, int left, bool is2Col )
{
    switch( Type )
    {
    case COMPONENT_EDIT:
		//if(Count > 16)
		if(is2Col)
		{
			for( int i = 0; i < Count; ++i )
			{
				if(i%2 == 0)
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					//Object[i]->Left   = Object[0]->Left;
					Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					Object[i]->Left   = left;
				}
				else
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					//Object[i]->Left   = Object[0]->Left+100;
					Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					Object[i]->Left   = left+100;
				}
				
				Object[i]->Height = height;//20;
				Object[i]->Width  = width;//50;

			}
		}
		else
		{
			for( int i = 0; i < Count; ++i )
			{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				//Object[i]->Left   = Object[0]->Left;
				Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				Object[i]->Left   = left;
				Object[i]->Height = height;//20
				Object[i]->Width  = width;//60
			}
		}

        break;
    case COMPONENT_ILABEL:
		//if(Count > 16)
		if(is2Col)
		{
			for( int i = 0; i < Count; ++i )
			{
				if(i%2 == 0)
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					//Object[i]->Left   = Object[0]->Left;
					Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					Object[i]->Left   = left;
				}
				else
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					//Object[i]->Left   = Object[0]->Left+100;
					Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					Object[i]->Left   = left+100;
				}
				
				Object[i]->Height = height;//20;
				Object[i]->Width  = width;//50;

			}
		}
		else
		{
			for( int i = 0; i < Count; ++i )
			{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				//Object[i]->Left   = Object[0]->Left;
				Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				Object[i]->Left   = left;
				Object[i]->Height = height;//20
				Object[i]->Width  = width;//40
			}
		}

        break;
    case COMPONENT_SCROBAR:
        for( int i = 0; i < Count; ++i )
        {
            //Object[i]->Top    = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
            //Object[i]->Left   = Object[0]->Left;
            Object[i]->Top    = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
			Object[i]->Left   = left;
            Object[i]->Height = height;//20;
            Object[i]->Width  = width;//140;
        }
		break;
    case COMPONENT_BUTTON:
		break;
	case COMPONENT_CHECKBOX:
        for( int i = 0; i < Count; ++i )
        {
        	if(i%2 == 0)
        	{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
				//Object[i]->Left   = Object[0]->Left;
				Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
				Object[i]->Left   = left;
        	}
			else
			{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
				//Object[i]->Left   = Object[0]->Left+50;
				Object[i]->Top	  = top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
				Object[i]->Left   = left+50;
			}

            Object[i]->Height = height;//20;
            Object[i]->Width  = width;//50;
        }
		break;
	case COMPONENT_DBOOLEAN:
		if(is2Col)
		{
			for( int i = 0; i < Count; ++i )
			{
				if(i%2 == 0)
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					//Object[i]->Left   = Object[0]->Left;
					Object[i]->Top	  = top + ( height + HEIGHT_INTERNAL ) * i/2;
					Object[i]->Left   = left;
				}
				else
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					//Object[i]->Left   = Object[0]->Left+50;
					Object[i]->Top	  = top + ( height + HEIGHT_INTERNAL ) * (i-1)/2;
					Object[i]->Left   = left+50;
				}
			
				Object[i]->Height = height;//40;
				Object[i]->Width  = width;//40;
			}

		}
		else
		{
			for( int i = 0; i < Count; ++i )
			{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				//Object[i]->Left   = Object[0]->Left;
				Object[i]->Top	  = top;
				Object[i]->Left   = left  + ( width + HEIGHT_INTERNAL ) * i;
				Object[i]->Height = height;//20
				Object[i]->Width  = width;//40
			}
		}


		break;
	case COMPONENT_COMBOBOX:
		if(is2Col)
		{
		}
		else
		{
			for( int i = 0; i < Count; ++i )
			{
				Object[i]->Top	  = top + ( height + 20 ) * i;
				Object[i]->Left   = left;
				Object[i]->Height = height;//
				Object[i]->Width  = width;//
			}

		}
		break;
	case COMPONENT_SWITCH:
		if(is2Col)
		{
			for( int i = 0; i < Count; ++i )
			{
				if(i%2 == 0)
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i/2;
					//Object[i]->Left	= Object[0]->Left;
					Object[i]->Top	  = top + ( height + HEIGHT_INTERNAL ) * i/2;
					Object[i]->Left   = left;
				}
				else
				{
					//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * (i-1)/2;
					//Object[i]->Left	= Object[0]->Left+50;
					Object[i]->Top	  = top + ( height + HEIGHT_INTERNAL ) * (i-1)/2;
					Object[i]->Left   = left+50;
				}
			
				Object[i]->Height = height;//40;
				Object[i]->Width  = width;//40;
			}
	
		}
		else
		{
			for( int i = 0; i < Count; ++i )
			{
				//Object[i]->Top	  = Object[0]->Top + ( Object[0]->Height + HEIGHT_INTERNAL ) * i;
				//Object[i]->Left	= Object[0]->Left;
				Object[i]->Top	  = top + ( height + HEIGHT_INTERNAL ) * i;
				Object[i]->Left   = left;
				Object[i]->Height = height;//20
				Object[i]->Width  = width;//40
			}
		}
	
		break;

	default:
		break;
    }
}
//---------------------------------------------------------------------------
__fastcall API_Class::__ArrangeComponent( void *Object, int type, int Count, int height, int width, int top, int left, bool is2Col  )
{
    switch( type )
    {
    case COMPONENT_EDIT:
        __template_func( (TEdit**)Object, type, Count, height, width, top, left, is2Col );
		break;
    case COMPONENT_SCROBAR:
        __template_func( (TScrollBar**)Object, type, Count, height, width, top, left, is2Col );
		break;
    case COMPONENT_ILABEL:
        __template_func( (TLabel**) Object, type, Count, height, width, top, left, is2Col );
		break;
    case COMPONENT_BUTTON:
        __template_func( (TButton**) Object, type, Count, height, width, top, left, is2Col );
		break;
	case COMPONENT_CHECKBOX:
		__template_func( (TCheckBox**) Object, type, Count, height, width, top, left, is2Col );
		break;
	case COMPONENT_DBOOLEAN:
		__template_func( (TDBoolean**) Object, type, Count, height, width, top, left, is2Col );
		break;
	case COMPONENT_COMBOBOX:
		__template_func( (TComboBox**) Object, type, Count, height, width, top, left, is2Col );
		break;
	case COMPONENT_SWITCH:
		__template_func( (TDBoolean**) Object, type, Count, height, width, top, left, is2Col );
	default:
		break;

    }
    
    return 0;
}
//---------------------------------------------------------------------------
AnsiString __fastcall API_Class::GetDirectory(AnsiString path)
{
    int end = path.LastDelimiter("\\");
    AnsiString dir = path.SubString(0, end);
    return dir;
}
//---------------------------------------------------------------------------
AnsiString __fastcall API_Class::GetFileName(AnsiString path)
{
    int start = path.LastDelimiter("\\");
    int end = path.Length();
    AnsiString fileName = path.SubString(start+1, end-start);
    return fileName;
}
//---------------------------------------------------------------------------
AnsiString __fastcall API_Class::GetExtension(AnsiString path)
{
    int start = path.LastDelimiter(".");
    int end = path.Length();
    AnsiString extension = path.SubString(start, end);
    return extension;
}
//---------------------------------------------------------------------------
AnsiString __fastcall API_Class::AutoAddExtension(AnsiString path, AnsiString ext)
{
    AnsiString currentExt = GetExtension(path);
    if(ext.AnsiCompareIC(currentExt) == 0)
    {
        return path;
    }
    else
    {
        return path+ext;
    }
}


 
