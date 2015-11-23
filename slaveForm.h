//---------------------------------------------------------------------------

#ifndef slaveFormH
#define slaveFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <jpeg.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <Buttons.hpp>

#include "ESI_Export.h"
#include "BC_Eeprom.h"
#include "AdMaster.h"
#include "di1132_pin.h"
#include "ao4008_pin.h"
#include "ai3032_pin.h"
#include "ai3216_pin.h"
#include "do2032_pin.h"
#include "ro2308_pin.h"
#include "rtd3504_pin.h"
#include "mo7002_pin.h"

#define _BOOL_			"BOOL"
#define _INT8_			"INT8"
#define _INT32_			"INT32"
#define _UINT8_			"UINT8"
#define _UINT16_		"UINT16"
#define _UINT32_		"UINT32"
#define _STRING_		"STRING"
#define _ADLINK_STRING1_	"ADLINK_STRING1"
#define _ADLINK_STRING2_	"ADLINK_STRING2"

//#define _ADLINK_		"ADLINK"
#define _EPS_			"EPS_#"

#define  TOTAL_SLOTS     4
#define EPS4008_ID  0x4008  // AO eps-4008
#define EPS1132_ID  0x1132  // DI eps-1132
#define EPS3032_ID  0x3032  // AI eps-3032
#define EPS3216_ID  0x3216  // AI eps-3216
#define EPS2032_ID  0x2032  // DO eps-2032
#define EPS2132_ID  0x2132  // DO eps-2132
#define EPS3504_ID  0x3504 //  RTD eps-3504
#define EPS2308_ID  0x2308  // RO eps-2308
#define EPS7002_ID	0x7002	// MO eps-7002

#define DELAY_AFTER_FoE_COMPLETION 120000 // waiting for 120 seconds (120,000 millisecond)

#ifndef ERR_FoE_ERRS
#define	ERR_FoE_ERRS					0x8000

#define FoE_ERR_DATA_CHECKSUM  			(0x00+ERR_FoE_ERRS)
#define FoE_ERR_FILE_NOT_FOUND  		(0x01+ERR_FoE_ERRS)
#define FoE_ERR_DISABLE_RESPONSE		(0x04+ERR_FoE_ERRS)
#define FoE_ERR_INVALID_PACKET_NUM		(0x05+ERR_FoE_ERRS)
#define FoE_ERR_NOT_IN_BOOTSTRAP		(0x08+ERR_FoE_ERRS)
#define FoE_ERR_INCORRECT_FILENAME		(0x09+ERR_FoE_ERRS)

#endif // ERR_FoE_ERRS

/*
#define OD_INDEX_BC6000_MANUFACTORY_VER       0x100B
#define OD_SUBINDEX_BC6000_HW                   0x03
#define OD_SUBINDEX_BC6000_FW                   0x04
#define OD_SUBINDEX_BC6000_SW                   0x01


#define OD_INDEX_BC6000_IDENTITY              0x1018
#define OD_SUBINDEX_BC6000_VENDOR_ID            0x01
#define OD_SUBINDEX_BC6000_PRODUCT_CODE         0x02
#define OD_SUBINDEX_BC6000_REVISION_NUM         0x03
#define OD_SUBINDEX_BC6000_SERIAL_NUM           0x04


#define OD_BASE_INDEX_MODULE_IDENTIFICATION   0xC000
#define OD_SUBINDEX_MODULE_DEVICE_TYPE          0x04
#define OD_SUBINDEX_MODULE_VENDOR_ID            0x05
#define OD_SUBINDEX_MODULE_PRODUCT_CODE         0x06
#define OD_SUBINDEX_MODULE_REVISION_NUM         0x07
#define OD_SUBINDEX_MODULE_SERIAL_NUM           0x08
#define OD_SUBINDEX_MODULE_HW_VER               0x14
#define OD_SUBINDEX_MODULE_FW_VER               0x15

#define OD_BASE_INDEX_MODULE_SETTINGS   		0xC001
#define OD_BASE_INDEX_MODULE_CALIBRATION   		0xC002
*/
static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,\
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,\
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,\
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,\
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,\
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,\
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,\
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,\
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,\
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,\
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,\
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,\
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,\
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,\
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,\
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,\
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,\
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,\
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,\
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,\
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,\
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,\
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,\
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,\
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,\
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,\
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,\
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,\
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,\
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,\
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,\
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0 };

typedef struct _OD_INFO
{
        UINT index;
        UINT subIndex;
        char moc[16];
        char datatype[16];
        //siString datatype;
        UINT dataSize;
} OD_INFO;

typedef struct _BC_DISPLAY_INFO
{
	UINT vendorID;
	UINT productCode;
	UINT revisionNum;
	UINT serialNum;
	UINT hwVer;
	UINT fwVer;
	UINT swVer;
	UINT adlinkSerial[2];
	UINT errCode;

} BC_DISPLAY_INFO;

typedef struct _AO_DISPLAY_INFO
{
	UINT moduleIdent;
	UINT hwVer;
	UINT swVer;
	UINT adlinkSerial[2];
	UINT errCode;

} AO_DISPLAY_INFO;

typedef struct _DI_DISPLAY_INFO
{
	UINT moduleIdent;
	UINT hwVer;
	UINT swVer;
	UINT adlinkSerial[2];
	UINT errCode;

} DI_DISPLAY_INFO;

typedef struct _SLOT_DISPLAY_INFO
{
	AnsiString moduleName;
	AnsiString moduleType;
	UINT moduleIdent;
	UINT hwVer;
	UINT swVer;
	UINT adlinkSerial[2];
	UINT errCode;

} SLOT_DISPLAY_INFO;

typedef struct _ERR_LOG_INFO
{
	UINT bit;
	char err[128];
} ERR_LOG_INFO;

//---------------------------------------------------------------------------
class TformSlave : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *tbshtBp;
        TTabSheet *tbshtBc;
        TImage *imgBc;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *edBcInfo0;
        TEdit *edBcInfo1;
        TEdit *edBcInfo2;
        TEdit *edBcInfo3;
        TEdit *edBcInfo4;
        TEdit *edBcInfo5;
        TEdit *edBcInfo6;
        TEdit *edBcInfo7;
        TEdit *edBcInfo8;
    TTabSheet *tbshtSlot0;
    TImage *imgSlot0;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
    TEdit *edMod0;
    TEdit *edHwVer0;
    TEdit *edSwVer0;
    TEdit *edAdlinkSerial0;
    TEdit *edErr0;
    TTabSheet *tbshtSlot1;
    TImage *imgSlot1;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
    TEdit *edMod1;
    TEdit *edHwVer1;
    TEdit *edSwVer1;
    TEdit *edAdlinkSerial1;
    TEdit *edErr1;
    TTabSheet *tbshtSlot2;
    TImage *imgSlot2;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
    TEdit *edMod2;
    TEdit *edHwVer2;
    TEdit *edSwVer2;
    TEdit *edAdlinkSerial2;
    TEdit *edErr2;
    TTabSheet *tbshtSlot3;
    TImage *imgSlot3;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
    TEdit *edMod3;
    TEdit *edHwVer3;
    TEdit *edSwVer3;
    TEdit *edAdlinkSerial3;
    TEdit *edErr3;
        TOpenDialog *OpenDialogFoe;
        TBitBtn *btnFoe;
        TBitBtn *btnGetOd;
    TValueListEditor *vleCardInfo;
        TImage *imgBp;
    TImage *imgBpSlot0;
    TImage *imgBpSlot1;
    TImage *imgBpSlot2;
    TImage *imgBpSlot3;
    TEdit *edModType0;
        TLabel *Label30;
    TEdit *edModType1;
    TEdit *edModType2;
    TEdit *edModType3;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
    TTabSheet *tbshtDcDiag;
    TImage *imgDcPara;
    TTabSheet *tbshtDcPara;
    TGroupBox *gbPdo;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label45;
    TLabel *Label46;
    TEdit *edPdoData0;
    TEdit *edPdoData1;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TEdit *edPdoData2;
    TEdit *edPdoData3;
    TEdit *edPdoData4;
    TEdit *edPdoData5;
    TEdit *edPdoData6;
    TEdit *edPdoData7;
    TEdit *edPdoData8;
    TEdit *edPdoData9;
    TEdit *edPdoData10;
    TEdit *edPdoData11;
    TEdit *edPdoData12;
    TEdit *edPdoData13;
    TEdit *edPdoData14;
    TEdit *edPdoData15;
    TEdit *edPdoData16;
    TEdit *edPdoData17;
    TEdit *edPdoData18;
    TEdit *edPdoData19;
    TEdit *edPdoData20;
    TEdit *edPdoData21;
    TGroupBox *gbDcPara;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label49;
    TLabel *Label50;
    TEdit *edPdoData22;
    TEdit *edPdoData23;
    TEdit *edPdoData24;
    TEdit *edPdoData25;
    TTabSheet *tbshtErrLog;
    TRichEdit *richEdErr;
    TValueListEditor *vleErrLog;
    TButton *btnRefresh;
    TButton *btnClear;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnFoeClick(TObject *Sender);
        void __fastcall btnGetOdClick(TObject *Sender);
        void __fastcall imgBpSlot0Click(TObject *Sender);
        void __fastcall imgBpSlot1Click(TObject *Sender);
        void __fastcall imgBpSlot2Click(TObject *Sender);
        void __fastcall imgBpSlot3Click(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnRefreshClick(TObject *Sender);
    void __fastcall btnClearClick(TObject *Sender);
    void __fastcall vleErrLogSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
private:	// User declarations
        int wSlaveNo;
		int adlinkNo;
		API_Class *api;
        ULONG m_dwModuleSlot[TOTAL_SLOTS];
		AnsiString *nData, *nSlotData, *nDcData;
		AnsiString systemPath;
		TProgressBar *foePrgBar;
		TForm *foePrgForm;
		UINT foeTotal;
		int slotToMo[TOTAL_SLOTS];
		
        OD_INFO *p_od_info, *p_ao_info, *p_ai16_info, *p_ai32_info, *p_di32_info, *p_do32_info;
		OD_INFO *p_ro_info, *p_rtd_info, *p_mo_info, *p_general_bc_info, *p_general_slot_info, *p_dc_parameter_info;
		ERR_LOG_INFO *p_err_info;
        int od_size, ao_size, ai16_size, ai32_size, di32_size, do32_size, ro_size, rtd_size, mo_size, general_bc_size, general_slot_size, dc_parameter_size, err_info_size;
        //void __fastcall showPinPage(int slot);
        void __fastcall enableComponent(bool enabled);
        void __fastcall updateBcInfo(AnsiString *nData, long nErr);
		void __fastcall updateDcInfo(AnsiString *nData, int size, long nErr);
		void __fastcall updateSlotInfo(int slot, AnsiString img, AnsiString *nSlotData, long nErr);
        void __fastcall clearSlotInfo(int slot, AnsiString img);

        void __fastcall displayOdListTbl(USHORT wSlaveNo, UINT dataSize, UINT idx, UINT subIdx, char *dataType);
		short __fastcall odToString(USHORT wSlaveNo, UINT dataSize, UINT idx, UINT subIdx, char *dataType, AnsiString *str, AnsiString *err);
		void __fastcall displayFoePrg(int percent);

        
public:		// User declarations
	
		TformDi1132 *eps1132[TOTAL_SLOTS];
		TformAo4008 *eps4008[TOTAL_SLOTS];
		TformAi3032 *eps3032[TOTAL_SLOTS];
		TformAi3216 *eps3216[TOTAL_SLOTS];
		TformDo2032 *eps2032[TOTAL_SLOTS];
		TformDo2032 *eps2132[TOTAL_SLOTS];
		TformRo2308 *eps2308[TOTAL_SLOTS];
		TformRtd3504 *eps3504[TOTAL_SLOTS];
		TformMo7002 *eps7002[TOTAL_SLOTS];

        __fastcall TformSlave(TComponent* Owner);
        void __fastcall updateSlaveNum(int wSlaveNo, int adlinkNo, API_Class *api);
        void __fastcall updateSlaveInfo();
		bool isSlaveUpdate;
		bool isFoeClick;
        bool isGetOdClick;
		bool isRefreshClick;
		bool isClearClick;
		void __fastcall foeDownload();
        void __fastcall getOdProcess();
		void __fastcall displayOdFunc(bool isShow);
		void __fastcall showSlotPage(int slot);
		void __fastcall refreshErrLogTbl();
		void __fastcall clearFlash();
		void __fastcall selectCell(int col, int row);
		void __fastcall errBit2Msg(unsigned long long value);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSlave *formSlave;
//---------------------------------------------------------------------------
#endif
