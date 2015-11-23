//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainForm.h"
#include "pollingThread.h"
#include "ESI_Initial.c"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
PollingThread *pollingThread;
//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner)
        : TForm(Owner)
{
    //hide maximum, minimum, exit button on export window
    TBorderIcons tempBI;
    int iFileHandle;
    int iFileLength;
    char *pszBuffer;

    //tempBI >> biSystemMenu;
    tempBI >> biMinimize;
    tempBI >> biMaximize;
    //tempBI >> biHelp;

    //enableComponent(false);
    ESITemplatePathName = "ADLINK_BC6000.xml";
    this->Position = poScreenCenter;
    //this->Position = poDesktopCenter;
    //this->Position = poMainFormCenter;
    //this->Position = poOwnerFormCenter;

	systemPath = ExtractFilePath(Application->ExeName); 

	//base class for all form to use
	api = new API_Class(richEdMain);

    if(!ReadIni())
    {
        return;
    }

    if(m_odVisible)
    {
        //this->SetBounds(100, 50, 980, 788);
        this->Height = 788;
        this->Width = 980;
    }
    else
    {
        this->Height = 788;
        this->Width = 740;
        this->pnlMac->Left = 488;
        this->btnDump->Left = 542;
        this->btnClear->Left = 630;
    }

    for(int i = 0; i < ETHERCAT_SLAVE_MAX; i++)
    {
        slave[i] = NULL;
		m_productCode[i] = 0;
		device_productCode[i] = 0;
    }

    m_bTargetXmlCreated = false;
    //slave = new TFormSlave(this);
    //slave->VleCardInfo->Hide();


	this->ExportForm = new TForm(this);
	this->ExportForm->Parent = this;
	this->ExportForm->Width = 320;
	this->ExportForm->Height = 100;
	this->ExportForm->Top = 200;
	this->ExportForm->Left = 400;
    this->ExportForm->FormStyle = fsStayOnTop;
    //this->ExportForm->FormStyle = fsMDIChild;
    //this->ExportForm->FormStyle = fsNormal;
    //this->ExportForm->FormStyle = fsMDIForm;
    this->ExportForm->BorderIcons = tempBI;
	//this->ExportForm->Position = poScreenCenter;
/*
	this->lbPercent = new TLabel(this->ExportForm);
	this->lbPercent->Parent = this->ExportForm;
	this->lbPercent->Height = 15;
	this->lbPercent->Width = 100;
	this->lbPercent->Top = 0;
	this->lbPercent->Left = 10;
*/
	this->ExportPrgBar = new TProgressBar(this->ExportForm);
	this->ExportPrgBar->Parent = this->ExportForm;


	this->ExportPrgBar->Width = 280;
	this->ExportPrgBar->Top = 30;
	this->ExportPrgBar->Left = 10;

	cbEthernet->Style = csDropDownList;
	treeMain->ReadOnly = true;
	richEdMain->ReadOnly = true;

    cbEthernet->ShowHint = true;
    cbEthernet->Hint = "Choose the physical MAC address to start";
    btnScan->ShowHint = true;
    btnScan->Hint = "Scan for current bus coupler";
    btnExport->ShowHint = true;
    btnExport->Hint = "Export *xml file for current devices";

	info = new TformInfo(this);
	info->Hide();

}
//---------------------------------------------------------------------------
void __fastcall TformMain::FormCreate(TObject *Sender)
{
    int i, vi;
    AnsiString strTemp;

    if( BuildAdapterListing() == true )
    {
    	for( i = 0, vi = 0; i < m_nCardNum; i++ )
    	{
            strTemp = IntToHex(m_AvailNIC[i].MacAddress[0], 2) +"-"+
            IntToHex(m_AvailNIC[i].MacAddress[1], 2) +"-"+
            IntToHex(m_AvailNIC[i].MacAddress[2], 2) +"-"+
            IntToHex(m_AvailNIC[i].MacAddress[3], 2) +"-"+
            IntToHex(m_AvailNIC[i].MacAddress[4], 2) +"-"+
            IntToHex(m_AvailNIC[i].MacAddress[5], 2);

            cbEthernet->Items->Add(strTemp);
            vi++;
        }
        //m_nCardIndex = 0;
        cbEthernet->ItemIndex = 0;
		GetMacProc(cbEthernet->ItemIndex);

    }
	else
	{
		api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "FormCreate(): Get MAC address fail");
	}
	pollingThread = NULL;
	isECMasterOpen = false;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::enableComponent(bool enabled)
{
        //toolBarMain->Enabled = enabled;
        btnScan->Enabled = enabled;
		btnExport->Enabled = enabled;
        treeMain->Enabled = enabled;
}
//---------------------------------------------------------------------------
bool __fastcall TformMain::ReadIni()
{
	TStringList *slt=new TStringList;
	int lineCnt;
	AnsiString *line;

    slt->LoadFromFile(systemPath+"\\LinkMasterPro.ini");
    if(slt->Count > 0)
    {
    	lineCnt = slt->Count;
		line = new AnsiString[lineCnt];
		for(int i = 0; i < lineCnt; i++)
		{
			line[i] = slt->Strings[i];
		}
    }
    else
    {
        return false;
    }
    //int start = line[0].LastDelimiter("=");
    //int end = line[0].Length();
    m_odVisible = StrToBool(line[0].SubString(line[0].LastDelimiter("=")+1, line[0].Length()).Trim());
	m_isDebug = StrToBool(line[1].SubString(line[1].LastDelimiter("=")+1, line[1].Length()).Trim());
    return true;
}
//---------------------------------------------------------------------------

bool __fastcall TformMain::BuildAdapterListing()
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    //int cardNo;
    //AnsiString strError;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL)
    {
		api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "BuildAdapterListing(): Error allocating memory needed to call GetAdaptersinfo");
        return false;
    }

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
    dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    if (dwRetVal == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);

        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL)
        {
            api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "BuildAdapterListing(): Error allocating memory needed to call GetAdaptersinfo");
            return false;
        }
    }

    //if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    if (dwRetVal == NO_ERROR)
    {
    	m_nCardNum = 0;
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            if( pAdapter->Type == MIB_IF_TYPE_ETHERNET ) // only read MIB_IF_TYPE_ETHERNET Adapter
            {
		        memcpy( m_AvailNIC[m_nCardNum].NICDescription, pAdapter->Description, (MAX_ADAPTER_DESCRIPTION_LENGTH + 4) );
				memcpy( m_AvailNIC[m_nCardNum].MacAddress, pAdapter->Address, MAX_ADAPTER_ADDRESS_LENGTH );
				m_AvailNIC[m_nCardNum].AddressLength = pAdapter->AddressLength;
				
				m_nCardNum ++;

            }
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        //strError = "GetAdaptersInfo failed with error:" + IntToStr(dwRetVal);
    	//strError.Format( TEXT("GetAdaptersInfo failed with error: %d"), dwRetVal);
    	api->displayLogMsg(dwRetVal, "GetAdaptersInfo failed with error");
    }

    if (pAdapterInfo)
    {
        free(pAdapterInfo);
    }

    return true;
}
//---------------------------------------------------------------------------
int __fastcall TformMain::GetNICMac( BYTE *pMacAddr, UINT MacAddrLength, int index)
{
    if( MacAddrLength != m_AvailNIC[index].AddressLength )
    {
        return -1;
    }

    memcpy( pMacAddr, m_AvailNIC[index].MacAddress, MacAddrLength );

    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::GetMacProc(int index)
{
	BYTE pMacAddr[6];
	
	if(GetNICMac( pMacAddr, sizeof(pMacAddr), index) != 0)
	{
		api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "GetNICMac(): Invalid MAC Address length");
		return;
	}
	for(int i = 0; i < sizeof(pMacAddr); i++)
	{
		m_bMacAddr[i] = pMacAddr[i];
	}
	enableComponent(true);
	//btnScan->Enabled = true;
	btnExport->Enabled = false;
	
	//this->treeMain->Items->Clear();
	
	//clear previous child form
	//releasePollingThread();
	//cleanupChildForm();

}
//---------------------------------------------------------------------------

void __fastcall TformMain::cbEthernetChange(TObject *Sender)
{
    //int index = cbEthernet->ItemIndex;
	//enableComponent(false);
	//btnScan->Enabled = false;
	//btnExport->Enabled = false;
	int index = cbEthernet->ItemIndex;
	GetMacProc(index);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::Exit1Click(TObject *Sender)
{
    this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::LinkMasterPro1Click(TObject *Sender)
{
    info->Show();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnScanClick(TObject *Sender)
{
    long nErr;
	AnsiString str;
	int adlinkNo = 0;
    ULONG nVendorID, nProductCode;
    
	ULONG dwVendorIDAddr = 0x08;	//read vendor ID for other EtherCAT devices
	ULONG dwProductCodeAddr = 0x0A;	//read product code for other EtherCAT devices

    m_nSlaveNum = 0;

    for(int i=0; i< ETHERCAT_SLAVE_MAX; i++)
    {
        //m_SlaveElement[i] = FALSE;
        ECATSlaveNo[i] = -1;
    }

    //clear previous child form
    releasePollingThread();
	//releaseFoeThread();
    cleanupChildForm();

    this->treeMain->Items->Clear();
    tnMain = this->treeMain->Items->Add(NULL,"EtherCAT");

	if(!isECMasterOpen)
	{
		nErr = StartECMaster(m_bMacAddr);
		isECMasterOpen = true;	//check ECMaster open or not
	}
	else
	{
		nErr = ReScanSlaves(m_bMacAddr);
	}

    if( nErr == ECMaster_ERROR_SUCCESS )
    {
        nErr = GetSlaves(&m_nSlaveNum, devType);

        if(nErr == ECMaster_ERROR_SUCCESS)
        {
            for(int i = 0; i < m_nSlaveNum; i++)
            {
            	if(devType[i] == AD_SLAVE_TYPE_BC6000)
            	{
					str = "ADLINK-"+AnsiString(_EPS_) + IntToStr(adlinkNo);
					//ECATSlaveNo[adlinkNo] = i;	// adlink slave no / real slave no
					ECATSlaveNo[i] = adlinkNo;
					//tnMaster[i] = this->treeMain->Items->AddChild(tnMain, str);
					//generateProductCode(i);
					
					adlinkNo++;
            	}
				else
				{
					//nErr = SendSyncCoEUploadReq(i, sizeof(ULONG), OD_INDEX_BC6000_IDENTITY, 1, &nData, &dwDataLen);
					nErr = Slave_EEPROM_Read( i, dwVendorIDAddr, &nVendorID);
					if(nErr == ECMaster_ERROR_SUCCESS)
					{
						if(nVendorID == 0x144A)
						{
							nErr = Slave_EEPROM_Read( i, dwProductCodeAddr, &nProductCode);
							if(nErr == ECMaster_ERROR_SUCCESS)
							{
								str = "Unknown-Product_Code_#"+IntToHex((int)nProductCode, 4);
							}
							else
							{
								api->displayLogMsg(nErr, "Read EEPROM fail: " +IntToHex((int)dwProductCodeAddr, 4));
							}
						}
						else
						{
							str = "Unknown-Vendor_ID_#"+IntToHex((int)nVendorID, 4);
						}					
					}
					else
					{
						api->displayLogMsg(nErr, "Read EEPROM fail"+IntToHex((int)dwVendorIDAddr, 4));
					}
				}
				tnMaster[i] = this->treeMain->Items->AddChild(tnMain, str);

            }
			
			//calculate total export time
			m_nAdlinkSlaveNum = adlinkNo;
			wait_export_total = 100*m_nAdlinkSlaveNum;
			btnExport->Enabled = true;

			/*calculate cycle time & sync offset*/
			calculateDcSettings();

			createPollingThread();
			//createFoeThread();
			systemTimer->Enabled = true;
        }
        else
        {
			api->displayLogMsg(nErr, "Scan(): Get slave module fail");
        }
    }
    else
    {
		api->displayLogMsg(nErr, "Scan(): Start EtherCAT Master fail");
    }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::calculateDcSettings()
{
    short nErr = 0;
	long nSyncMRxPdo = 0, nSyncMTxPdo = 0, positiveJitter, negativeJitter;
	long cycle_time, target_cycle_time1, target_cycle_time2, safe_region1, safe_region2;
	long sync_offset, sync_offset1, min_sync_offset1, max_sync_offset1, sync_offset2, min_sync_offset2, max_sync_offset2;
	long copyTime1, copyTime2, delayTime, inputConversionTime;
	long copyTime1_total = 0, copyTime2_total = 0, delayTime_total = 0, inputConversionTime_total = 0;
	long dataTransferTime;
	long rxPdo, txPdo;
	long nValidEntries;
	ULONG dwDataLen;
	ULONG m_dwModuleSlot[TOTAL_SLOTS];
	int scanMode = 1;

	for(int slaveNo = 0; slaveNo < m_nSlaveNum; slaveNo ++)
	{
		if(devType[slaveNo] != AD_SLAVE_TYPE_BC6000)	//ignore third party slave
    	{
    		continue;
    	}

		nErr = SendSyncCoEDownloadReq(slaveNo, sizeof(ULONG), 0x100C, 1, &scanMode);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "calculateDcSettings(): Write scan mode fail");
		}

		nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x1C32, 6, &copyTime1, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "calculateDcSettings(): Get copy time1 fail");
		}
		else
		{
			copyTime1_total += copyTime1/1000;
		}

		nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x1C34, 3, &delayTime, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "calculateDcSettings(): Get delay time fail");
		}
		else
		{
			delayTime_total += delayTime/1000;
		}

		nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x1C33, 9, &inputConversionTime, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "calculateDcSettings(): Get input conversion time fail");
		}
		else
		{
			inputConversionTime_total += inputConversionTime/1000;
		}

		nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x1C33, 6, &copyTime2, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "calculateDcSettings(): Get copy time2 fail");
		}
		else
		{
			copyTime2_total += copyTime2/1000;
		}

		for(int nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
		{
			m_dwModuleSlot[nSlotNum] = 0x00000000;
			nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x100C, nSlotNum+2, &m_dwModuleSlot[nSlotNum], &dwDataLen);
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
				api->displayLogMsg(nErr, "calculateDcSettings(): Get slot module " +AnsiString(nSlotNum)+" fail");
				continue;
			}

			
			switch(m_dwModuleSlot[nSlotNum])
			{
			// RxPDO
			case EPS4008_ID:
				nValidEntries =  AO_eps4008_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += AO_eps4008_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			case EPS2032_ID:
				nValidEntries =  DO_eps2032_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += DO_eps2032_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			case EPS2132_ID:
				nValidEntries =  DO_eps2132_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += DO_eps2132_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			case EPS2308_ID:
				nValidEntries =  RO_eps2308_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += RO_eps2308_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			// TxPDO
			case EPS1132_ID:
				nValidEntries =  DI_eps1132_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += DI_eps1132_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;
				
			case EPS3032_ID:
				nValidEntries =  AI_eps3032_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += AI_eps3032_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;
				
			case EPS3216_ID:
				nValidEntries =  AI_eps3216_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += AI_eps3216_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;
	
			case EPS3504_ID:
				nValidEntries =  RTD_eps3504_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += RTD_eps3504_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			// RxPDO & TxPDO
			case EPS7002_ID:
				nValidEntries =  MO_eps7002_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += MO_eps7002_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				nValidEntries =  MO_eps7002_PDO[1].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += MO_eps7002_PDO[1].PdoEntry[nPdoEntry].wBitLen;
				}
				break;

			default:
				// do nothing
				break;


			}

		}

		nValidEntries =  bc6000_PDO[0].ValidEntries;
		for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
		{
			nSyncMRxPdo += bc6000_PDO[0].PdoEntry[nPdoEntry].wBitLen;
		}

		nValidEntries =  bc6000_PDO[1].ValidEntries;
		for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
		{
			nSyncMTxPdo += bc6000_PDO[1].PdoEntry[nPdoEntry].wBitLen;
		}

	}

	//transfer to byte
	rxPdo = nSyncMRxPdo / 8;
	txPdo = nSyncMTxPdo / 8;

	positiveJitter = StrToInt(edPositiveJitter->Text);
	negativeJitter = StrToInt(edNegativeJitter->Text);
	dataTransferTime = 10 + (rxPdo + txPdo)*0.08 + m_nAdlinkSlaveNum * 2 + (rxPdo + txPdo)*10/1390;

	cycle_time = positiveJitter - negativeJitter + dataTransferTime + copyTime1_total + delayTime_total + inputConversionTime_total + copyTime2_total;
	
	sync_offset = (positiveJitter + dataTransferTime + copyTime1_total)*100 / cycle_time;
	sync_offset = (sync_offset > 50) ? (sync_offset - 100) : sync_offset;

	if(cycle_time < 500)
	{
		target_cycle_time1 = 500;
		target_cycle_time2 = 1000;
	}
	else if(cycle_time < 1000)
	{
		target_cycle_time1 = 1000;
		target_cycle_time2 = 2000;
	}
	else
	{
		target_cycle_time1 = (cycle_time + 1000)/1000*1000;
		target_cycle_time2 = (cycle_time + 2000)/1000*1000;
	}

	//sync offset for target cycle time 1
	safe_region1 = target_cycle_time1 - cycle_time;
	
	sync_offset1 = (positiveJitter + dataTransferTime + copyTime1_total + safe_region1/2)*100 / target_cycle_time1;
	sync_offset1 = (sync_offset1 > 50) ? (sync_offset1 - 100) : sync_offset1;
	
	min_sync_offset1 = (positiveJitter + dataTransferTime + copyTime1_total)*100 / target_cycle_time1;
	min_sync_offset1 = (min_sync_offset1 > 50) ? (min_sync_offset1 - 100) : min_sync_offset1;
	
	max_sync_offset1 = (positiveJitter + dataTransferTime + copyTime1_total + safe_region1)*100 / target_cycle_time1;
	max_sync_offset1 = (max_sync_offset1 > 50) ? (max_sync_offset1 - 100) : max_sync_offset1;

	//sync offset for target cycle time 2
	safe_region2 = target_cycle_time2 - cycle_time;
	
	sync_offset2 = (positiveJitter + dataTransferTime + copyTime1_total + safe_region2/2)*100 / target_cycle_time2;
	sync_offset2 = (sync_offset2 > 50) ? (sync_offset2 - 100) : sync_offset2;
	
	min_sync_offset2 = (positiveJitter + dataTransferTime + copyTime1_total)*100 / target_cycle_time2;
	min_sync_offset2 = (min_sync_offset2 > 50) ? (min_sync_offset2 - 100) : min_sync_offset2;
	
	max_sync_offset2 = (positiveJitter + dataTransferTime + copyTime1_total + safe_region2)*100 / target_cycle_time2;
	max_sync_offset2 = (max_sync_offset2 > 50) ? (max_sync_offset2 - 100) : max_sync_offset2;

	api->displayMark("************************************************");
	api->displayMark("************* Fieldbus Analysis Result *************");
	api->displayMark("************************************************");
	api->displayLogMsg(0, "Minimum Cycle Time: " + AnsiString(cycle_time) + "us");
	api->displayLogMsg(0, "Optimized Sync Offset: " + AnsiString(sync_offset) + "%");
	api->displayMark("************* Target Cycle Time Case 1 *************");
	//api->displayLogMsg(0, "Case 1 Target Cycle Time: " + AnsiString(target_cycle_time1) + URLEncode(" £gs"));
	api->displayLogMsg(0, "Case 1 Target Cycle Time: " + AnsiString(target_cycle_time1) + "us");
	api->displayLogMsg(0, "Case 1 Sync Offset: " + AnsiString(sync_offset1) + "%");
	if(max_sync_offset1 < 0 && min_sync_offset1 > 0)
	{
		api->displayLogMsg(0, "Case 1 Sync Offset Region: (-50% ~ " + AnsiString(max_sync_offset1) + "%, " + AnsiString(min_sync_offset1) + "% ~ 50%)");
	}
	else
	{
		api->displayLogMsg(0, "Case 1 Sync Offset Region: (" + AnsiString(min_sync_offset1) + "% ~ " + AnsiString(max_sync_offset1) + "%)");
	}
	api->displayMark("************* Target Cycle Time Case 2 *************");
	//api->displayLogMsg(0, "Case 2 Target Cycle Time: " + AnsiString(target_cycle_time2) + URLEncode(" £gs"));
	api->displayLogMsg(0, "Case 2 Target Cycle Time: " + AnsiString(target_cycle_time2) + "us");
	api->displayLogMsg(0, "Case 2 Sync Offset: " + AnsiString(sync_offset2) + "%");
	if(max_sync_offset2 < 0 && min_sync_offset2 > 0)
	{
		api->displayLogMsg(0, "Case 2 Sync Offset Region: (-50% ~ " + AnsiString(max_sync_offset2) + "%, " + AnsiString(min_sync_offset2) + "% ~ 50%)");
	}
	else
	{
		api->displayLogMsg(0, "Case 2 Sync Offset Region: (" + AnsiString(min_sync_offset2) + "% ~ " + AnsiString(max_sync_offset2) + "%)");
	}

	api->displayMark("************************************************");
	api->displayMark("******** Note : Only EPS devices are analyzed ********");
	api->displayMark("************************************************");
}
//---------------------------------------------------------------------------
void __fastcall TformMain::TreeMainMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Check_Level;
    AnsiString text, start;
    short nErr;

    //suspendPollingThread();
	
    tn = this->treeMain->GetNodeAt(X,Y);

    //====================================================================
    if( tn != NULL )
    {
        this->treeMain->Selected = tn;
        Check_Level  = this->treeMain->Selected->Level;
        tn->Selected = true;
    }
    else
    {
        //this->treeEthercat->Selected = this->treeEthercat->Selected ;
        //Check_Level                  = this->treeMain->Selected->Level;
        return;
    }

    switch(Check_Level)
    {
    case Level_Root:
        //do nothing
        break;

    case Level_CardType:
        text = this->treeMain->Selected->Text;
        start = text.SubString(1, 6);   //get "ADLINK"
        if(start == "ADLINK")
        {
            int adlinkNo = StrToInt(text.AnsiLastChar());
			for(int i = 0; i < ETHERCAT_SLAVE_MAX; i++)
			{
				if(ECATSlaveNo[i] == adlinkNo)
				{
					wSlaveNo = i;	//find real slave No
					break;
				}
			}
			//wSlaveNo = ECATSlaveNo[adlinkNo];
            if(slave[wSlaveNo] == NULL)
            {
                slave[wSlaveNo] = new TformSlave(this);
                slave[wSlaveNo]->Caption = _EPS_ + IntToStr(adlinkNo);
				slave[wSlaveNo]->displayOdFunc(m_odVisible);
				
				slave[wSlaveNo]->updateSlaveNum(wSlaveNo, adlinkNo, api);
				
            }
			slave[wSlaveNo]->isSlaveUpdate = true;
        }
        break;

    case Level_CardNo:
        //do nothing
        break;
    default:
        //do nothing
        break;
    }
	//resumePollingThread();
}
//---------------------------------------------------------------------------
void __fastcall TformMain::checkSameDevice(int total_slave)
{
    short nErr = 0;
	unsigned long dwProductCode, dwRevisionNum;
    ULONG dwDataLen;
    ULONG m_dwModuleSlot, m_dwModuleSlot_cmp, checkInBit;
	//unsigned long device_productCode[100];
	//for(int i = 0; i < 100; i++)
	{
		//device_productCode[i] = 0;
	}

	for(int slaveNo = 0; slaveNo < total_slave; slaveNo ++)
	{
		if(devType[slaveNo] != AD_SLAVE_TYPE_BC6000)	//ignore third party slave
		{
			continue;
		}
		readModuleIdentity(slaveNo, &dwProductCode, &dwRevisionNum);
		device_productCode[slaveNo] = dwProductCode;	//use 0/1 to record different device set

		m_productCode[slaveNo] = dwProductCode;	//get all EPS product code
		m_revisionNum[slaveNo] = dwRevisionNum;	//get all EPS revision number
	}

	for(int slaveNo = 0; slaveNo < total_slave; slaveNo ++)
	{
		//scan for same product code
		for(int slaveNo_cmp = slaveNo+1; slaveNo_cmp < total_slave; slaveNo_cmp++)
		{
			if(device_productCode[slaveNo] == device_productCode[slaveNo_cmp])
			{
				//scan for same device
				checkInBit = 0x00;
	    		for(int nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
	    		{
	    			m_dwModuleSlot = 0x00000000;
	    			nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x100C, nSlotNum+2, &m_dwModuleSlot, &dwDataLen);
					if(nErr != ECMaster_ERROR_SUCCESS)
					{
						api->displayLogMsg(nErr, "checkSameDevice(): Get slot module " +AnsiString(nSlotNum)+" fail");
					}
					
					m_dwModuleSlot_cmp = 0x00000000;
					nErr = SendSyncCoEUploadReq(slaveNo_cmp, sizeof(ULONG), 0x100C, nSlotNum+2, &m_dwModuleSlot_cmp, &dwDataLen);
					if(nErr != ECMaster_ERROR_SUCCESS)
					{
						api->displayLogMsg(nErr, "checkSameDevice(): Get slot module " +AnsiString(nSlotNum)+" fail");
					}

					if(m_dwModuleSlot == m_dwModuleSlot_cmp)
					{
						checkInBit =  0x01 << nSlotNum | checkInBit;
					}
                }

				if(checkInBit == 0x0F)
				{
					device_productCode[slaveNo_cmp] = 0;	//same product code, same device
				}
				else
				{
					device_productCode[slaveNo_cmp] = 1;	//same product code, different device
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TformMain::readModuleIdentity(int slaveNo, unsigned long *productCode, unsigned long *revisionNum)
{
	long nErr;
	ULONG dwDataLen = 0;

	unsigned long dwProductCode, dwRevisionNum;

	nErr = SendSyncCoEUploadReq( slaveNo, sizeof(ULONG), OD_INDEX_BC6000_IDENTITY, OD_SUBINDEX_BC6000_PRODUCT_CODE, &dwProductCode, &dwDataLen );
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "readModuleIdentity(): Slave " + AnsiString(slaveNo) + " Get product code fail: " + AnsiString(dwProductCode));
	}
	*productCode = dwProductCode;

	nErr = SendSyncCoEUploadReq( slaveNo, sizeof(ULONG), OD_INDEX_BC6000_IDENTITY, OD_SUBINDEX_BC6000_REVISION_NUM, &dwRevisionNum, &dwDataLen );
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "readModuleIdentity(): Slave " + AnsiString(slaveNo) + " Get revision number fail: " + AnsiString(dwRevisionNum));
	}
	*revisionNum = dwRevisionNum;

}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnExportClick(TObject *Sender)
{
#define PDO_MAX_NUM   5

    short nErr = 0;
    AnsiString strESIoutputXmlFile;
	//AnsiString repToolPath, argument, repTool, profile, strDevice;

    int nModulePDO = 0;
    bool isESICreated = false;
    int nSlotNum;
    SII_MODULE_PDO SIIModulePdo[TOTAL_SLOTS+1];	// 4 slots + BC6000 (5th slot)
    ULONG dwDataLen;
    ULONG m_dwModuleSlot[TOTAL_SLOTS];
	ULONG *pSerialNum, serialNum = 0;
    int nAxisNum = 0;
	int scanMode = 1;	//1: scan current module	2:read moudule from flash	3:scan current module and update flash

    if(m_nSlaveNum <= 0)
    {
		api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "Export(): No Slave module");
        return;
    }

	//suspendPollingThread();
	releasePollingThread();
	enableComponent(false);
    
    this->SaveDlgExport->InitialDir = ExtractFileDir(Application->ExeName) ;
    this->SaveDlgExport->Filter = "xml files (*.xml)|*.XML";

    if(this->SaveDlgExport->Execute())
    {
        strESIoutputXmlFile = SaveDlgExport->FileName;
        if(!strESIoutputXmlFile.IsEmpty())
        {
            strESIoutputXmlFile = api->AutoAddExtension(strESIoutputXmlFile, ".xml");

			//display export percent
			
			this->ExportForm->Show();
            this->ExportForm->SetFocus();

			checkSameDevice(m_nSlaveNum);
			
            for(int slaveNo = 0; slaveNo < m_nSlaveNum; slaveNo ++)
            {
            	if(devType[slaveNo] != AD_SLAVE_TYPE_BC6000)	//ignore third party slave
            	{
            		continue;
            	}

				
				nErr = SendSyncCoEDownloadReq(slaveNo, sizeof(ULONG), 0x100C, 1, &scanMode);
				if(nErr != ECMaster_ERROR_SUCCESS)
				{
					api->displayLogMsg(nErr, "Export(): Write scan mode fail");
				}

	    		for( nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
	    		{
	    			m_dwModuleSlot[nSlotNum] = 0x00000000;
	    			//nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0xF030, nSlotNum+1, &m_dwModuleSlot[nSlotNum], &dwDataLen);
	    			nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x100C, nSlotNum+2, &m_dwModuleSlot[nSlotNum], &dwDataLen);
					if(nErr != ECMaster_ERROR_SUCCESS)
					{
						api->displayLogMsg(nErr, "Export(): Get slot module " +AnsiString(nSlotNum)+" fail");
						continue;
					}
					else
					{
						if(m_isDebug)
						{
							api->displayLogMsg(nErr, "Export(): Slave "+AnsiString(slaveNo)+" slot " +AnsiString(nSlotNum)+" ("+ IntToHex((int)m_dwModuleSlot[nSlotNum], 4)+")");
						}
					}
                }

				if(m_isDebug)
				{
					api->displayLogMsg(nErr, "Export(): Slave "+AnsiString(slaveNo)+" product code " + IntToHex((int)m_productCode[slaveNo], 8));
				}

				//generateProductCode(slaveNo, &dwProductCode);
				//m_productCode[slaveNo] = dwProductCode;

				//same product code, same device-->combine the device
				if(device_productCode[slaveNo] != 0)
				{
					if(!isESICreated)	//the first one adlink slave box, use CREATE_NEW_MULTIPLE_DEVICES_ESI to create ESI
					{
						ESISingleDeviceAdd(slaveNo, m_dwModuleSlot, m_productCode[slaveNo], m_revisionNum[slaveNo], CREATE_NEW_MULTIPLE_DEVICES_ESI, strESIoutputXmlFile.c_str(), strESIoutputXmlFile.Length());
						isESICreated = true;
					}
					else	//after second adlink slave box, use APPEND_TO_MULTIPLE_DEVICES_ESI to append ESI
					{
						ESISingleDeviceAdd(slaveNo, m_dwModuleSlot, m_productCode[slaveNo], m_revisionNum[slaveNo], APPEND_TO_MULTIPLE_DEVICES_ESI, strESIoutputXmlFile.c_str(), strESIoutputXmlFile.Length());
					}

				}

				displayExportPrg(60, slaveNo);


				//Update EEPROM

				nModulePDO = 0;
				nAxisNum = 0;
				for(nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
				{
					switch( m_dwModuleSlot[nSlotNum] )
					{
					case EPS4008_ID:
						// Fill the SII_MODULE_PDO
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(AO_eps4008_PDO[0]);
						nModulePDO ++;
						break;
				
					case EPS1132_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(DI_eps1132_PDO[0]);
						nModulePDO ++;
						break;
				
					case EPS3032_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(AI_eps3032_PDO[0]);
						nModulePDO ++;
						break;
				
					case EPS3216_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(AI_eps3216_PDO[0]);
						nModulePDO ++;
						break;
				
					case EPS2032_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(DO_eps2032_PDO[0]);
						nModulePDO ++;
						break;

					case EPS2132_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(DO_eps2132_PDO[0]);
						nModulePDO ++;
						break;

					case EPS3504_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(RTD_eps3504_PDO[0]);
						nModulePDO ++;
						break;

					case EPS2308_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = 0; // ignored 
						SIIModulePdo[nModulePDO].nPdoNumber = 1;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(RO_eps2308_PDO[0]);
						nModulePDO ++;
						break;
					case EPS7002_ID:
						SIIModulePdo[nModulePDO].nSlotNumber = nSlotNum;
						SIIModulePdo[nModulePDO].nAxisNumber = nAxisNum;
						SIIModulePdo[nModulePDO].nPdoNumber = 2;
						SIIModulePdo[nModulePDO].pPdoList[0] = &(MO_eps7002_PDO[0]);
						SIIModulePdo[nModulePDO].pPdoList[1] = &(MO_eps7002_PDO[1]);
						nModulePDO ++;
						nAxisNum += 2;
						break;
						
					default:
						// do nothing
						break;
					}
				}


				displayExportPrg(70, slaveNo);

				// Bus-Couple
				SIIModulePdo[nModulePDO].nSlotNumber = 0x0b; 
				SIIModulePdo[nModulePDO].nPdoNumber = 2;
				SIIModulePdo[nModulePDO].pPdoList[0] = &(bc6000_PDO[0]);
				SIIModulePdo[nModulePDO].pPdoList[1] = &(bc6000_PDO[1]);
				nModulePDO ++;

				nErr = SII_EEPROM_Init( SIIModulePdo, nModulePDO, BusCouple_Eeprom, m_productCode[slaveNo], m_revisionNum[slaveNo]); 
				if( nErr != ECMaster_ERROR_SUCCESS )
				{
					api->displayLogMsg(nErr, "Export(): SII_EEPROM_Init() Failed, Product code:" + IntToHex((int)m_productCode[slaveNo], 8)+", Revision number:"+IntToHex((int)m_revisionNum[slaveNo], 8));
				}
				if(m_isDebug)
				{
					api->displayLogMsg(nErr, "Export(): nModulePDO "+ AnsiString(nModulePDO)+ ", product code "+IntToHex((int)m_productCode[slaveNo], 8));
				}

				displayExportPrg(80, slaveNo);


				//same product code, different device-->use serial number to recongnize
				if(device_productCode[slaveNo] == 1)
				{
					pSerialNum = (ULONG*)(BusCouple_Eeprom + 0x1C);
					serialNum++;
					*pSerialNum = api->convertEndian(serialNum);
				}

				//nErr = SII_EEPROM_Write( slaveNo, BusCouple_Eeprom );
				nErr = SII_EEPROM_Write_All( slaveNo, BusCouple_Eeprom );
				if( nErr == ECMaster_ERROR_SUCCESS )
				{
					//ShowMessage( " SII_EEPROM_Write() Slave_" +AnsiString(slaveNo)+" Completed" );
					api->displayLogMsg(nErr, "Export(): Update slave " +AnsiString(slaveNo)+ " EEPROM completed");

				}
				else
				{
					api->displayLogMsg(nErr, "Export(): Updtae slave " +AnsiString(slaveNo)+ " EEPROM failed");
				}


				//wait_export_percent = 100 * (wSlaveNo+1);
				displayExportPrg(90, slaveNo);

            }

			if(nErr == ECMaster_ERROR_SUCCESS)
			{
				generateDeviceXml(m_nSlaveNum, strESIoutputXmlFile);
			}

			displayExportPrg(100, m_nSlaveNum-1);

        }
    }

	//resumePollingThread();
	createPollingThread();
	enableComponent(true);

}
//---------------------------------------------------------------------------
bool __fastcall TformMain::ESISingleDeviceAdd(int slaveNo, ULONG dwModuleSlot[], unsigned long dwProductCode, unsigned long dwRevisionNum, USHORT wFileOperation, TCHAR strESIoutputXmlFile[], int bufLength)
{
#define PDO_MAX_NUM   5 // 4 slots + BC6000
	long nSlotNum, nAxisNum;
	long nSyncMRxPdo, nSyncMTxPdo;
	long nRxPdoNum, nTxPdoNum;
	long nValidEntries;
	unsigned short wPdoIndex;
	ESI_DEVICE_PDO *pESIObject;
	unsigned short wRxPdoIndex[PDO_MAX_NUM];
	unsigned short wTxPdoIndex[PDO_MAX_NUM];
	TCHAR lpszCurDirectory[MAX_PATH+32];
	TCHAR lpszTempFileName[32] = TEXT("\\_temp.xml");
	//TCHAR lpszTempFileName[32];

	TCHAR *lpszESITempXmlFile = NULL;

	DWORD dwRet;
	short nErr = ECMaster_ERROR_SUCCESS;

	//_stprintf( lpszTempFileName, TEXT( "\\_temp%d.xml" ), slaveNo ) ;

	   //dwRet = GetCurrentDirectoryA(32, lpszCurDirectory);
	   dwRet = GetCurrentDirectoryA(bufLength, lpszCurDirectory);
	   if( dwRet == 0 )
	   {
		   api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "ESISingleDeviceAdd(): GetCurrentDirectory failed");
		   this->ExportForm->Hide();
		   return false;
	   }
	   if(dwRet > (DWORD)bufLength)
	   {
		   api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "ESISingleDeviceAdd(): File path too long");
		   this->ExportForm->Hide();
		   return false;
	   }
	
		//_tcscat( lpszCurDirectory, lpszTempFileName );
		memcpy( lpszCurDirectory, lpszTempFileName, sizeof(lpszCurDirectory) );
		lpszESITempXmlFile = lpszCurDirectory;
	
		// Clear m_bTargetXmlCreated
		m_bTargetXmlCreated = false;
		nSyncMRxPdo = 0; 
		nSyncMTxPdo = 0;
		nRxPdoNum = 0;
		nTxPdoNum = 0;
		nAxisNum = 0;
	
		for( nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
		{
			switch( dwModuleSlot[nSlotNum] )
			{
			case 0x00000000:
				// do nothing
				break;
			case EPS4008_ID:
				ESI_Default_Data_Update( &(AO_eps4008[0]), &(AO_eps4008_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_AO_eps4008( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS1132_ID:
				ESI_Default_Data_Update( &(DI_eps1132[0]), &(DI_eps1132_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_DI_eps1132( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS3032_ID:
				ESI_Default_Data_Update( &(AI_eps3032[0]), &(AI_eps3032_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_AI_eps3032( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS3216_ID:
				ESI_Default_Data_Update( &(AI_eps3216[0]), &(AI_eps3216_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_AI_eps3216( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS2032_ID:
				ESI_Default_Data_Update( &(DO_eps2032[0]), &(DO_eps2032_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_DO_eps2032( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS2132_ID:
				ESI_Default_Data_Update( &(DO_eps2132[0]), &(DO_eps2132_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_DO_eps2132( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS3504_ID:
				ESI_Default_Data_Update( &(RTD_eps3504[0]), &(RTD_eps3504_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_RTD_eps3504( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS2308_ID:
				ESI_Default_Data_Update( &(RO_eps2308[0]), &(RO_eps2308_PDO[0]), nSlotNum, nAxisNum );
				nErr = ExportESI_RO_eps2308( nSlotNum, lpszESITempXmlFile );
				break;
			case EPS7002_ID:
				ESI_Default_Data_Update( &(MO_eps7002[0]), &(MO_eps7002_PDO[0]), nSlotNum, nAxisNum );
				ESI_Default_Data_Update( &(MO_eps7002[1]), &(MO_eps7002_PDO[1]), nSlotNum, nAxisNum );
				nErr = ExportESI_MO_eps7002( nSlotNum, lpszESITempXmlFile, nAxisNum);
				nAxisNum += 2;  // dual axes
				break;
			default:
				api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "ESISingleDeviceAdd(): Unsupport Module ID");
				break;
			}

			if( nErr != ECMaster_ERROR_SUCCESS )
			{
				api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ExportESI 0x"+IntToHex((int)dwModuleSlot[nSlotNum], 4)+" failed");
				return false;
			}
		}


		//export percentage
		displayExportPrg(10, slaveNo);

		// ESI RxPDO
		nAxisNum = 0;  // reset nAxisNum
		for( nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
		{
			switch( dwModuleSlot[nSlotNum] )
			{
			case EPS4008_ID:
				nErr = ESIPDOExport( nSlotNum, &(AO_eps4008_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  AO_eps4008_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += AO_eps4008_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(AO_eps4008_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wRxPdoIndex[nRxPdoNum] = wPdoIndex;
				
				nRxPdoNum ++;
	
				break;
	
			case EPS2032_ID:
				nErr = ESIPDOExport( nSlotNum, &(DO_eps2032_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  DO_eps2032_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += DO_eps2032_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(DO_eps2032_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wRxPdoIndex[nRxPdoNum] = wPdoIndex;
							
				nRxPdoNum ++;
	
				break;

			case EPS2132_ID:
				nErr = ESIPDOExport( nSlotNum, &(DO_eps2132_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  DO_eps2132_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += DO_eps2132_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(DO_eps2132_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wRxPdoIndex[nRxPdoNum] = wPdoIndex;
							
				nRxPdoNum ++;
			
				break;

	
			case EPS2308_ID:
				nErr = ESIPDOExport( nSlotNum, &(RO_eps2308_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  RO_eps2308_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += RO_eps2308_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(RO_eps2308_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wRxPdoIndex[nRxPdoNum] = wPdoIndex;
							
				nRxPdoNum ++;
	
				break;
			case EPS7002_ID:
				nErr = ESIPDOExport( nSlotNum, &(MO_eps7002_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, nAxisNum);
				nAxisNum += 2;  // dual axes
				nValidEntries =  MO_eps7002_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMRxPdo += MO_eps7002_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				pESIObject = &(MO_eps7002_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wRxPdoIndex[nRxPdoNum] = wPdoIndex;

				nRxPdoNum ++;
				break;
			default:
				// do nothing
				break;
			}

			if( nErr != ECMaster_ERROR_SUCCESS )
			{
				api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESIPDOExport() failed, ID: 0x" + IntToHex((int)dwModuleSlot[nSlotNum], 4));
				return false;
			}
		}

		if(m_isDebug)
		{
			api->displayLogMsg(0, "ESISingleDeviceAdd(): m_bTargetXmlCreated: " + AnsiString((int)m_bTargetXmlCreated));
		}

		if(m_bTargetXmlCreated == false)
		{
			nErr = ESIPDOExport( 0x0B, &(bc6000_PDO[0]), NULL, lpszESITempXmlFile, 0 );
			m_bTargetXmlCreated = true;
		}
		else
		{
			nErr = ESIPDOExport( 0x0B, &(bc6000_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
		}

		//nErr = ESIPDOExport( 0x0B, &(bc6000_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
		nValidEntries =  bc6000_PDO[0].ValidEntries;
		for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
		{
			nSyncMRxPdo += bc6000_PDO[0].PdoEntry[nPdoEntry].wBitLen;
		}
		
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): BC-6000 ESIPDOExport() failed ");
			return false;
		}
	
		pESIObject = &( bc6000_PDO[0]);
		wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
		wRxPdoIndex[nRxPdoNum] = wPdoIndex;
					
		nRxPdoNum ++;
	
		nErr = ESISyncMChange(lpszESITempXmlFile, RxPDO_TYPE, (nSyncMRxPdo >> 3), nRxPdoNum, wRxPdoIndex );
	
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESISyncMChange( RxPDO_TYPE ) failed ");
			return false;
		}

		//export percentage
		displayExportPrg(30, slaveNo);


		// ESI TxPDO
		nAxisNum = 0; //reset nAxisNum
		for( nSlotNum = 0; nSlotNum < TOTAL_SLOTS; nSlotNum ++ )
		{
			switch( dwModuleSlot[nSlotNum] )
			{
			case EPS1132_ID:
				nErr = ESIPDOExport( nSlotNum, &(DI_eps1132_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  DI_eps1132_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += DI_eps1132_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(DI_eps1132_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wTxPdoIndex[nTxPdoNum] = wPdoIndex;
				
				nTxPdoNum ++;
				break;
			case EPS3032_ID:
				nErr = ESIPDOExport( nSlotNum, &(AI_eps3032_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  AI_eps3032_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += AI_eps3032_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
	
				pESIObject = &(AI_eps3032_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wTxPdoIndex[nTxPdoNum] = wPdoIndex;
				
				nTxPdoNum ++;
				break;
			case EPS3216_ID:
				nErr = ESIPDOExport( nSlotNum, &(AI_eps3216_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  AI_eps3216_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += AI_eps3216_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
	
				pESIObject = &(AI_eps3216_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wTxPdoIndex[nTxPdoNum] = wPdoIndex;
				
				nTxPdoNum ++;
				break;
	
			case EPS3504_ID:
				nErr = ESIPDOExport( nSlotNum, &(RTD_eps3504_PDO[0]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
				nValidEntries =  RTD_eps3504_PDO[0].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += RTD_eps3504_PDO[0].PdoEntry[nPdoEntry].wBitLen;
				}
	
				pESIObject = &(RTD_eps3504_PDO[0]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wTxPdoIndex[nTxPdoNum] = wPdoIndex;
				
				nTxPdoNum ++;
				break;
				
			case EPS7002_ID:
				nErr = ESIPDOExport( nSlotNum, &(MO_eps7002_PDO[1]), lpszESITempXmlFile, lpszESITempXmlFile, nAxisNum);
				nAxisNum += 2;  // dual axes
				nValidEntries =  MO_eps7002_PDO[1].ValidEntries;
				for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
				{
					nSyncMTxPdo += MO_eps7002_PDO[1].PdoEntry[nPdoEntry].wBitLen;
				}
				
				pESIObject = &(MO_eps7002_PDO[1]);
				wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
				wTxPdoIndex[nTxPdoNum] = wPdoIndex;

				nTxPdoNum ++;
				break;

			default:
				// do nothing
				break;
			}
			if( nErr != ECMaster_ERROR_SUCCESS )
			{
				api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESIPDOExport() 0x" +IntToHex((int)dwModuleSlot[nSlotNum], 4)+ "failed");
				return false;
			}

		}


		nErr = ESIPDOExport( 0x0B, &(bc6000_PDO[1]), lpszESITempXmlFile, lpszESITempXmlFile, 0 );
		nValidEntries =  bc6000_PDO[1].ValidEntries;
		for( int nPdoEntry = 0; nPdoEntry < nValidEntries; nPdoEntry ++)
		{
			nSyncMTxPdo += bc6000_PDO[1].PdoEntry[nPdoEntry].wBitLen;
		}
		
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): BC-6000 ESIPDOExport() failed ");
			return false;
		}
	
		pESIObject = &( bc6000_PDO[1]);
		wPdoIndex = pESIObject->wIdxBase + ( pESIObject->wIdxMultiple * (USHORT)nSlotNum );
		wTxPdoIndex[nTxPdoNum] = wPdoIndex;
			
		nTxPdoNum ++;
	
		nErr = ESISyncMChange(lpszESITempXmlFile, TxPDO_TYPE, (nSyncMTxPdo >> 3), nTxPdoNum, wTxPdoIndex );
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESISyncMChange( TxPDO_TYPE ) failed ");
			return false;
		}
	
	
		//nErr = ESIProductCodeChange( lpszESITempXmlFile, dwProductCode );
		nErr = ESIDevicePhysicsChange( lpszESITempXmlFile, dwProductCode, dwRevisionNum);
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESIProductCodeChange() failed ");
			return false;
		}

		if( nAxisNum > 0 ) // MO module is repsent, change the Profile
		{
			nErr = ESIProfileChange( lpszESITempXmlFile, 402, nAxisNum );
			if( nErr != ECMaster_ERROR_SUCCESS )
			{
				api->displayLogMsg(nErr, "ESIProfileChange() failed, Export Cancel ");
				return false;
			}

			nErr = ESISlotsDelete( lpszESITempXmlFile );
			if( nErr != ECMaster_ERROR_SUCCESS )
			{
				api->displayLogMsg(nErr, " ESISlotsDelete() failed ");
				return false;
			}
		}

		for( nSlotNum = 0; nSlotNum < 4; nSlotNum ++ )
		{
			switch( dwModuleSlot[nSlotNum] )
			{
			case 0x00000000:
				// do nothing
				break;
			case EPS4008_ID:
			case EPS1132_ID:
			case EPS3032_ID:
			case EPS3216_ID:
			case EPS2032_ID:
			case EPS2132_ID:
			case EPS3504_ID:
			case EPS2308_ID:
			case EPS7002_ID:
				nErr = ESISlotChange( lpszESITempXmlFile, nSlotNum, (unsigned short)( dwModuleSlot[nSlotNum] & 0xFFFF)  );
				break;
			default:
				// do nothing
				api->displayLogMsg(nErr, "ESISlotChange() failed ");
				break;
			}		
		}
	
		nErr = ESIDeviceAppend( lpszESITempXmlFile, strESIoutputXmlFile, wFileOperation );
		if( nErr != ECMaster_ERROR_SUCCESS )
		{
			api->displayLogMsg(nErr, "ESISingleDeviceAdd(): ESIDeviceAppend() failed ");
			return false;
		}


		// Final, delete the temprary file.
		if(remove(lpszESITempXmlFile)!= 0)
		{
			api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, " delete temp file failed ");
		}

		//export percentage
		displayExportPrg(50, slaveNo);

		return true;


}
//---------------------------------------------------------------------------
void __fastcall TformMain::ESI_Default_Data_Update( ESI_OBJECT *pESIObject, ESI_DEVICE_PDO *pESIPDO, long nSlotNum, long nAxisNum )
{
		unsigned long nSubIndex, EntryIndex, dwDefaultData;
		USHORT wEntryIndex, wItemIndex;
		USHORT wSwapMappedIndex;
		
		if( (pESIObject->wIdxBase != pESIPDO->wIdxBase ) ) // either RxPDO-Map_XXX nor TxPDO-Map_XXX
		{
			api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, AnsiString(pESIObject->ObjectName) + "- wIdxBase is not matched");
			return;
		}
		
		nSubIndex = (pESIObject->ObjectInfo.ValidSubItems -1); 
		
#if 0
		if( pESIObject->ObjectInfo.SubItem[0].Info.DefaultData	!=	nSubIndex )
		{
			strTemp.Format( TEXT("%s -	Invalid ValidSubItems in ESI_OBJECT"), pESIObject->ObjectName );
			AfxMessageBox( strTemp );
			return;
		}
#endif
	
		if( pESIPDO->ValidEntries !=  nSubIndex )
		{
			api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, AnsiString(pESIObject->ObjectName) + "-	 ValidEntries not equal to ValidSubItems");
			return;
		}		
			
		for( EntryIndex=0, wItemIndex=1; EntryIndex<nSubIndex; EntryIndex++, wItemIndex++ )
		{
			if( pESIPDO->wEntryIdxRefer == PDO_ENTRY_INDEX_BY_SLOT )
				wEntryIndex = pESIPDO->PdoEntry[EntryIndex].wEntryIdxBase + ( (USHORT)nSlotNum * pESIPDO->PdoEntry[EntryIndex].wEntryIdxMultiple);
			else if( pESIPDO->wEntryIdxRefer == PDO_ENTRY_INDEX_BY_AXIS )
				wEntryIndex = pESIPDO->PdoEntry[EntryIndex].wEntryIdxBase + ( (USHORT)nAxisNum * pESIPDO->PdoEntry[EntryIndex].wEntryIdxMultiple);
									
			wSwapMappedIndex = ( wEntryIndex >> 8 ) & 0xFF;
			wSwapMappedIndex |= ( (wEntryIndex & 0xFF) << 8 );
			
			dwDefaultData = pESIObject->ObjectInfo.SubItem[wItemIndex].Info.DefaultData;
			dwDefaultData = (dwDefaultData & 0xFFFF0000); // mask for mapping index
			dwDefaultData = (dwDefaultData | wSwapMappedIndex);
			
			pESIObject->ObjectInfo.SubItem[wItemIndex].Info.DefaultData = dwDefaultData;
		}

}
//---------------------------------------------------------------------------
void __fastcall TformMain::generateDeviceXml(int totalSlave, AnsiString strESIoutputXmlFile)
{
	AnsiString repToolPath, argument, repTool, profile, strDevice, parameters;
	
	//------------ESI convert to device.xml------------//
	repToolPath = "C:\\Program Files\\3S CODESYS\\CODESYS\\Common\\";
	profile = "C:\\Program Files\\3S CODESYS\\CODESYS\\Profiles\\CODESYS V3.5 SP5 Patch 2.profile";
	argument = " --profile=\"C:\\Program Files\\3S CODESYS\\CODESYS\\Profiles\\CODESYS V3.5 SP5 Patch 2\" --importDevice --converter=\"{3992c588-7bdb-4a7c-908d-f444808d8cd2}\" ";
	repTool = "RepTool.exe";
	
	if(FileExists(repToolPath+repTool) && FileExists(profile))
	{
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = repTool.c_str();
		parameters = argument+ "\""+strESIoutputXmlFile+"\"";
        ShExecInfo.lpParameters = parameters.c_str();   //avoid white space in path
		//ShExecInfo.lpParameters = (argument+ "\""+strESIoutputXmlFile+"\"").c_str();   //avoid white space in path
		ShExecInfo.lpDirectory = repToolPath.c_str();
		ShExecInfo.nShow = SW_RESTORE;
		//ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		
		ShellExecuteEx(&ShExecInfo);
		
		int ret = WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
		if(ret == WAIT_OBJECT_0)
		{
			//------------Append contents to device.xml------------//
			for(int slaveNo = 0; slaveNo < totalSlave; slaveNo ++)
			{
				if(device_productCode[slaveNo] == 0)
				{
					continue;
				}
				//strDevice = "C:\\ProgramData\\CODESYS\\Devices\\65\\144A_" + IntToHex((int)m_productCode[slaveNo], 8) + "00010111\\Revision%3D16%2300010111\\device.xml";
				strDevice = "C:\\ProgramData\\CODESYS\\Devices\\65\\144A_" + IntToHex((int)m_productCode[slaveNo], 8) + IntToHex((int)m_revisionNum[slaveNo], 8)+
				"\\Revision%3D16%23"+ IntToHex((int)m_revisionNum[slaveNo], 8)+"\\device.xml";
				if(FileExists(strDevice))
				{
					xmlParser(strDevice);
				}
				else
				{
					api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, strDevice+" not exist");
				}
			}
		}
		else
		{
			api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "Generate device.xml fail");
		}
	
	}
	else
	{
		api->displayLogMsg(ECMaster_ERROR_SUCCESS, "RepTool or profile not exist");
	}

}
//---------------------------------------------------------------------------
void __fastcall TformMain::displayExportPrg(int percent, int slaveNo)
{
/*	int total = wait_export_total;
	int exportPercent = (float)(percent +100*slaveNo)/total * 100;
	this->ExportPrgBar->Position = percent +100*slaveNo;
	this->ExportForm->Caption = "Exporting: " + AnsiString(exportPercent) + "%";
	if(this->ExportPrgBar->Position >= this->ExportPrgBar->Max)
	{
		this->ExportPrgBar->Position = 0;
		this->ExportForm->Caption = "Exporting: " + AnsiString(0) + "%";
		this->ExportForm->Hide();
	}*/
	api->displayPrg(this->ExportForm, this->ExportPrgBar, "Exporting: ", percent, slaveNo, wait_export_total);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::createPollingThread()
{
	if(pollingThread == NULL)
	{
		pollingThread = new PollingThread(this, this);
        pollingThread->isEndThread = false;
		pollingThread->Resume();
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMain::releasePollingThread()
{
	if(pollingThread != NULL)
	{
		//pollingThread->Suspend();
        //pollingThread->isSuspend = true;
        pollingThread->isEndThread = true;
        pollingThread->WaitFor();
		pollingThread->Terminate();
		pollingThread = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMain::resumePollingThread()
{
	if(pollingThread != NULL)
	{
		pollingThread->Resume();
        pollingThread->isSuspend = false;
		//api->enableCoE = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMain::suspendPollingThread()
{
	if(pollingThread != NULL)
	{
		//pollingThread->Suspend();
        pollingThread->isSuspend = true;
		//api->enableCoE = true;
	}
}
//---------------------------------------------------------------------------

HRESULT __fastcall TformMain::ExportESI_AO_eps4008(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
    HRESULT hr = E_FAIL;

    if( m_bTargetXmlCreated == false )
    {
    	hr = ESIObjectExport( nSlotNum, &(AO_eps4008[0]),NULL, strESIoutputXmlFile, 0 );
    	m_bTargetXmlCreated = true;
    }
    else
    {
    	hr = ESIObjectExport( nSlotNum, &(AO_eps4008[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
    }

    hr = ESIObjectExport( nSlotNum, &(AO_eps4008[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	
    hr = ESIObjectDT9000Export( nSlotNum, &AO_eps4008_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

    hr = ESIObjectExport( nSlotNum, &(AO_eps4008[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 ); // DTC401_4008
	hr = ESIObjectExport( nSlotNum, &(AO_eps4008[3]), strESIoutputXmlFile, strESIoutputXmlFile, 0 ); // DTC402_4008

    return hr;
}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_DI_eps1132(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
    HRESULT hr = E_FAIL;

    // DI Module
    if( m_bTargetXmlCreated == false )
    {
    	hr = ESIObjectExport( nSlotNum, &(DI_eps1132[0]),NULL, strESIoutputXmlFile, 0 );
    	m_bTargetXmlCreated = true;
    }
    else
    {
    	hr = ESIObjectExport( nSlotNum, &(DI_eps1132[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
    }

    hr = ESIObjectExport( nSlotNum, &(DI_eps1132[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	
	hr = ESIObjectDT9000Export( nSlotNum, &DI_eps1132_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	hr = ESIObjectExport( nSlotNum, &(DI_eps1132[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

    return hr;
}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_AI_eps3032(long nSlotNum,  TCHAR strESIoutputXmlFile[])
{
	HRESULT hr = E_FAIL;
	
	if( m_bTargetXmlCreated == false )
	{
		hr = ESIObjectExport( nSlotNum, &(AI_eps3032[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		hr = ESIObjectExport( nSlotNum, &(AI_eps3032[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	hr = ESIObjectExport( nSlotNum, &(AI_eps3032[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	hr = ESIObjectDT9000Export( nSlotNum, &AI_eps3032_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	hr = ESIObjectExport( nSlotNum, &(AI_eps3032[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	hr = ESIObjectExport( nSlotNum, &(AI_eps3032[3]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	return hr;

}

//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_AI_eps3216(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
	HRESULT hr = E_FAIL;

	if( m_bTargetXmlCreated == false )
	{
		hr = ESIObjectExport( nSlotNum, &(AI_eps3216[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		hr = ESIObjectExport( nSlotNum, &(AI_eps3216[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	hr = ESIObjectExport( nSlotNum, &(AI_eps3216[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	hr = ESIObjectDT9000Export( nSlotNum, &AI_eps3216_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	hr = ESIObjectExport( nSlotNum, &(AI_eps3216[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	hr = ESIObjectExport( nSlotNum, &(AI_eps3216[3]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	return hr;
}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_DO_eps2032(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
	HRESULT hr = E_FAIL;

	// DI Module
	if( m_bTargetXmlCreated == false )
	{
		hr = ESIObjectExport( nSlotNum, &(DO_eps2032[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		hr = ESIObjectExport( nSlotNum, &(DO_eps2032[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	hr = ESIObjectExport( nSlotNum, &(DO_eps2032[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	hr = ESIObjectDT9000Export( nSlotNum, &DO_eps2032_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	hr = ESIObjectExport( nSlotNum, &(DO_eps2032[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	return hr;

}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_DO_eps2132(long nSlotNum, TCHAR strESIoutputXmlFile[] )
{
	short nError = ECMaster_ERROR_SUCCESS;

	// DI Module
	if( m_bTargetXmlCreated == false )
	{
		nError = ESIObjectExport( nSlotNum, &(DO_eps2132[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		nError = ESIObjectExport( nSlotNum, &(DO_eps2132[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	nError = ESIObjectExport( nSlotNum, &(DO_eps2132[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	nError = ESIObjectDT9000Export( nSlotNum, &DO_eps2132_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	nError = ESIObjectExport( nSlotNum, &(DO_eps2132[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	return nError;

}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_RTD_eps3504(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
	HRESULT hr = E_FAIL;

	// DI Module
	if( m_bTargetXmlCreated == false )
	{
		hr = ESIObjectExport( nSlotNum, &(RTD_eps3504[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		hr = ESIObjectExport( nSlotNum, &(RTD_eps3504[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	hr = ESIObjectExport( nSlotNum, &(RTD_eps3504[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	hr = ESIObjectDT9000Export( nSlotNum, &RTD_eps3504_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

#if 1
	hr = ESIObjectExExport( nSlotNum, &(RTD_eps3504_Ex[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
#else
	hr = ESIObjectExport( nSlotNum, &(RTD_eps3504[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
#endif

	return hr;

}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_RO_eps2308(long nSlotNum, TCHAR strESIoutputXmlFile[])
{
	HRESULT hr = E_FAIL;

	// DI Module
	if( m_bTargetXmlCreated == false )
	{
		hr = ESIObjectExport( nSlotNum, &(RO_eps2308[0]), NULL, strESIoutputXmlFile, 0 );
		m_bTargetXmlCreated = true;
	}
	else
	{
		hr = ESIObjectExport( nSlotNum, &(RO_eps2308[0]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );
	}

	hr = ESIObjectExport( nSlotNum, &(RO_eps2308[1]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	hr = ESIObjectDT9000Export( nSlotNum, &RO_eps2308_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	hr = ESIObjectExport( nSlotNum, &(RO_eps2308[2]), strESIoutputXmlFile, strESIoutputXmlFile, 0 );

	return hr;

}
//---------------------------------------------------------------------------
HRESULT __fastcall TformMain::ExportESI_MO_eps7002(long nSlotNum, TCHAR strESIoutputXmlFile[], long nAxisNum )
{
	short nError = ECMaster_ERROR_SUCCESS;

	// DI Module
	if( m_bTargetXmlCreated == false )
	{
		nError = ESIObjectExport( nSlotNum, &(MO_eps7002[0]), NULL, strESIoutputXmlFile, nAxisNum );
		m_bTargetXmlCreated = true;
	}
	else
	{
		nError = ESIObjectExport( nSlotNum, &(MO_eps7002[0]), strESIoutputXmlFile, strESIoutputXmlFile, nAxisNum );
	}

	for(int i = 1; i < 59; i++)
	{
		nError = ESIObjectExport( nSlotNum, &(MO_eps7002[i]), strESIoutputXmlFile, strESIoutputXmlFile, nAxisNum );
	}

	nError = ESIObjectDT9000Export( nSlotNum, &MO_eps7002_Object_DT9000, strESIoutputXmlFile, strESIoutputXmlFile );

	return nError;

}
//---------------------------------------------------------------------------
void __fastcall TformMain::changeToBootStrap()
{
	short nErr;

	nErr = StateMachineRequest(DEVICE_STATE_BOOTSTRAP);
	if( nErr != ECMaster_ERROR_SUCCESS )
	{
		api->displayLogMsg(nErr,"changeToBootStrap() failed");
	}
	waitStateMachine(DEVICE_STATE_BOOTSTRAP);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::changeToInit()
{
	short nErr;
	
	nErr = StateMachineRequest(DEVICE_STATE_INIT);
	if( nErr != ECMaster_ERROR_SUCCESS )
	{
		api->displayLogMsg(nErr, "changeToInit() failed");
	}
	waitStateMachine(DEVICE_STATE_INIT);

}
//---------------------------------------------------------------------------
void __fastcall TformMain::changeToPreOp()
{
	short nErr;
/*
	for ( int i=0;i<100;i++ )
	{
		StateMachineRequest(DEVICE_STATE_PREOP);
		StateMachineRequest(DEVICE_STATE_INIT);
	}
*/
	nErr = StateMachineRequest(DEVICE_STATE_PREOP);
	if( nErr != ECMaster_ERROR_SUCCESS )
	{
		api->displayLogMsg(nErr, "changeToPreOp() failed");
	}
	waitStateMachine(DEVICE_STATE_PREOP);

}
//---------------------------------------------------------------------------

void __fastcall TformMain::waitStateMachine(USHORT status)
{
	USHORT currStatus;
	StateMachineGet(&currStatus);
	while(currStatus != status)
	{
		StateMachineGet(&currStatus);
		//Sleep(100);
		api->delay(100);
	}	
}
//---------------------------------------------------------------------------
void __fastcall TformMain::confirmFoeStatus()
{
	short nErr;
	USHORT wFoEStatus;
	ULONG dwDataLen;
	AnsiString action;
	
	nErr = SendSyncCoEUploadReq( wSlaveNo, sizeof(USHORT), OD_INDEX_BC6000_FoE_STATUS, 0, &wFoEStatus, &dwDataLen );
	if( wFoEStatus != 2 )
	{
		api->displayLogMsg(nErr, "confirmFoeStatus(): FoE download failed, status " + AnsiString(wFoEStatus));
	}
	else
	{
		api->displayLogMsg(nErr, "confirmFoeStatus(): FoE download success, status " + AnsiString(wFoEStatus) + ", please restart EPS system.");
	}

}
//---------------------------------------------------------------------------
void __fastcall TformMain::cleanupChildForm()
{
	for(int i = 0; i < ETHERCAT_SLAVE_MAX; i ++)
	{
		if(slave[i] != NULL)
		{
			slave[i]->Release();
			slave[i] = NULL;
		}
	}
	//api->clearErrMsg();
	api->windowsCnt = 0;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    short nErr;

	releasePollingThread();
	//releaseFoeThread();
	systemTimer->Enabled = false;
	cleanupChildForm();
	
    nErr = StopECMaster();
    OutputDebugString("=========close=========");
    if(nErr != ECMaster_ERROR_SUCCESS)
    {
        ShowMessage("Stop ECat master fail!!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::btnClearClick(TObject *Sender)
{
	api->clearErrMsg();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btnDumpClick(TObject *Sender)
{
	 FILE *FSave;
     USHORT year, month, day, hour, min, sec, msec;
     TDateTime now = TDateTime::CurrentDateTime();
     AnsiString datetime ="";
    if(richEdMain->Lines->Text.IsEmpty())
    {
        return;
    }
     now.DecodeDate(&year, &month, &day);
     now.DecodeTime(&hour, &min, &sec, &msec);
     datetime = IntToStr(year)+"-"+IntToStr(month)+"-"+IntToStr(day)+"_"+IntToStr(hour)+"-"+IntToStr(min)+"-"+IntToStr(sec);
     AnsiString file = "Log_"+datetime+".txt";
	 FSave = fopen(file.c_str(), "w");
	 if(FSave != NULL)
	 {
	 	fprintf(FSave, "%s\n", richEdMain->Lines->Text.c_str());
	 }
	 else
	 {
	 	api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "Dump(): Dump log file fail");
	 }
	 fclose(FSave);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::pollingCycle()
{
	short nErr = ECMaster_ERROR_SUCCESS;
	ULONG m_dwModuleSlot[TOTAL_SLOTS];
	ULONG dwDataLen;
	int scanMode = 1;	//1: scan current module	2:read moudule from flash	3:scan current module and update flash

	for(int slaveNo = 0; slaveNo < m_nSlaveNum; slaveNo ++)
	{
	
		if(slave[slaveNo] == NULL || devType[slaveNo] != AD_SLAVE_TYPE_BC6000 )
		{
			continue;
		}
/*
		nErr = SendSyncCoEDownloadReq(slaveNo, sizeof(ULONG), 0x100C, 1, &scanMode);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "pollingCycle(): Write scan mode fail");
		}
*/	
		if(slave[slaveNo]->isSlaveUpdate)
		{
			slave[slaveNo]->updateSlaveInfo();
		}

		if(slave[slaveNo]->isGetOdClick)
		{
			slave[slaveNo]->getOdProcess();
		}

		if(slave[slaveNo]->isRefreshClick)
		{
			slave[slaveNo]->refreshErrLogTbl();
		}

		if(slave[slaveNo]->isClearClick)
		{
			slave[slaveNo]->clearFlash();
		}

		if(api->windowsCnt == 0)
		{
			continue;
		}
	
		for(int i = 0; i < TOTAL_SLOTS; i++ )
		{
			m_dwModuleSlot[i] = 0x00000000;
			nErr = SendSyncCoEUploadReq(slaveNo, sizeof(ULONG), 0x100C, i+2, &m_dwModuleSlot[i], &dwDataLen);
			
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
					api->displayLogMsg(nErr, "pollingCycle(): Get slave "+AnsiString(slaveNo)+ ", slot " +AnsiString(i)+" module fail, index 0xF030:"+AnsiString(i));
					continue;
			}
			else
			{
				if(m_isDebug)
				{
					api->displayLogMsg(nErr, "pollingCycle(): Slave "+AnsiString(slaveNo)+" slot " +AnsiString(i)+" ("+ IntToHex((int)m_dwModuleSlot[i], 4)+")");
				}
			}
	
			switch(m_dwModuleSlot[i])
			{
				case EPS1132_ID:	//DI32
					if(slave[slaveNo]->eps1132[i] != NULL && slave[slaveNo]->eps1132[i]->Showing)
					{
						slave[slaveNo]->eps1132[i]->readData();
					}
					break;
				case EPS3032_ID:	//AI32
					if(slave[slaveNo]->eps3032[i] != NULL && slave[slaveNo]->eps3032[i]->Showing)
					{
						slave[slaveNo]->eps3032[i]->readData();
					}
					break;
				case EPS3216_ID:	//AI16
					if(slave[slaveNo]->eps3216[i] != NULL && slave[slaveNo]->eps3216[i]->Showing)
					{
						slave[slaveNo]->eps3216[i]->readData();
					}
					break;
				case EPS3504_ID:	//RTD
					if(slave[slaveNo]->eps3504[i] != NULL && slave[slaveNo]->eps3504[i]->Showing)
					{
						slave[slaveNo]->eps3504[i]->readData();
					}
					break;
				case EPS4008_ID:	//AO8
					if(slave[slaveNo]->eps4008[i] != NULL && slave[slaveNo]->eps4008[i]->Showing)
					{
						/*if(slave[slaveNo]->eps4008[i]->isWrite)
						{
							slave[slaveNo]->eps4008[i]->writeData();
						}
						else
						{
							slave[slaveNo]->eps4008[i]->readData();
						}*/
						if(slave[slaveNo]->eps4008[i]->state == 0)
						{
							slave[slaveNo]->eps4008[i]->readData();
						}
						else if(slave[slaveNo]->eps4008[i]->state == 2)
						{
							slave[slaveNo]->eps4008[i]->writeData();
						}
						else
						{
						}
					}
					break;
				case EPS2032_ID:	//DO32
					if(slave[slaveNo]->eps2032[i] != NULL && slave[slaveNo]->eps2032[i]->Showing)
					{
						if(slave[slaveNo]->eps2032[i]->state == 0)
						{
							slave[slaveNo]->eps2032[i]->readData();
						}
						else
						{
							slave[slaveNo]->eps2032[i]->writeData();
						}
					}
					break;
				case EPS2132_ID:	//DO32
					if(slave[slaveNo]->eps2132[i] != NULL && slave[slaveNo]->eps2132[i]->Showing)
					{
						if(slave[slaveNo]->eps2132[i]->state == 0)
						{
							slave[slaveNo]->eps2132[i]->readData();
						}
						else
						{
							slave[slaveNo]->eps2132[i]->writeData();
						}
					}
					break;
				case EPS2308_ID:	//RO
					if(slave[slaveNo]->eps2308[i] != NULL && slave[slaveNo]->eps2308[i]->Showing)
					{
						if(slave[slaveNo]->eps2308[i]->state == 0)
						{
							slave[slaveNo]->eps2308[i]->readData();
						}
						else
						{
							slave[slaveNo]->eps2308[i]->writeData();
						}
					}
					break;
				case EPS7002_ID:
					if(slave[slaveNo]->eps7002[i] != NULL && slave[slaveNo]->eps7002[i]->Showing)
					{
                        if(slave[slaveNo]->eps7002[i]->state == 0)
                        {
                        	slave[slaveNo]->eps7002[i]->readData();
                        }
						else if(slave[slaveNo]->eps7002[i]->state == 2)
						{
							slave[slaveNo]->eps7002[i]->writeData();
						}
						else
						{
						}
					}
				default:
					break;
			}
	
		}
	
	}
	
	//Sleep(COE_UPDATE_INTERVAL);
	api->delay(COE_UPDATE_INTERVAL);


}
//---------------------------------------------------------------------------

void __fastcall TformMain::systemTimerTimer(TObject *Sender)
{
	for(int slaveNo = 0; slaveNo < m_nSlaveNum; slaveNo ++)
	{
		if(slave[slaveNo] == NULL || devType[slaveNo] != AD_SLAVE_TYPE_BC6000)
		{
			continue;
		}

		if(slave[slaveNo]->isFoeClick)
		{
            systemTimer->Enabled = false;
			enableComponent(false);
			pnlMac->Enabled = false;
		    releasePollingThread();

			changeToInit();
			changeToBootStrap();
			slave[slaveNo]->foeDownload();
			changeToInit();
			changeToPreOp();

			confirmFoeStatus();

		    createPollingThread();
			enableComponent(true);
			pnlMac->Enabled = true;
            systemTimer->Enabled = true;

		}

	}
}
//---------------------------------------------------------------------------
void __fastcall TformMain::xmlParser(AnsiString szFileName)
{
    AnsiString localName;

    int parameter_cnt = 0;
	PARAMETER	*para;

    _di_IXMLNode XN;
    _di_IXMLNode XN_1st_child;
    _di_IXMLNode XN_2nd_child;
    _di_IXMLNode XN_3rd_child;
    _di_IXMLNode XN_4th_child, XN_4th_sibling;
    _di_IXMLNode XN_5th_child;
    _di_IXMLNode XN_6th_child;
/*
   try
   {
      XMLDoc->LoadFromFile( URLEncode(szFileName) );
   }
   catch(Exception &e)
   {
      ShowMessage(e.Message);
   }
*/
	XMLDoc->LoadFromFile( URLEncode(szFileName) );

   if(!XMLDoc->IsEmptyDoc())
   {
   		XN=XMLDoc->DocumentElement;

        XN_1st_child = XN->ChildNodes->FindNode("Device");    //first child
        if(XN_1st_child == NULL)
        {
        	api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): 1st node NULL");
            return;
        }

        XN_2nd_child = XN_1st_child->ChildNodes->FindNode("Connector");    //second child
        if(XN_2nd_child == NULL)
        {
        	api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): 2nd node NULL");
            return;
        }

        XN_3rd_child = XN_2nd_child->ChildNodes->FindNode("HostParameterSet"); //third child
        if(XN_3rd_child == NULL)
        {
        	api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): 3rd node NULL");
            return;
        }

        XN_4th_child = XN_3rd_child->ChildNodes->FindNode("Parameter");	//fourth child
        if(XN_4th_child == NULL)
        {
        	api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): 4th node NULL");
            return;
        }

		//count parameter number
        XN_4th_sibling = XN_4th_child;	
        while(XN_4th_sibling != NULL)
        {
            localName = XN_4th_sibling->GetLocalName();
            if(localName == "Parameter")
            {
                XN_4th_sibling = XN_4th_sibling->NextSibling();
                parameter_cnt ++;
            }
            else if(localName == "ParameterSection")    //determine this file has done before or not
            {
                XN_4th_sibling = XN_4th_sibling->NextSibling();
                if(XN_4th_sibling != NULL)
                {
                    localName = XN_4th_sibling->GetLocalName();
                    if(localName == "Parameter")
                    {
						api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): device has done before");
                        return;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

		XN_4th_sibling = XN_4th_child;
		para = new PARAMETER[parameter_cnt];

		for(int i = 0; i < parameter_cnt; i++)
		{
			para[i].parameterId = 2147483648+i;
			para[i].type = "std:POINTER TO DINT";
			para[i].attr.offlineaccess = "none";
            para[i].attr.onlineaccess = "none";
            para[i].attr.download = "true";
			//get original "parameter" info
			para[i].def.text = (WideString)XN_4th_sibling->GetAttribute(WideString("ParameterId"));
            para[i].name.name = "local:Output";
			XN_5th_child = XN_4th_sibling->ChildNodes->FindNode("Name");
			para[i].name.text = "Pointer to " + XN_5th_child->GetText();
			para[i].comment = "!--Default to the parameter id of the PDO " + XN_5th_child->GetText()+"--";
			para[i].custom = "POINTER_TO_IOCONFIGADDRESS";

			addSection(XN_3rd_child, para[i]);

			XN_4th_sibling = XN_4th_sibling->NextSibling();		//move to next "parameter"
		}

        XMLDoc->SaveToFile( URLEncode(szFileName) );
		api->displayLogMsg(ECMaster_ERROR_SUCCESS, "xmlParser(): "+szFileName+" done");        //XMLDoc->SaveToFile("modified.xml");

		delete[] para;
        //delete(para);

   }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::addSection(_di_IXMLNode XN, PARAMETER para)
{
	_di_IXMLNode XN_1st_child;
	_di_IXMLNode XN_2nd_child;
	_di_IXMLNode XN_3rd_child;
	
    XN_1st_child = XN->AddChild("Parameter");
    XN_1st_child->SetAttribute("ParameterId", WideString(para.parameterId));
    XN_1st_child->SetAttribute("type", para.type);

    XN_2nd_child = XN_1st_child->AddChild("Attributes");
    XN_2nd_child->SetAttribute("offlineaccess", para.attr.offlineaccess);
    XN_2nd_child->SetAttribute("onlineaccess", para.attr.onlineaccess);
    XN_2nd_child->SetAttribute("download", para.attr.download);

    XN_2nd_child = XN_1st_child->AddChild("Default");
    XN_2nd_child->SetText(para.def.text);
	//XN_2nd_child->SetText(para.comment);

	//XN_2nd_child = XN_1st_child->AddChild("comment");

    XN_2nd_child = XN_1st_child->AddChild("Name");
    XN_2nd_child->SetAttribute("name", para.name.name);
    XN_2nd_child->SetText(para.name.text);

    XN_2nd_child = XN_1st_child->AddChild("Custom");

    XN_3rd_child = XN_2nd_child->AddChild(para.custom);
}
//---------------------------------------------------------------------------



