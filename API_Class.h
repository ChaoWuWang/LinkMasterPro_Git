#ifndef API_ClassH
#define API_ClassH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "AdMaster.h"
#include "time.h"
#include "math.h"
#include "DBUILib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>

#define OD_SHIFT	1

#define AO_NUM			8
#define AI16_NUM		16
#define AI32_NUM		32
#define DO32_NUM		32
#define DI32_NUM		32
#define RO_NUM			8
#define RTD_NUM			4
#define MO_NUM			8

#define  MAX_WINDOW_SUPPORT		4

#define _OUTPUT_DATA_ADDRESS_	0x3000
#define _INPUT_DATA_ADDRESS_	0x4000


#define OD_INDEX_BC6000_MANUFACTORY_VER       0x100B
#define OD_SUBINDEX_BC6000_HW                   0x03
#define OD_SUBINDEX_BC6000_FW                   0x04
#define OD_SUBINDEX_BC6000_SW                   0x01


#define OD_INDEX_BC6000_IDENTITY              0x1018
#define OD_SUBINDEX_BC6000_VENDOR_ID            0x01
#define OD_SUBINDEX_BC6000_PRODUCT_CODE         0x02
#define OD_SUBINDEX_BC6000_REVISION_NUM         0x03
#define OD_SUBINDEX_BC6000_SERIAL_NUM           0x04

#if OD_SHIFT
#define OD_BASE_INDEX_MODULE_IDENTIFICATION   0x5000
#else
#define OD_BASE_INDEX_MODULE_IDENTIFICATION   0xC000
#endif
#define OD_SUBINDEX_MODULE_DEVICE_TYPE          0x04
#define OD_SUBINDEX_MODULE_VENDOR_ID            0x05
#define OD_SUBINDEX_MODULE_PRODUCT_CODE         0x06
#define OD_SUBINDEX_MODULE_REVISION_NUM         0x07
#define OD_SUBINDEX_MODULE_SERIAL_NUM           0x08
#define OD_SUBINDEX_MODULE_HW_VER               0x14
#define OD_SUBINDEX_MODULE_FW_VER               0x15

#if OD_SHIFT
#define OD_BASE_INDEX_MODULE_SETTINGS   		0x5001
#define OD_BASE_INDEX_MODULE_CALIBRATION   		0x5002
#else
#define OD_BASE_INDEX_MODULE_SETTINGS   		0xC001
#define OD_BASE_INDEX_MODULE_CALIBRATION   		0xC002
#endif

#define COMPONENT_EDIT                      (0)
#define COMPONENT_SCROBAR                   (1)
#define COMPONENT_ILABEL                    (2)
#define COMPONENT_BUTTON                    (3)
#define COMPONENT_CHECKBOX                  (4)
#define COMPONENT_DBOOLEAN                  (5)
#define COMPONENT_COMBOBOX					(6)
#define COMPONENT_SWITCH					(7)

#define HEIGHT_INTERNAL                     (5)

#define COE_UPDATE_INTERVAL					600
//#define COE_UPDATE_INTERVAL					3000

#define CONFIGURATION_OBJECTS_AREA			0x400
#define CONFIGURATION_MO_AREA				0x1000

class API_Class
{
private:
	
public:
    __fastcall API_Class(TRichEdit *edErr);
	TRichEdit	*richEdErr;
	int windowsCnt;

	void __fastcall delay(unsigned long iMilliSeconds);
	void __fastcall displayMark(AnsiString mark);
	void __fastcall displayLogMsg(short err, AnsiString msg);
	void __fastcall clearErrMsg();
	void __fastcall displayPrg(TForm *frm, TProgressBar *prgBar, AnsiString title, int percent, int no, UINT total);
	void __fastcall setCommProperties(TForm *frm, int height, int width);

	short __fastcall writeSpecArea(USHORT wSlaveNo, int slot , UINT baseAddr, UINT area, UINT digital);
	short __fastcall readSpecArea(USHORT wSlaveNo, int slot, UINT baseAddr, UINT area, UINT *digital);
	short __fastcall writeMoDigital(USHORT wSlaveNo, int moNum, UINT baseAddr, UINT digital);
	short __fastcall readMoDigital(USHORT wSlaveNo, int moNum, UINT baseAddr, UINT *digital);
    short __fastcall writeDigital(USHORT wSlaveNo, int slot, UINT baseAddr, UINT digital);
    short __fastcall readDigital(USHORT wSlaveNo, int slot, UINT baseAddr, UINT *digital);
    short __fastcall writeVoltage(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *vol);
    short __fastcall readVoltage(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *vol);
	short __fastcall readCurrent(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, float *current);
    short __fastcall writeRaw(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, USHORT *raw);
    short __fastcall readRaw(USHORT wSlaveNo, int slot, int subTotal, UINT baseAddr, USHORT *raw);
	//short __fastcall writeSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT raw);
	short __fastcall writeSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, UINT raw);
	//short __fastcall readSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT *raw);
	short __fastcall readSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, UINT *raw);
	//short __fastcall readSingleRaw(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, BYTE *raw);
	short __fastcall writeWire(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT wire);
	short __fastcall readWire(USHORT wSlaveNo, int slot, int subIdx, UINT baseAddr, USHORT *wire);
    float __fastcall rawToVol(USHORT raw);
    USHORT __fastcall volToRaw(float vol);
	float __fastcall rawToOhm(UINT raw, USHORT wire);
	UINT __fastcall ohmToRaw(float ohm);
	float __fastcall rawToCurrent(USHORT raw);
	USHORT __fastcall CurrentToRaw(float current);
	ULONG __fastcall convertEndian(ULONG num);

	AnsiString __fastcall GetDirectory(AnsiString path);
	AnsiString __fastcall GetFileName(AnsiString path);
	AnsiString __fastcall GetExtension(AnsiString path);
	AnsiString __fastcall AutoAddExtension(AnsiString path, AnsiString ext);
	
	__fastcall __ArrangeComponent( void *Object, int type, int Count, int height, int width, int top, int left, bool is2Col );
};

extern PACKAGE API_Class *api;

#endif