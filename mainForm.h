//---------------------------------------------------------------------------

#ifndef mainFormH
#define mainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>

#include <iphlpapi.h>
#include "slaveForm.h"
#include "infoForm.h"
//#include "ESI_Initial.c"
#include "stdio.h"
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
//#include <IdBaseComponent.hpp>
#include <IdCoder.hpp>
//#include <IdCoder3To4.hpp>



//for tree level
#define  Level_Root                      (0)
#define  Level_CardType                  (1)
#define  Level_CardNo                    (2)
#define  Level_Function                  (3)
#define  Level_AxisNo

#define  MAX_NIC_SUPPORT                25

#define OD_INDEX_BC6000_FoE_STATUS                       0x100F



typedef struct _NIC_INFO_
{
	char NICDescription[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
	BYTE MacAddress[MAX_ADAPTER_ADDRESS_LENGTH];
	UINT AddressLength;
} NIC_INFO, *PNIC_INFO;

typedef struct _ATTRIBUTES_
{
	WideString			offlineaccess;
	WideString 			onlineaccess;
	WideString			download;
	WideString			channel;
} ATTRIBUTES;

typedef struct _DEFAULT_
{
    WideString     text;
} DEFAULT;

typedef struct _NAME_
{
    WideString    name;
    WideString    text;
} NAME;

typedef struct _DESCRIPTION_
{
    ULONG         name;
    AnsiString    text;
} DESCRIPTION;

typedef struct _PARAMETER_
{
	ULONG				parameterId;
	WideString 			type;
    ATTRIBUTES          attr;
    DEFAULT             def;
	WideString			comment;
    NAME                name;
    DESCRIPTION         descrip;
	WideString			custom;
} PARAMETER;


//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *mainMenu;
        TMenuItem *File1;
        TMenuItem *Exit1;
        TMenuItem *About1;
        TMenuItem *LinkMasterPro1;
        TImageList *imgListMain;
        TToolBar *toolBarMain;
        TToolButton *btnScan;
        TToolButton *btnExport;
        TTreeView *treeMain;
        TRichEdit *richEdMain;
    TSplitter *spliterVertical;
        TSaveDialog *SaveDlgExport;
        TPanel *pnlMac;
        TComboBox *cbEthernet;
        TLabel *Label1;
        TPanel *pnlLog;
        TButton *btnClear;
        TButton *btnDump;
    TTimer *systemTimer;
    TXMLDocument *XMLDoc;
    TSplitter *spliterHorizontal;
    TEdit *edPositiveJitter;
    TEdit *edNegativeJitter;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
        void __fastcall TreeMainMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall LinkMasterPro1Click(TObject *Sender);
        void __fastcall btnScanClick(TObject *Sender);
        void __fastcall btnExportClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btnClearClick(TObject *Sender);
        void __fastcall btnDumpClick(TObject *Sender);
    void __fastcall systemTimerTimer(TObject *Sender);
    void __fastcall cbEthernetChange(TObject *Sender);
private:	// User declarations
        int m_nSlaveNum;	// total slave number
        int m_nAdlinkSlaveNum;	//total adlink slave number
		int wSlaveNo;		//current slave No
		unsigned long m_productCode[ETHERCAT_SLAVE_MAX], device_productCode[ETHERCAT_SLAVE_MAX];
		unsigned long m_revisionNum[ETHERCAT_SLAVE_MAX];
        bool m_odVisible, m_isDebug;
	int ECATSlaveNo[ETHERCAT_SLAVE_MAX];
	USHORT devType[ETHERCAT_SLAVE_MAX];
    TformSlave  *slave[ETHERCAT_SLAVE_MAX];
	TProgressBar *ExportPrgBar;
	TForm *ExportForm;
	TLabel *lbPercent;
	API_Class *api;
	UINT wait_export_percent, wait_export_total;
    TformInfo   *info;
    TTreeNode *tnMain, *tnMaster[ETHERCAT_SLAVE_MAX];
    TTreeNode *tn;
	AnsiString systemPath;
    AnsiString ESITemplatePathName;
    ULONG_PTR m_Category_Address;
	bool isECMasterOpen;
	bool __fastcall ReadIni();
	bool __fastcall BuildAdapterListing();
	int __fastcall GetNICMac( BYTE *pMacAddr, UINT MacAddrLength, int index);
	void __fastcall GetMacProc(int index);
    void __fastcall enableComponent(bool enabled);
	void __fastcall checkSameDevice(int total_slave);
	void __fastcall readModuleIdentity(int slaveNo, unsigned long *productCode, unsigned long *revisionNum);
	void __fastcall cleanupChildForm();
	void __fastcall displayExportPrg(int percent, int slaveNo);
	void __fastcall createPollingThread();
	void __fastcall releasePollingThread();
	void __fastcall resumePollingThread();
	void __fastcall suspendPollingThread();

	bool __fastcall ESISingleDeviceAdd(int slaveNo, ULONG dwModuleSlot[], unsigned long dwProductCode, unsigned long dwRevisionNum, USHORT wFileOperation, TCHAR strESIoutputXmlFile[], int bufLength);
	void __fastcall ESI_Default_Data_Update( ESI_OBJECT *pESIObject, ESI_DEVICE_PDO *pESIPDO, long nSlotNum, long nAxisNum );
	void __fastcall generateDeviceXml(int totalSlave, AnsiString strESIoutputXmlFile);

    HRESULT __fastcall ExportESI_AO_eps4008(long nSlotNum, TCHAR strESIoutputXmlFile[]);
    HRESULT __fastcall ExportESI_DI_eps1132(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_AI_eps3032(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_AI_eps3216(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_DO_eps2032(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_DO_eps2132(long nSlotNum, TCHAR strESIoutputXmlFile[] );
	HRESULT __fastcall ExportESI_RTD_eps3504(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_RO_eps2308(long nSlotNum, TCHAR strESIoutputXmlFile[]);
	HRESULT __fastcall ExportESI_MO_eps7002(long nSlotNum, TCHAR strESIoutputXmlFile[], long nAxisNum );

	void __fastcall changeToBootStrap();
	void __fastcall changeToInit();
	void __fastcall changeToPreOp();
	void __fastcall waitStateMachine(USHORT status);
	void __fastcall confirmFoeStatus();
	void __fastcall calculateDcSettings();

	void __fastcall xmlParser(AnsiString szFileName);
	void __fastcall addSection(_di_IXMLNode XN, PARAMETER para);


protected:
    BYTE m_bMacAddr[6];
    bool m_bTargetXmlCreated;
    NIC_INFO m_AvailNIC[MAX_NIC_SUPPORT];
	//int m_nCardIndex;
	int m_nCardNum;
public:		// User declarations
        __fastcall TformMain(TComponent* Owner);
		void __fastcall pollingCycle();

};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif
