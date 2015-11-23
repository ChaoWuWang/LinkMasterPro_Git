//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "slaveForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformSlave *formSlave;

//---------------------------------------------------------------------------
__fastcall TformSlave::TformSlave(TComponent* Owner)
        : TForm(Owner)
{
	//General items info
	static OD_INFO general_bc_info[] =
	{
		{0x1008, 0x00,		"M",		_STRING_,	32},//device name
		{OD_INDEX_BC6000_IDENTITY, 0x01,		"M",		_UINT32_,	sizeof(UINT)},//vendor ID
		{OD_INDEX_BC6000_IDENTITY, 0x02,		"M",		_UINT32_,	sizeof(UINT)},//product code
		{OD_INDEX_BC6000_IDENTITY, 0x03,		"M",		_UINT32_,	sizeof(UINT)},//revision number
		{OD_INDEX_BC6000_IDENTITY, 0x04,		"M",		_UINT32_,	sizeof(UINT)},//serial number
		
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x03,    "",    _UINT32_,	 sizeof(UINT)},//hw ver
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x01,    "",    _UINT32_,	 sizeof(UINT)},//fw ver
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x04,    "",    _UINT32_,	 sizeof(UINT)},//sw ver
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x05,    "",    _ADLINK_STRING1_,	32},//adlink serial1
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x06,    "",    _ADLINK_STRING2_, 	32},//adlink serial2
	};
	p_general_bc_info = general_bc_info;
	general_bc_size = sizeof(general_bc_info)/sizeof(OD_INFO);

	static OD_INFO general_slot_info[] =
	{
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x03,		"",		_STRING_,	32},//module name
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x02,		"",		_STRING_,	32},//module type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x0A,		"",		_UINT32_,	sizeof(UINT)},//module ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x14,		"",		_UINT32_,	sizeof(UINT)},//hw ver
		//{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x16,		"O",	_UINT32_,	sizeof(UINT)},//sw ver
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x15,		"O",	_UINT32_,	sizeof(UINT)},//fw ver
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x1A,		"",		_ADLINK_STRING1_,	4},//adlink serial1
		{OD_BASE_INDEX_MODULE_IDENTIFICATION, 0x1B,		"",		_ADLINK_STRING2_,	4},//adlink serial2
	};
	p_general_slot_info = general_slot_info;
	general_slot_size = sizeof(general_slot_info)/sizeof(OD_INFO);

	static OD_INFO dc_parameter_info[] =
	{
		{0x1C32, 1,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 4,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 5,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 6,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 8,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 9,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 10,	"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 11,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 12,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 32,	"",		_INT8_, sizeof(CHAR)},

		{0x1C33, 1,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 4,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 5,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 6,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 8,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 9,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 10,	"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 11,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 12,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 32,	"",		_INT8_, sizeof(CHAR)},
		
		{0x1C34, 0,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C34, 1,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C34, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C34, 3,		"",		_UINT32_, sizeof(UINT)},

	};
	p_dc_parameter_info = dc_parameter_info;
	dc_parameter_size = sizeof(dc_parameter_info)/sizeof(OD_INFO);

	//BC(EPS-6000)
    static OD_INFO od_info[] =
    {
        {0x1000, 0,      "M",    _UINT32_,		sizeof(UINT)},//Device Type
        {0x1001, 0,      "O",    _UINT32_,		sizeof(UINT)},//Error Register

        {0x1003, 0,      "O",    "",     	sizeof(CHAR)},//Number of errors
        {0x1003, 1,      "O",    "", 		sizeof(UINT)},//Standard error field
        {0x1003, 2,      "O",    "", 		sizeof(UINT)},
        {0x1003, 3,      "O",    "", 		sizeof(UINT)},
        {0x1003, 4,      "O",    "", 		sizeof(UINT)},
        {0x1003, 5,      "O",    "", 		sizeof(UINT)},
        {0x1003, 6,      "O",    "", 		sizeof(UINT)},
        {0x1003, 7,      "O",    "", 		sizeof(UINT)},
        {0x1003, 8,      "O",    "", 		sizeof(UINT)},

        {0x1004, 0,      "O",    "",     	sizeof(CHAR)},//Error range setting
        {0x1004, 1,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 2,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 3,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 4,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 5,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 6,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 7,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 8,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 9,      "O",    "",     	sizeof(USHORT)},
        {0x1004, 10,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 11,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 12,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 13,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 14,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 15,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 16,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 17,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 18,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 19,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 20,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 21,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 22,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 23,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 24,      "O",    "", 		sizeof(USHORT)},
        {0x1004, 25,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 26,      "O",    "", 		sizeof(BOOL)},
        {0x1004, 27,      "O",    "", 		sizeof(UINT)},

        {0x1005, 0,      "O",    "", 		sizeof(CHAR)},	//not implement yet!!
        {0x1005, 1,      "O",    "", 		sizeof(BOOL)},
        {0x1005, 2,      "O",    "", 		sizeof(BOOL)},
        {0x1005, 3,      "O",    "", 		sizeof(USHORT)},
        {0x1005, 4,      "O",    "", 		sizeof(USHORT)},
        {0x1005, 5,      "O",    "", 		sizeof(UINT)},
        {0x1005, 6,      "O",    "", 		sizeof(UINT)},

        {0x1008, 0,      "M",    _STRING_,		8},//Device Name
        {0x1009, 0,      "M",    _STRING_,		8},//Hardware Version
        {0x100A, 0,      "M",    _STRING_,		8},//Software Version

        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x00,      "M",    _UINT8_,	 		sizeof(CHAR)},//Manufactory Version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x01,      "M",    _UINT32_,	 	sizeof(UINT)},//Slave Code Version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x02,      "M",    _STRING_,		32},//SDK version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x03,      "M",    _UINT32_,	 	sizeof(UINT)},//Hardware Version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x04,      "M",    _UINT32_,	 	sizeof(UINT)},//Software Version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x05,      "M",    _ADLINK_STRING1_,	 	4},//Adlink Serial Number
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x06,      "M",    _ADLINK_STRING2_,	 	4},//Adlink Serial Number
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x07,      "M",    _STRING_,	 	16},//Driver Version
        {OD_INDEX_BC6000_MANUFACTORY_VER,	0x08,      "M",    _STRING_,	 	32},//Library Version

		{0x100C, 0,		"O",	_UINT8_,	sizeof(CHAR)},
		{0x100C, 1,		"O",	_UINT16_,	sizeof(USHORT)},
		{0x100C, 2,		"O",	_UINT16_,	sizeof(USHORT)},
		{0x100C, 3,		"O",	_UINT16_,	sizeof(USHORT)},
		{0x100C, 4,		"O",	_UINT16_,	sizeof(USHORT)},
		{0x100C, 5,		"O",	_UINT16_,	sizeof(USHORT)},

		{0x100E, 0,		"O",	_UINT8_,	sizeof(CHAR)},
		{0x100E, 1,		"O",	_UINT32_,	sizeof(UINT)},	//Temperature of the System
		{0x100E, 2,		"O",	_UINT32_,	sizeof(UINT)},	//Temperature of Zynq Junction

		{0x100F, 0,		"O",	_UINT16_,	sizeof(USHORT)},//Error Handling

        {OD_INDEX_BC6000_IDENTITY,	0x00,      "M",    _UINT8_,	 sizeof(CHAR)},//Number of entries
        {OD_INDEX_BC6000_IDENTITY,	0x01,      "M",    _UINT32_, 	 sizeof(UINT)},//Vendor ID
        {OD_INDEX_BC6000_IDENTITY,	0x02,      "M",    _UINT32_,	 sizeof(UINT)},//Product Code
        {OD_INDEX_BC6000_IDENTITY,	0x03,      "M",    _UINT32_,	 sizeof(UINT)},//Revision Number
        {OD_INDEX_BC6000_IDENTITY,	0x04,      "M",    _UINT32_,	 sizeof(UINT)},//Serial Number

		{0x10F1, 0,		"O",	_UINT8_,	sizeof(CHAR)},//Error Settings
        {0x10F1, 1,		"C",	_UINT32_,	sizeof(UINT)},
        {0x10F1, 2,		"C",	_UINT32_,	sizeof(UINT)},

		{0x10F3, 0,		"O",	_UINT8_,	sizeof(CHAR)},
		{0x10F3, 1,		"C",	_UINT8_,	sizeof(CHAR)},//Diagnosis history
		{0x10F3, 2,		"C",	_UINT8_,	sizeof(CHAR)},
		{0x10F3, 3,		"C",	_UINT8_,	sizeof(CHAR)},
		{0x10F3, 4,		"C",	_UINT8_,	sizeof(CHAR)},
		{0x10F3, 5,		"C",	_UINT16_,	sizeof(USHORT)},

		{0x1100, 0,		"O",	_UINT8_,	sizeof(CHAR)},
		{0x1100, 1,		"O",	_UINT32_,	sizeof(UINT)},
		{0x1100, 2,		"O",	_UINT32_,	sizeof(UINT)},

		{0x1313, 0,		"C",	_UINT8_,	sizeof(CHAR)},
		{0x1313, 1,		"C",	_UINT32_,	sizeof(UINT)},
		{0x1313, 2,		"C",	_UINT32_,	sizeof(UINT)},
		{0x1313, 3,		"C",	_UINT32_,	sizeof(UINT)},

		{0x1C00, 0,		"",		_UINT8_,	sizeof(CHAR)},
		{0x1C00, 1,		"",		_UINT8_,	sizeof(CHAR)},
		{0x1C00, 2, 	"", 	_UINT8_,	sizeof(CHAR)},
		{0x1C00, 3, 	"", 	_UINT8_,	sizeof(CHAR)},
		{0x1C00, 4, 	"", 	_UINT8_,	sizeof(CHAR)},

        {0x1C12, 0,      "C",	_UINT32_,	 sizeof(UINT)},//SyncManager 2
        {0x1C13, 0,      "C",   _UINT32_,  	 sizeof(UINT)},//SyncManager 3

		{0x1C32, 0,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C32, 1,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 4,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 5,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 6,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 8,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 9,		"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 10,	"",		_UINT32_, sizeof(UINT)},
		{0x1C32, 11,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 12,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C32, 32,	"",		_BOOL_, sizeof(BOOL)},

		{0x1C33, 0,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C33, 1,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 4,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 5,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 6,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 8,		"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 9,		"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 10,	"",		_UINT32_, sizeof(UINT)},
		{0x1C33, 11,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 12,	"",		_UINT16_, sizeof(USHORT)},
		{0x1C33, 32,	"",		_BOOL_, sizeof(BOOL)},
		
		{0x1C34, 0,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C34, 1,		"",		_UINT8_, sizeof(CHAR)},
		{0x1C34, 2,		"",		_UINT32_, sizeof(UINT)},
		{0x1C34, 3,		"",		_UINT32_, sizeof(UINT)},


        //{OD_BASE_INDEX_MODULE_IDENTIFICATION, 	0,      "",    _UINT32_,	 sizeof(UINT)},//Information Data

		{0x3FFF, 0,		"",		_UINT32_, sizeof(UINT)},
		{0x4FFF, 0,		"",		_UINT32_, sizeof(UINT)},

        {0xF000, 0,      "",    _UINT8_,	sizeof(CHAR)},//Number of entries
		{0xF000, 1, 	 "",	_UINT16_,	sizeof(USHORT)},//Index distance
		{0xF000, 2, 	 "",	_UINT16_,	sizeof(USHORT)},//Max number of modules

		{0xF010, 0,      "",    _UINT8_,	sizeof(CHAR)},
        {0xF010, 1,      "",    _UINT32_,	sizeof(UINT)},
		{0xF010, 2, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF010, 3, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF010, 4, 	 "",	_UINT32_,	sizeof(UINT)},

		{0xF030, 0,      "",    _UINT8_,	sizeof(CHAR)},
		{0xF030, 1, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF030, 2, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF030, 3, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF030, 4, 	 "",	_UINT32_,	sizeof(UINT)},

		{0xF050, 0,      "",    _UINT8_,	sizeof(CHAR)},
		{0xF050, 1, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF050, 2, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF050, 3, 	 "",	_UINT32_,	sizeof(UINT)},
		{0xF050, 4,      "",    _UINT32_,	sizeof(UINT)},

    };

    p_od_info = od_info;
    od_size = sizeof(od_info)/sizeof(OD_INFO);

	//AO(EPS-4008)
	static OD_INFO ao_info[] =
	{
        {0x1600, 0,		"",		_UINT8_,	sizeof(CHAR)},
        {0x1600, 1,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 2,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 3,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 4,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 5,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 6,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 7,		"",		_UINT32_,	sizeof(UINT)},
        {0x1600, 8,		"",		_UINT32_,	sizeof(UINT)},

		{_INPUT_DATA_ADDRESS_, 0,		"",		_UINT8_,	sizeof(CHAR)},
		{_INPUT_DATA_ADDRESS_, 1,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 2,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 3,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 4,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 5,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 6,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 7,		"",		_UINT16_,	sizeof(USHORT)},
		{_INPUT_DATA_ADDRESS_, 8,		"",		_UINT16_,	sizeof(USHORT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,	 "O",	 _UINT32_,	 sizeof(UINT)},//Device type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,	 "O",	 _UINT32_,	 sizeof(UINT)},//Vendor ID
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,	 "O",	 _UINT32_,	 sizeof(UINT)},//Product Code
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,	 "C",	 _UINT32_,	 sizeof(UINT)},//Revision Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,	 "C",	 _UINT32_,	 sizeof(UINT)},//Serial Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,	 "",	_UINT32_,	 sizeof(UINT)},//Module PDP group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,	 "",	_UINT32_,	 sizeof(UINT)},//Module Ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,	 "",	_UINT32_,	 sizeof(UINT)},//Slot
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,	 "",	_UINT32_,	 sizeof(UINT)},//Slot Group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,	 "",	_UINT32_,	 sizeof(UINT)},//HW Version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,	 "",	_UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,	 "O",	 _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x05,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x06,	"",		_UINT32_,	4},

		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x04,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x05,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x06,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x07,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x08,	"",		_UINT32_,	4},
	};

	p_ao_info = ao_info;
	ao_size = sizeof(ao_info)/sizeof(OD_INFO);

	//AI16(EPS-3216)
	static OD_INFO ai16_info[] =
	{
        {0x1A00, 0x00,		"",		_UINT8_,	sizeof(CHAR)},
        {0x1A00, 0x01,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x02,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x03,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x04,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x05,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x06,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x07,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x08,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x09,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0A,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0B,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0C,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0D,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0E,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0F,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x10,		"",		_UINT32_,	sizeof(UINT)},

		{_OUTPUT_DATA_ADDRESS_, 0x00,		"",		_UINT8_,	sizeof(CHAR)},
		{_OUTPUT_DATA_ADDRESS_, 0x01,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x02,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x03,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x04,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x05,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x06,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x07,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x08,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x09,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0A,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0B,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0C,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0D,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0E,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0F,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x10,		"",		_UINT16_,	sizeof(USHORT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,	 "O",	 _UINT32_,	 sizeof(UINT)},//Device type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,	 "O",	 _UINT32_,	 sizeof(UINT)},//Vendor ID
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,	 "O",	 _UINT32_,	 sizeof(UINT)},//Product Code
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,	 "C",	 _UINT32_,	 sizeof(UINT)},//Revision Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,	 "C",	 _UINT32_,	 sizeof(UINT)},//Serial Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,	 "",	_UINT32_,	 sizeof(UINT)},//Module PDP group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,	 "",	_UINT32_,	 sizeof(UINT)},//Module Ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,	 "",	_UINT32_,	 sizeof(UINT)},//Slot
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,	 "",	_UINT32_,	 sizeof(UINT)},//Slot Group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,	 "",	_UINT32_,	 sizeof(UINT)},//HW Version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,	 "",	_UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,	 "O",	 _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT16_,	2},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	4},

		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x00,	"", 	_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x04,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x05,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x06,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x07,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x08,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x09,	"",		_UINT32_,	4},

	};
	p_ai16_info = ai16_info;
	ai16_size = sizeof(ai16_info)/sizeof(OD_INFO);

	//AI32(EPS-3032)
	static OD_INFO ai32_info[] =
	{
        {0x1A00, 0x00,		"",		_UINT8_,	sizeof(CHAR)},
        {0x1A00, 0x01,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x02,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x03,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x04,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x05,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x06,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x07,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x08,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x09,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0A,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0B,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0C,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0D,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0E,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x0F,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x10,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x11,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x12,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x13,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x14,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x15,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x16,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x17,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x18,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x19,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1A,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1B,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1C,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1D,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1E,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x1F,		"",		_UINT32_,	sizeof(UINT)},
        {0x1A00, 0x20,		"",		_UINT32_,	sizeof(UINT)},


		{_OUTPUT_DATA_ADDRESS_, 0x00,		"",		_UINT8_,	sizeof(CHAR)},
		{_OUTPUT_DATA_ADDRESS_, 0x01,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x02,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x03,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x04,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x05,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x06,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x07,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x08,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x09,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0A,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0B,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0C,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0D,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0E,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x0F,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x10,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x11,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x12,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x13,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x14,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x15,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x16,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x17,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x18,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x19,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1A,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1B,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1C,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1D,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1E,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x1F,		"",		_UINT16_,	sizeof(USHORT)},
		{_OUTPUT_DATA_ADDRESS_, 0x20,		"",		_UINT16_,	sizeof(USHORT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,	 "O",	 _UINT32_,	 sizeof(UINT)},//Device type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,	 "O",	 _UINT32_,	 sizeof(UINT)},//Vendor ID
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,	 "O",	 _UINT32_,	 sizeof(UINT)},//Product Code
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,	 "C",	 _UINT32_,	 sizeof(UINT)},//Revision Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,	 "C",	 _UINT32_,	 sizeof(UINT)},//Serial Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,	 "",	_UINT32_,	 sizeof(UINT)},//Module PDP group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,	 "",	_UINT32_,	 sizeof(UINT)},//Module Ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,	 "",	_UINT32_,	 sizeof(UINT)},//Slot
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,	 "",	_UINT32_,	 sizeof(UINT)},//Slot Group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,	 "",	_UINT32_,	 sizeof(UINT)},//HW Version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,	 "",	_UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,	 "O",	 _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	1},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT16_,	2},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	4},

		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x00,	"",		_UINT8_,	1},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x04,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x05,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x06,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x07,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x08,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_CALIBRATION,		0x09,	"",		_UINT32_,	4},

	};
	p_ai32_info = ai32_info;
	ai32_size = sizeof(ai32_info)/sizeof(OD_INFO);

	//DI32(EPS-1132)
	static OD_INFO di32_info[] =
	{

        {0x1A00, 0x00,		"",		_UINT8_,	sizeof(CHAR)},
        {0x1A00, 0x01,		"",		_UINT32_,	sizeof(UINT)},


		{_OUTPUT_DATA_ADDRESS_, 0,		"",		_UINT32_,	sizeof(UINT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,    "O",    _UINT32_,	 sizeof(UINT)},//Device type
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,    "O",    _UINT32_,	 sizeof(UINT)},//Vendor ID
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,    "O",    _UINT32_,	 sizeof(UINT)},//Product Code
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,    "C",    _UINT32_,	 sizeof(UINT)},//Revision Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,    "C",    _UINT32_,	 sizeof(UINT)},//Serial Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,    "",    _UINT32_,	 sizeof(UINT)},//Module PDP group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,    "",    _UINT32_,	 sizeof(UINT)},//Module Ident
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,    "",    _UINT32_,	 sizeof(UINT)},//Slot
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,    "",    _UINT32_,	 sizeof(UINT)},//Slot Group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,    "",    _UINT32_,	 sizeof(UINT)},//HW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,    "",    _UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,    "O",    _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	1},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x05,	"",		_UINT32_,	4},

	};
	p_di32_info = di32_info;
	di32_size = sizeof(di32_info)/sizeof(OD_INFO);

	//DO32(EPS-2032)
	static OD_INFO do32_info[] =
	{
		{0x1600, 0,		"",		_UINT8_,	sizeof(CHAR)},
		{0x1600, 1,		"",		_UINT32_,	sizeof(UINT)},


		{_INPUT_DATA_ADDRESS_, 0,		"",		_UINT32_,	sizeof(UINT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,	 "O",	 _UINT32_,	 sizeof(UINT)},//Device type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,	 "O",	 _UINT32_,	 sizeof(UINT)},//Vendor ID
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,	 "O",	 _UINT32_,	 sizeof(UINT)},//Product Code
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,	 "C",	 _UINT32_,	 sizeof(UINT)},//Revision Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,	 "C",	 _UINT32_,	 sizeof(UINT)},//Serial Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,	 "",	_UINT32_,	 sizeof(UINT)},//Module PDP group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,	 "",	_UINT32_,	 sizeof(UINT)},//Module Ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,	 "",	_UINT32_,	 sizeof(UINT)},//Slot
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,	 "",	_UINT32_,	 sizeof(UINT)},//Slot Group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,	 "",	_UINT32_,	 sizeof(UINT)},//HW Version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,	 "",	_UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,	 "O",	 _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	4},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	4},

	};
	p_do32_info = do32_info;
	do32_size = sizeof(do32_info)/sizeof(OD_INFO);

	//RO(EPS-2308)
	static OD_INFO ro_info[] =
	{

		{0x1600, 0,		"",		_UINT8_,	sizeof(CHAR)},
		{0x1600, 1,		"",		_UINT32_,	sizeof(UINT)},

        //{0x1C12, 0,		"",		_UINT8_,	sizeof(CHAR)},
        //{0x1C13, 0,		"",		_UINT8_,	sizeof(CHAR)},

		{_INPUT_DATA_ADDRESS_, 0,		"",		_UINT32_,	sizeof(UINT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,    "O",    _UINT32_,	 sizeof(UINT)},//Device type
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,    "O",    _UINT32_,	 sizeof(UINT)},//Vendor ID
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,    "O",    _UINT32_,	 sizeof(UINT)},//Product Code
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,    "C",    _UINT32_,	 sizeof(UINT)},//Revision Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,    "C",    _UINT32_,	 sizeof(UINT)},//Serial Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,    "",    _UINT32_,	 sizeof(UINT)},//Module PDP group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,    "",    _UINT32_,	 sizeof(UINT)},//Module Ident
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,    "",    _UINT32_,	 sizeof(UINT)},//Slot
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,    "",    _UINT32_,	 sizeof(UINT)},//Slot Group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,    "",    _UINT32_,	 sizeof(UINT)},//HW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,    "",    _UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,    "O",    _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 sizeof(UINT)},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 sizeof(UINT)},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x05,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x06,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x07,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x08,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x09,	"",		_UINT32_,	sizeof(UINT)},

	};
	p_ro_info = ro_info;
	ro_size = sizeof(ro_info)/sizeof(OD_INFO);

	//RTD(EPS-3504)
	static OD_INFO rtd_info[] =
	{

		{0x1A00, 0,		"",		_UINT8_,	sizeof(CHAR)},
		{0x1A00, 1,		"",		_UINT32_,	sizeof(UINT)},


		{_OUTPUT_DATA_ADDRESS_, 0,		"",		_UINT8_,	sizeof(UINT)},
		{_OUTPUT_DATA_ADDRESS_, 1,		"",		_UINT32_,	sizeof(UINT)},
		{_OUTPUT_DATA_ADDRESS_, 2,		"",		_UINT32_,	sizeof(UINT)},
		{_OUTPUT_DATA_ADDRESS_, 3,		"",		_UINT32_,	sizeof(UINT)},
		{_OUTPUT_DATA_ADDRESS_, 4,		"",		_UINT32_,	sizeof(UINT)},

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,    "O",    _UINT32_,	 sizeof(UINT)},//Device type
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,    "O",    _UINT32_,	 sizeof(UINT)},//Vendor ID
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,    "O",    _UINT32_,	 sizeof(UINT)},//Product Code
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,    "C",    _UINT32_,	 sizeof(UINT)},//Revision Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,    "C",    _UINT32_,	 sizeof(UINT)},//Serial Number
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,    "",    _UINT32_,	 sizeof(UINT)},//Module PDP group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,    "",    _UINT32_,	 sizeof(UINT)},//Module Ident
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,    "",    _UINT32_,	 sizeof(UINT)},//Slot
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,    "",    _UINT32_,	 sizeof(UINT)},//Slot Group
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,    "",    _UINT32_,	 sizeof(UINT)},//HW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,    "",    _UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,    "O",    _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 sizeof(UINT)},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 sizeof(UINT)},//Adlink Serial Number [39:32]

		{OD_BASE_INDEX_MODULE_SETTINGS,			0x00,	"",		_UINT8_,	sizeof(CHAR)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x01,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x02,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x03,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x04,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x05,	"",		_UINT32_,	sizeof(UINT)},
		{OD_BASE_INDEX_MODULE_SETTINGS,			0x06,	"",		_UINT32_,	sizeof(UINT)},

	};
	p_rtd_info = rtd_info;
	rtd_size = sizeof(rtd_info)/sizeof(OD_INFO);

	//MO(EPS-7002)
	static OD_INFO mo_info[] =
	{
		{0x1600, 0, 	"", 	_UINT8_,	sizeof(CHAR)},
		{0x1600, 1, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1600, 2, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1600, 3, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1600, 4, 	"", 	_UINT32_,	sizeof(UINT)},

		{0x1A00, 0, 	"", 	_UINT8_,	sizeof(CHAR)},
		{0x1A00, 1, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1A00, 2, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1A00, 3, 	"", 	_UINT32_,	sizeof(UINT)},
		{0x1A00, 4, 	"", 	_UINT32_,	sizeof(UINT)},


		{0x2010, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2020, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2024, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x210C, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2110, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2114, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2118, 0,		"", 	_UINT32_,	sizeof(UINT)},
		//{0x2310, 0,		"", 	_INT32_,	sizeof(int)},
		//{0x2314, 0,		"", 	_INT32_,	sizeof(int)},
		{0x230C, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2310, 0,		"", 	_INT32_,	sizeof(int)},
		{0x2314, 0,		"", 	_INT32_,	sizeof(int)},
		{0x2318, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x231C, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2320, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2324, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2328, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x232C, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2330, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2334, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x2378, 0,		"", 	_UINT32_,	sizeof(UINT)},
		{0x237C, 0,		"", 	_UINT32_,	sizeof(UINT)},
		
		
		{0x603F,	0,	 "O",	 _UINT16_,	 16},//Error code
		{0x6040,	0,	 "M",	 _UINT32_,	 32},//Control word
		{0x6041,	0,	 "M",	 _UINT32_,	 32},//Status word
		{0x6060,	0,	 "M",	 _INT8_,	 8},//Modes of operation
		{0x6061,	0,	 "M",	 _INT8_,	 8},//Modes of operation display
		{0x6064,	0,	 "M",	 _UINT32_,	 32},//Position actual value
		{0x607A,	0,	 "M",	 _UINT32_,	 32},//Target position
		{0x60B8,	0,	 "O",	 _UINT16_,	 16},
		{0x60B9,	0,	 "O",	 _UINT32_,	 32},
		{0x60BA,	0,	 "O",	 _UINT32_,	 32},
		{0x60BB,	0,	 "O",	 _UINT32_,	 32},
		{0x60BC,	0,	 "O",	 _UINT32_,	 32},
		{0x60BD,	0,	 "O",	 _UINT32_,	 32},

		{0x60C2,	0,	 "O",	 _UINT8_,	 8},
		{0x60C2,	1,	 "O",	 _UINT8_,	 8},
		{0x60C2,	2,	 "O",	 _UINT8_,	 8},

		{0x60D0,	0,	 "O",	 _UINT16_,	 16},
		{0x60FD,	0,	 "O",	 _UINT16_,	 16},
		{0x60FE,	0,	 "O",	 _UINT16_,	 16},
		{0x6502,	0,	 "M",	 _UINT32_,	 32},

		{0x683F,	0,	 "O",	_UINT16_,	 16},//Error code
		{0x6840,	0,	 "M",	_UINT16_,	 16},//Control word
		{0x6841,	0,	 "M",	_UINT16_,	 16},//Status word
		{0x6860,	0,	 "M",	_INT8_,		 8},//Modes of operation
		{0x6861,	0,	 "M",	_INT8_,	 	 8},//Modes of operation display
		{0x6864,	0,	 "M",	_UINT32_,	 32},//Position actual value
		{0x687A,	0,	 "M",	_UINT32_,	 32},//Target position
		
		{0x68B8,	0,	 "O",	_UINT16_,	 16},
		{0x68B9,	0,	 "O",	_UINT16_,	 16},
		{0x68BA,	0,	 "O",	_UINT32_,	 32},
		{0x68BB,	0,	 "O",	_UINT32_,	 32},
		{0x68BC,	0,	 "O",	_UINT32_,	 32},
		{0x68BD,	0,	 "O",	_UINT32_,	 32},

		{0x68C2,	0,	 "O",	_UINT32_,	 32},
		{0x68C2,	1,	 "O",	_UINT8_,	 8},
		{0x68C2,	2,	 "O",	_UINT8_,	 8},

		{0x68D0,	0,	 "O",	_UINT16_,	 16},
		{0x68FD,	0,	 "O",	_UINT32_,	 32},
		{0x68FE,	0,	 "O",	_UINT32_,	 32},
		{0x6D02,	0,	 "M",	_UINT32_,	 32},//Supported drive modes

		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x00,    "O",    _UINT8_,	 sizeof(CHAR)},
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x02,    "O",    _STRING_,	 32},//Type string
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x03,    "O",    _STRING_,	 32},//Name string
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x04,	 "O",	 _UINT32_,	 sizeof(UINT)},//Device type
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x05,	 "O",	 _UINT32_,	 sizeof(UINT)},//Vendor ID
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x06,	 "O",	 _UINT32_,	 sizeof(UINT)},//Product Code
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x07,	 "C",	 _UINT32_,	 sizeof(UINT)},//Revision Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x08,	 "C",	 _UINT32_,	 sizeof(UINT)},//Serial Number
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x09,	 "",	_UINT32_,	 sizeof(UINT)},//Module PDP group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0A,	 "",	_UINT32_,	 sizeof(UINT)},//Module Ident
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0B,	 "",	_UINT32_,	 sizeof(UINT)},//Slot
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x0C,	 "",	_UINT32_,	 sizeof(UINT)},//Slot Group
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x14,	 "",	_UINT32_,	 sizeof(UINT)},//HW Version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x15,	 "",	_UINT32_,	 sizeof(UINT)},//FPGA,CPLD version
		{OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x16,	 "O",	 _UINT32_,	 sizeof(UINT)},//SW Version
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1A,    "",    _ADLINK_STRING1_,	 4},//Adlink Serial Number [31:0]
        {OD_BASE_INDEX_MODULE_IDENTIFICATION,	0x1B,    "",    _ADLINK_STRING2_,	 4},//Adlink Serial Number [39:32]

	};
	p_mo_info = mo_info;
	mo_size = sizeof(mo_info)/sizeof(OD_INFO);

	static ERR_LOG_INFO err_info[] =
	{
		{0, "Module ID mismatach"},
		{1, "EtherCAT link is broken"},
		{2, "Connection error for internal modules"},
		{3, "Internal voltage too high (BC connector voltage error)"},
		{4, "Internal voltage too low  (BC connector voltage error)"},
		{5, "Internal voltage too high (Zynq junction voltage error)"},
		{6, "Internal voltage too low  (Zynq junction voltage error)"},
		{7, "Internal current, BC connector current error"},
		{8, "Continues over current, BC connector current error"},
		{9, "Each connector's current error"},
		{10, "Internal handshake error for modules"},
		{11, "DO no communication error"},
		{12, "RO no communication error"},
		{13, "AO no communication error"},
		{14, "Excess temperature device, each connector's temperature"},
		{15, "Too low temperature device, each connector's temperature"},
		{16, "Excess temperature device, Zynq junction temperature warning"},
		{17, "Too low temperature device"},
		{18, "RO counter expired warning"},
		{19, "DI over temperature error (>150)"},
		{20, "Mains over-voltage phase, AO voltage difference larger than expected value(loop-back function)"},
		{21, "Mains under-voltage phase, AO voltage difference larger than expected value(loop-back function)"},
		{22, "Syncmaster Sync0 output error"},
		{23, "Syncmaster Sync0 input error"},
		{24, "Application controller failure"},
		{25, "Proccess data watchdog timeout"},
		{26, "Booting error"},

		{0+32, "MO emergency occur"},
		{1+32, "MO alarm occur"},
		{2+32, "MO no communication error"},
		{16+32, "Parameter error"},
	};
	p_err_info = err_info;
	err_info_size = sizeof(err_info)/sizeof(ERR_LOG_INFO);

	nData = new AnsiString[general_bc_size+1];
	nSlotData = new AnsiString[general_slot_size+1];
	nDcData = new AnsiString[dc_parameter_size+1];

	//this->BorderStyle = bsDialog;
	//this->BorderStyle = bsNone;
	//this->BorderStyle = bsSingle;
	//this->BorderStyle = bsSizeable;
	//this->BorderStyle = bsSizeToolWindow;
	//this->BorderStyle = bsToolWindow;
	//this->FormStyle = fsNormal;

    for(int i = 0; i < TOTAL_SLOTS; i++)
    {
        ((TImage *)FindComponent("imgBpSlot" + IntToStr(i)))->Cursor = crHandPoint;
        ((TImage *)FindComponent("imgBpSlot" + IntToStr(i)))->ShowHint = true;
        ((TImage *)FindComponent("imgBpSlot" + IntToStr(i)))->Hint = "Click to open the I/O panel";
    }

    btnFoe->ShowHint = true;
    btnFoe->Hint = "Download *bin file to flash";
    btnGetOd->ShowHint = true;
    btnGetOd->Hint = "Get OD information from current box";

    for(int i = 0; i < 9; i++)
    {
        ((TEdit *)FindComponent("edBcInfo" + AnsiString(i)))->ReadOnly = true;
    }

    for(int i = 0; i < TOTAL_SLOTS; i++)
    {
        ((TEdit *)FindComponent("edModType" + AnsiString(i)))->ReadOnly = true;
        ((TEdit *)FindComponent("edMod" + AnsiString(i)))->ReadOnly = true;
        ((TEdit *)FindComponent("edHwVer" + AnsiString(i)))->ReadOnly = true;
        ((TEdit *)FindComponent("edSwVer" + AnsiString(i)))->ReadOnly = true;
        ((TEdit *)FindComponent("edAdlinkSerial" + AnsiString(i)))->ReadOnly = true;
        ((TEdit *)FindComponent("edErr" + AnsiString(i)))->ReadOnly = true;
    }
	//displayOdFunc(false);

    TBorderIcons tempBI;
    //slaveBI >> biSystemMenu;
    //slaveBI >> biMinimize;
    //slaveBI >> biMaximize;
    //slaveBI >> biHelp;

    //tempBI >> biSystemMenu;
    //tempBI >> biMinimize;
    //tempBI >> biMaximize;
    //tempBI >> biHelp;
    this->BorderIcons = tempBI;

	this->foePrgForm = new TForm(this);
	this->foePrgForm->Parent = this;
	this->foePrgForm->Width = 320;
	this->foePrgForm->Height = 100;
	this->foePrgForm->Top = 100;
	this->foePrgForm->Left = 200;
	//this->foePrgForm->Position = poOwnerFormCenter;
	//foeTotal = 150000;

/*
	this->lbPercent = new TLabel(this->ExportForm);
	this->lbPercent->Parent = this->ExportForm;
	this->lbPercent->Height = 15;
	this->lbPercent->Width = 100;
	this->lbPercent->Top = 0;
	this->lbPercent->Left = 10;
*/	
	this->foePrgBar = new TProgressBar(this->foePrgForm);
	this->foePrgBar->Parent = this->foePrgForm;
	this->foePrgForm->BorderIcons = tempBI;
	//this->foePrgForm->FormStyle = fsStayOnTop;
	this->foePrgBar->Width = 280;
	this->foePrgBar->Top = 30;
	this->foePrgBar->Left = 10;

	richEdErr->ReadOnly = true;

	systemPath = ExtractFilePath(Application->ExeName); 

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::FormCreate(TObject *Sender)
{
	
	if(Sender != NULL)
	{
		imgBp->Picture->LoadFromFile(systemPath+"\\slot\\EPS-6000-Case.jpg");
		imgBc->Picture->LoadFromFile(systemPath+"\\slot\\EPS-6000.jpg");
		for(int i = 0; i < TOTAL_SLOTS; i++)
		{
			clearSlotInfo(i, "slot_empty.jpg");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::enableComponent(bool enabled)
{
	btnFoe->Enabled = enabled;
	btnGetOd->Enabled = enabled;
	btnRefresh->Enabled = enabled;
	btnClear->Enabled = enabled;
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::btnFoeClick(TObject *Sender)
{
	OpenDialogFoe->InitialDir = ExtractFileDir(Application->ExeName) ;
	OpenDialogFoe->Filter = "bin files (*.bin)|*.BIN";
	if(OpenDialogFoe->Execute())
	{
		isFoeClick = true;
		enableComponent(false);
	}
	
	//isFoeClick = true;
	//enableComponent(false);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::foeDownload()
{
    AnsiString strBinFilePath, strBinFileName;
    TCHAR lpszFwFileName[MAX_PATH];
    ULONG dwFileHandle;
    TFileStream *fs;
    //ifstream file;
    int nErr;
	BYTE binData[4096];
    ULONG nCurr, nData, nTotal=0;
    //File *binFile;
    ULONG prg = 0;
	int step = 1000;

	strBinFilePath = OpenDialogFoe->FileName;
	if(!strBinFilePath.IsEmpty())
	{
		
		strBinFileName = api->GetFileName(strBinFilePath);
        if(strBinFileName.Length() > MAX_PATH)
        {
            api->displayLogMsg(nErr, "Invalid file length");
        }
        //_tcscpy(lpszFwFileName , strBinFileName.c_str());
        memcpy(lpszFwFileName , strBinFileName.c_str(), sizeof(lpszFwFileName));
		
		nErr = FoE_Download_FileOpen(wSlaveNo, lpszFwFileName, &dwFileHandle );
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "FoE_Download_FileOpen() fail");
			displayFoePrg(foeTotal);
			isFoeClick = false;
			enableComponent(true);
			return;
		}
		
		fs = new TFileStream(strBinFilePath, fmOpenRead);
		nData = (ULONG)fs->Size;
		foeTotal = nData+40000;	//add delay 35s
		this->foePrgForm->Show();

		displayFoePrg(2500);
	
		while(nData > 0)
		{
			nCurr = fs->Read(binData, sizeof(binData));
			nErr = FoE_Download_DataWrite(wSlaveNo, dwFileHandle, binData, nCurr );
			if(nErr != ECMaster_ERROR_SUCCESS)
			{
				api->displayLogMsg(nErr, "FoE_Download_DataWrite() fail");
				break;
			}
			nData -= nCurr;
			nTotal += nCurr;
			displayFoePrg(2500+nTotal);
			
		}
		
		nErr = FoE_Download_FileClose( wSlaveNo, dwFileHandle);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "FoE_Download_FileClose() fail");
		}
		else
		{
			displayFoePrg(5000+nTotal);
			if( nData <= 0 )
			{
				//do delay
				for(prg = 5000+nTotal; prg <= foeTotal; prg+=step)
				{
					displayFoePrg(prg);
					//Sleep(step);
					api->delay(step);
				}
				
				api->displayLogMsg(nErr, "FoE Download : " + strBinFileName + " to EPS_#"+ AnsiString(adlinkNo)+" done");
			}
		}
		fs->Free();
	}


	isFoeClick = false;
	//btnFoe->Enabled = true;
	enableComponent(true);

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::btnGetOdClick(TObject *Sender)
{
	isGetOdClick = true;
	//btnGetOd->Enabled = false;
    //btnFoe->Enabled = false;
    enableComponent(false);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::getOdProcess()
{
    //int nErr;
	int step;
    UINT idx, subIdx, dataSize;
    char dataType[32];

    vleCardInfo->Strings->Clear();

	//read BC OD
    for(int i = 0; i < od_size; i++)
    {
    	displayOdListTbl(wSlaveNo, p_od_info[i].dataSize, p_od_info[i].index, p_od_info[i].subIndex, p_od_info[i].datatype);
    }

	//read each slot OD
	for(int slot = 0; slot < TOTAL_SLOTS; slot++)
	{
		//nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0xF030, slot+1, &m_dwModuleSlot[slot], &dwDataLen);
		//if(nErr == ECMaster_ERROR_SUCCESS)
		{
			switch(m_dwModuleSlot[slot])
			{
			case EPS4008_ID:	// AO eps-4008
				for(int i = 0; i < ao_size; i++)
				{
					if(p_ao_info[i].index == 0x1600)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_ao_info[i].datatype);
					memcpy(dataType, p_ao_info[i].datatype, sizeof(dataType));
					dataSize = p_ao_info[i].dataSize;
					idx = p_ao_info[i].index+slot*step;
					subIdx = p_ao_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS1132_ID:	// DI eps-1132
				for(int i = 0; i < di32_size; i++)
				{
					if( p_di32_info[i].index == 0x1A00)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_di32_info[i].datatype);
					memcpy(dataType, p_di32_info[i].datatype, sizeof(dataType));
					dataSize = p_di32_info[i].dataSize;
					idx = p_di32_info[i].index+slot*step;
					subIdx = p_di32_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS3032_ID:	// AI eps-3032
				for(int i = 0; i < ai32_size; i++)
				{
					if(p_ai32_info[i].index == 0x1A00)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_ai32_info[i].datatype);
					memcpy(dataType, p_ai32_info[i].datatype, sizeof(dataType));
					dataSize = p_ai32_info[i].dataSize;
					idx = p_ai32_info[i].index+slot*step;
					subIdx = p_ai32_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS3216_ID:	// AI eps-3216
				for(int i = 0; i < ai16_size; i++)
				{
					if(p_ai16_info[i].index == 0x1800 || p_ai16_info[i].index == 0x1A00)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_ai16_info[i].datatype);
					memcpy(dataType, p_ai16_info[i].datatype, sizeof(dataType));
					dataSize = p_ai16_info[i].dataSize;
					idx = p_ai16_info[i].index+slot*step;
					subIdx = p_ai16_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS2032_ID:	// DO eps-2032
			case EPS2132_ID:
				for(int i = 0; i < do32_size; i++)
				{
					if(p_do32_info[i].index == 0x1600)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_do32_info[i].datatype);
					memcpy(dataType, p_do32_info[i].datatype, sizeof(dataType));
					dataSize = p_do32_info[i].dataSize;
					idx = p_do32_info[i].index+slot*step;
					subIdx = p_do32_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS3504_ID:	//	RTD eps-3504
				for(int i = 0; i < rtd_size; i++)
				{
					if(p_rtd_info[i].index == 0x1A00)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_rtd_info[i].datatype);
					memcpy(dataType, p_rtd_info[i].datatype, sizeof(dataType));
					dataSize = p_rtd_info[i].dataSize;
					idx = p_rtd_info[i].index+slot*step;
					subIdx = p_rtd_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS2308_ID:	// RO eps-2308
				for(int i = 0; i < ro_size; i++)
				{
					if(p_ro_info[i].index == 0x1600)
					{
						step = 1;
					}
					else
					{
						step = CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_ro_info[i].datatype);
					memcpy(dataType, p_ro_info[i].datatype, sizeof(dataType));
					dataSize = p_ro_info[i].dataSize;
					idx = p_ro_info[i].index+slot*step;
					subIdx = p_ro_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);
				}
				break;
			case EPS7002_ID:
				for(int i = 0; i < mo_size; i++)
				{
					if(p_mo_info[i].index == 0x1600 || p_mo_info[i].index == 0x1A00)
					{
						idx = p_mo_info[i].index+slot;
					}
					else if(p_mo_info[i].index >> 12 == 2)
					{
						idx = p_mo_info[i].index+slotToMo[slot]*CONFIGURATION_OBJECTS_AREA;
					}
					else if(p_mo_info[i].index >> 12 == 6)
					{
						idx = p_mo_info[i].index+slotToMo[slot]*CONFIGURATION_MO_AREA;
					}
					else
					{
						idx = p_mo_info[i].index+slot*CONFIGURATION_OBJECTS_AREA;
					}
					//strcpy(dataType, p_mo_info[i].datatype);
					memcpy(dataType, p_mo_info[i].datatype, sizeof(dataType));
					dataSize = p_mo_info[i].dataSize;
					//idx = p_mo_info[i].index+slotToMo[slot]*step;
					subIdx = p_mo_info[i].subIndex;
					displayOdListTbl(wSlaveNo, dataSize, idx, subIdx, dataType);

				}
			default:
				break;
			}
	
		}
	
	}
        isGetOdClick = false;
        //btnGetOd->Enabled = true;
        //btnFoe->Enabled = true;
        enableComponent(true);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::displayOdListTbl(USHORT wSlaveNo, UINT dataSize, UINT idx, UINT subIdx, char *dataType)
{
    int nErr = ECMaster_ERROR_SUCCESS;
    //ULONG dwDataLen;
    //BYTE pData[64];
	//BYTE *pData;
	AnsiString strVal, err;
	
	nErr = odToString(wSlaveNo, dataSize, idx, subIdx, dataType, &strVal, &err);
	if( nErr == ECMaster_ERROR_SUCCESS )
	{
		vleCardInfo->InsertRow("0x"+IntToHex((int)idx, 4)+":"+IntToHex((int)subIdx, 2), strVal, true);
		//free(pData); 
	}
	else
	{
		vleCardInfo->InsertRow("0x"+IntToHex((int)idx, 4)+":"+IntToHex((int)subIdx, 2), "", true);
		api->displayLogMsg(nErr, "Get OD fail (0x"+IntToHex((int)idx, 4)+":"+IntToHex((int)subIdx, 2) +") " + err);
	}

}
//---------------------------------------------------------------------------
short __fastcall TformSlave::odToString(USHORT wSlaveNo, UINT dataSize, UINT idx, UINT subIdx, char *dataType, AnsiString *str, AnsiString *err)
{
    int nErr = ECMaster_ERROR_SUCCESS;
    ULONG dwDataLen;
    BYTE pData[64];

	*err = "";
	
	nErr = SendSyncCoEUploadReq(wSlaveNo, dataSize, idx, subIdx, pData, &dwDataLen);
	//LastEcError_Get(&ecErr);
	
	if( dwDataLen > sizeof(pData))
	{
		*err = "OD buffer too small";
		return nErr;
	}
	if( nErr == ECMaster_ERROR_SUCCESS )
	{
		AnsiString strVal = "";
		if(strcmp(dataType, _STRING_) == 0)	//transfer to ASCII
		{
			for(UINT j = 0; j < dwDataLen; j++)
			{
				strVal += (char)pData[j];
			}
		}
		else if(strcmp(dataType, _ADLINK_STRING1_) == 0) //16bit ASCII, 16bit hex
		{
			for(UINT j = dwDataLen-1; j >= dwDataLen/2; j--)
			{
				strVal += (char)pData[j];
			}
            for(int j = dwDataLen/2-1; j >= 0; j--)
			{
				strVal += IntToHex(pData[j], 2);
			}
		}
		else if(strcmp(dataType, _ADLINK_STRING2_) == 0) //16bit ASCII, 16bit hex
		{
			for(int j = dwDataLen-1; j >= 0; j--)
			{
				strVal += (char)pData[j];
			}
		}

		else	//do inverse
		{
			for(int j = dwDataLen-1; j >= 0; j--)
			{
				strVal += IntToHex(pData[j], 2);
			}
			strVal = "0x" + strVal;
		}
		
		*str = strVal;
	}
	return nErr;

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::displayOdFunc(bool isShow)
{
	if(isShow)
	{
		vleCardInfo->Show();
		btnGetOd->Show();
		this->SetBounds(0, 0, 750, 530);
	}
	else
	{
		vleCardInfo->Hide();
		btnGetOd->Hide();
		this->SetBounds(0, 0, 510, 530);
	}
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::displayFoePrg(int percent)
{
/*	int total = foeTotal;
	int currPrg = (float)percent/total * 100;
	foePrgBar->Max = total;
	foePrgBar->Position = percent;
	foePrgForm->Caption = "Downloading: " + AnsiString(currPrg) + "%";
	if(foePrgBar->Position >= foePrgBar->Max)
	{
		foePrgBar->Position = 0;
		foePrgForm->Caption = "Downloading: " + AnsiString(foePrgBar->Position) + "%";
		foePrgForm->Hide();
	}*/

	api->displayPrg(this->foePrgForm, this->foePrgBar, "Downloading: ", percent, 0, foeTotal);
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::updateSlaveNum(int wSlaveNo, int adlinkNo, API_Class *api)
{
    this->wSlaveNo = wSlaveNo;
	this->adlinkNo = adlinkNo;
	this->api = api;
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::updateSlaveInfo()
{
    short nErr = ECMaster_ERROR_SUCCESS;
    ULONG dwDataLen = 0;
	AnsiString err;
    
    ULONG step = CONFIGURATION_OBJECTS_AREA;
	int scanMode = 1;	//1: scan current module	2:read moudule from flash	3:scan current module and update flash

	int moNum = -1;
	for(int i = 0; i < TOTAL_SLOTS; i++)
	{
		slotToMo[i] = -1;
	}

	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), 0x100C, 1, &scanMode);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "updateSlaveInfo(): Slave "+AnsiString(wSlaveNo)+" write scan mode fail");
	}

	for(int i = 0; i < general_bc_size; i ++)
	{
		//nErr = SendSyncCoEUploadReq(wSlaveNo, p_general_bc_info[i].dataSize, p_general_bc_info[i].index, p_general_bc_info[i].subIndex, nData+i, &dwDataLen);
		nErr = odToString(wSlaveNo, p_general_bc_info[i].dataSize, p_general_bc_info[i].index, p_general_bc_info[i].subIndex, p_general_bc_info[i].datatype, nData+i, &err);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "updateSlaveInfo(): Slave "+AnsiString(wSlaveNo)+" index 0x" +IntToHex((int)p_general_bc_info[i].index,2)+":"+IntToHex((int)p_general_bc_info[i].subIndex,2)+" " +err);
		}
	}
	//LastEcError_Get(&ecErr);
	updateBcInfo(nData, nErr);

	for(int i = 0; i < dc_parameter_size; i ++)
	{
		nErr = odToString(wSlaveNo, p_dc_parameter_info[i].dataSize, p_dc_parameter_info[i].index, p_dc_parameter_info[i].subIndex, p_dc_parameter_info[i].datatype, nDcData+i, &err);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "updateSlaveInfo(): Slave "+AnsiString(wSlaveNo)+" index 0x" +IntToHex((int)p_dc_parameter_info[i].index,2)+":"+IntToHex((int)p_dc_parameter_info[i].subIndex,2)+" " +err);
		}
	}
	updateDcInfo(nDcData, dc_parameter_size, nErr);

	for(int i = 0; i < TOTAL_SLOTS; i++ )
	{
		m_dwModuleSlot[i] = 0x00000000;
		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0x100C, i+2, &m_dwModuleSlot[i], &dwDataLen);
		//nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0xF030, i+1, &m_dwModuleSlot[i], &dwDataLen);
		if(nErr == ECMaster_ERROR_SUCCESS)
		{
			if(m_dwModuleSlot[i] != 0x00000000)
			{
				//count MO number
				if(m_dwModuleSlot[i] == EPS7002_ID)
				{
					moNum++;
					slotToMo[i] = moNum;
				}
				for(int j= 0; j < general_slot_size; j++)
				{
					//nErr = SendSyncCoEUploadReq(wSlaveNo, p_general_slot_info[j].dataSize, p_general_slot_info[j].index+step*i, p_general_slot_info[j].subIndex, &nSlotData[i][j], &dwDataLen);
					nErr = odToString(wSlaveNo, p_general_slot_info[j].dataSize, p_general_slot_info[j].index+step*i, p_general_slot_info[j].subIndex, p_general_slot_info[j].datatype, nSlotData+j, &err);
					if(nErr != ECMaster_ERROR_SUCCESS)
					{
						api->displayLogMsg(nErr, "updateSlaveInfo(): Slave "+AnsiString(wSlaveNo)+", Slot " + AnsiString(i) +", index 0x" +IntToHex((int)p_general_slot_info[j].index,2)+":"+IntToHex((int)p_general_slot_info[j].subIndex,2)+" "+err);
					}
				}
			}
			
			//updateBcInfo(nData, ecErr);
			
			switch(m_dwModuleSlot[i])
			{
			   case EPS1132_ID:
				   updateSlotInfo(i, "EPS-1132.jpg", nSlotData, nErr);
				   break;
			   case EPS4008_ID:
				   updateSlotInfo(i, "EPS-4008.jpg", nSlotData, nErr);
				   break;
			   case EPS3032_ID:
				   updateSlotInfo(i, "EPS-3032.jpg", nSlotData, nErr);
				   break;
			   case EPS3216_ID:
				   updateSlotInfo(i, "EPS-3216.jpg", nSlotData, nErr);
				   break;
			   case EPS2032_ID:
				   updateSlotInfo(i, "EPS-2032.jpg", nSlotData, nErr);
				   break;
			   case EPS2132_ID:
				   updateSlotInfo(i, "EPS-2132.jpg", nSlotData, nErr);	//wait EPS-2132.jpg
				   break;				   
			   case EPS3504_ID:
				   updateSlotInfo(i, "EPS-3504.jpg", nSlotData, nErr);
				   break;
			   case EPS2308_ID:
				   updateSlotInfo(i, "EPS-2308.jpg", nSlotData, nErr);
				   break;
			   case EPS7002_ID:
				   updateSlotInfo(i, "EPS-7002.jpg", nSlotData, nErr);
				   break;
			   default:
				   clearSlotInfo(i, "slot_empty.jpg");
				   break;

			}
		}
		else
		{
			api->displayLogMsg(nErr, "updateSlaveInfo(): Get slot module " +AnsiString(i) + " fail");
		}
	}

    //return nErr;
    this->Show();
	this->isSlaveUpdate = false;
	
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::updateBcInfo(AnsiString *nData, long nErr)
{
	tbshtBp->Caption = "Control Panel";
	tbshtBc->Caption = nData[0];
	edBcInfo0->Text = nData[1];
	edBcInfo1->Text = nData[2];
	edBcInfo2->Text = nData[3];
	edBcInfo3->Text = nData[4];
	edBcInfo4->Text = nData[5];
	edBcInfo5->Text = nData[6];
	edBcInfo6->Text = nData[7];
	//nData[9] = StringReplace(nData[9], "0x", "", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
	edBcInfo7->Text = nData[9] + nData[8];
	//edBcInfo8->Text = "0x" + IntToHex((int)nErr, 8);
	edBcInfo8->Text = AnsiString(nErr);

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::updateDcInfo(AnsiString *nData, int size, long nErr)
{
	imgDcPara->Picture->LoadFromFile(systemPath+"\\slot\\DC_timing.jpg");
	for(int i = 0; i < size; i ++)
	{
		((TEdit *)FindComponent("edPdoData" + IntToStr(i)))->Text = nData[i];
	}
	
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::updateSlotInfo(int slot, AnsiString img, AnsiString *nSlotData, long nErr)
{
    ((TTabSheet *)FindComponent("tbshtSlot" + IntToStr(slot)))->Caption = nSlotData[0];
    ((TImage *)FindComponent("imgBpSlot" + IntToStr(slot)))->Picture->LoadFromFile(systemPath+"\\slot\\"+img);
    ((TImage *)FindComponent("imgSlot" + IntToStr(slot)))->Picture->LoadFromFile(systemPath+"\\slot\\"+img);
    ((TEdit *)FindComponent("edModType" + IntToStr(slot)))->Text = nSlotData[1];
    ((TEdit *)FindComponent("edMod" + IntToStr(slot)))->Text = nSlotData[2];
    ((TEdit *)FindComponent("edHwVer" + IntToStr(slot)))->Text = nSlotData[3];
    ((TEdit *)FindComponent("edSwVer" + IntToStr(slot)))->Text = nSlotData[4];
	//nSlotData[6] = StringReplace(nSlotData[6], "0x", "", TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
    ((TEdit *)FindComponent("edAdlinkSerial" + IntToStr(slot)))->Text = nSlotData[6] + nSlotData[5];
    //((TEdit *)FindComponent("edErr" + IntToStr(slot)))->Text = "0x" + IntToHex((int)nErr, 8);
    ((TEdit *)FindComponent("edErr" + IntToStr(slot)))->Text = AnsiString(nErr);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::clearSlotInfo(int slot, AnsiString img)
{
    //((TTabSheet *)FindComponent("tbshtSlot" + IntToStr(slot)))->Caption = "Slot"+IntToStr(slot-1);
    ((TTabSheet *)FindComponent("tbshtSlot" + IntToStr(slot)))->Caption = "Slot"+IntToStr(slot);
    ((TImage *)FindComponent("imgBpSlot" + IntToStr(slot)))->Picture->Assign(NULL);
    //((TImage *)FindComponent("imgBpSlot" + IntToStr(slot)))->Picture->LoadFromFile("slot\\"+img);
    ((TImage *)FindComponent("imgSlot" + IntToStr(slot)))->Picture->LoadFromFile(systemPath+"\\slot\\"+img);
    //((TImage *)FindComponent("imgBpSlot" + IntToStr(slot)))->Picture->LoadFromFile("slot\\slot_empty.jpg");
    //((TImage *)FindComponent("imgSlot" + IntToStr(slot)))->Picture->LoadFromFile("slot\\slot_empty.jpg");
	((TEdit *)FindComponent("edModType" + IntToStr(slot)))->Clear();
    ((TEdit *)FindComponent("edMod" + IntToStr(slot)))->Clear();
    ((TEdit *)FindComponent("edHwVer" + IntToStr(slot)))->Clear();
    ((TEdit *)FindComponent("edSwVer" + IntToStr(slot)))->Clear();
    ((TEdit *)FindComponent("edAdlinkSerial" + IntToStr(slot)))->Clear();
    ((TEdit *)FindComponent("edErr" + IntToStr(slot)))->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TformSlave::showSlotPage(int slot)
{
	//short nErr;
	//ULONG dwDataLen;

	if(api->windowsCnt == MAX_WINDOW_SUPPORT && m_dwModuleSlot[slot] != 0)
	{
		api->displayLogMsg(ECMaster_ERROR_EXTERNAL_ERROR, "Panels open are only allowed at most "+AnsiString(MAX_WINDOW_SUPPORT));
		return;
	}

	switch(m_dwModuleSlot[slot])
	{
		case EPS1132_ID:	//DI32
			if(eps1132[slot] == NULL)
			{
				eps1132[slot] = new TformDi1132(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps1132[slot]->Show();
			break;
			
		case EPS4008_ID:	//AO16
			if(eps4008[slot] == NULL)
			{
				eps4008[slot] = new TformAo4008(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps4008[slot]->Show();
			break;
			
		case EPS3032_ID:	//AI32
			if(eps3032[slot] == NULL)
			{
				eps3032[slot] = new TformAi3032(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps3032[slot]->Show();
			break;
			
		case EPS3216_ID:	//AI16
			if(eps3216[slot] == NULL)
			{
				eps3216[slot] = new TformAi3216(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps3216[slot]->Show();
			break;
			
		case EPS2032_ID:	//DO32
			if(eps2032[slot] == NULL)
			{
				eps2032[slot] = new TformDo2032(this, api, wSlaveNo, adlinkNo, slot, "EPS-2032");
			}
			eps2032[slot]->Show();
			break;
	
		case EPS2132_ID:	//DO32
			if(eps2132[slot] == NULL)
			{
				eps2132[slot] = new TformDo2032(this, api, wSlaveNo, adlinkNo, slot, "EPS-2132");
			}
			eps2132[slot]->Show();
			break;
			
		case EPS3504_ID:	//RTD
			if(eps3504[slot] == NULL)
			{
				eps3504[slot] = new TformRtd3504(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps3504[slot]->Show();
			break;
			
		case EPS2308_ID:	//RO
			if(eps2308[slot] == NULL)
			{
				eps2308[slot] = new TformRo2308(this, api, wSlaveNo, adlinkNo, slot);
			}
			eps2308[slot]->Show();
			break;

		case EPS7002_ID:	//MO
			if(eps7002[slot] == NULL)
			{
				eps7002[slot] = new TformMo7002(this, api, wSlaveNo, adlinkNo, slotToMo[slot], slot);
			}
			eps7002[slot]->Show();
		default:
			break;
	}

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::imgBpSlot0Click(TObject *Sender)
{
	showSlotPage(0);
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::imgBpSlot1Click(TObject *Sender)
{
	showSlotPage(1);
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::imgBpSlot2Click(TObject *Sender)
{
	showSlotPage(2);
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::imgBpSlot3Click(TObject *Sender)
{
	showSlotPage(3);
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::FormDestroy(TObject *Sender)
{
    free(nData);
    free(nSlotData);
    vleErrLog->Refresh();
    richEdErr->Refresh();
    vleCardInfo->Refresh();

	for(int i = 0; i <TOTAL_SLOTS; i++)
	{
		switch(m_dwModuleSlot[i])
		{
			case EPS1132_ID:	//DI32
				if(eps1132[i] != NULL)
				{
					eps1132[i]->Release();
				}
				break;
				
			case EPS4008_ID:	//AO16
				if(eps4008[i] != NULL)
				{
					eps4008[i]->Release();
				}	
				break;
				
			case EPS3032_ID:	//AI32
				if(eps3032[i] != NULL)
				{
					eps3032[i]->Release();
				}
				break;
				
			case EPS3216_ID:	//AI16
				if(eps3216[i] != NULL)
				{
					eps3216[i]->Release();
				}
				break;
				
			case EPS2032_ID:	//DO32
				if(eps2032[i] != NULL)
				{
					eps2032[i]->Release();
				}
				break;
		
			case EPS2132_ID:	//DO32
				if(eps2132[i] != NULL)
				{
					eps2132[i]->Release();
				}
				break;
				
			case EPS3504_ID:	//RTD
				if(eps3504[i] != NULL)
				{
					eps3504[i]->Release();
				}
				break;
				
			case EPS2308_ID:	//RO
				if(eps2308[i] != NULL)
				{
					eps2308[i]->Release();
				}
				break;
			default:
				break;
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::btnRefreshClick(TObject *Sender)
{
    isRefreshClick = true;
	enableComponent(false);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::refreshErrLogTbl()
{
    short nErr = ECMaster_ERROR_SUCCESS;
    ULONG dwDataLen = 0;
	UINT enabled = 1;
	USHORT totalCnt;
	UINT low32Bit, high32Bit;

    vleErrLog->Refresh();
	vleErrLog->Strings->Clear();
	//enable log
	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), 0x1005, 1, &enabled);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Enable log fail");
	}

	//get total count in flash
    nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0x1005, 3, &totalCnt, &dwDataLen);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Get total log count fail");
	}

	for(USHORT i = 0; i < totalCnt; i++)
	{
		nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), 0x1005, 4, &i);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "Write address " + AnsiString(i) + " fail");
		}

		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0x1005, 5, &low32Bit, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "Address " + AnsiString(i) + " get low 32 bit fail");
		}

		nErr = SendSyncCoEUploadReq(wSlaveNo, sizeof(ULONG), 0x1005, 6, &high32Bit, &dwDataLen);
		if(nErr != ECMaster_ERROR_SUCCESS)
		{
			api->displayLogMsg(nErr, "Address " + AnsiString(i) + " get high 32 bit fail");
		}

		vleErrLog->InsertRow(AnsiString(i), "0x"+IntToHex((int)high32Bit, 8)+IntToHex((int)low32Bit, 8), true);
	}
	selectCell(1, 1);

	isRefreshClick = false;
	enableComponent(true);

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::btnClearClick(TObject *Sender)
{
	isClearClick = true;
	enableComponent(false);
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::clearFlash()
{
    short nErr = ECMaster_ERROR_SUCCESS;
	UINT enabled = 1;
	UINT clear = 1;

	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), 0x1005, 1, &enabled);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Enable log fail");
	}

	nErr = SendSyncCoEDownloadReq(wSlaveNo, sizeof(ULONG), 0x1005, 2, &clear);
	if(nErr != ECMaster_ERROR_SUCCESS)
	{
		api->displayLogMsg(nErr, "Clear flash fail");
	}

	richEdErr->Clear();
    vleErrLog->Strings->Clear();

	isClearClick = false;
	enableComponent(true);

}
//---------------------------------------------------------------------------
void __fastcall TformSlave::selectCell(int col, int row)
{
    unsigned long long value;
    AnsiString strVal;//, from = "0x", to = "";
    if(col == 1)
    {
        strVal = vleErrLog->Cells[col][row];
        if(!strVal.IsEmpty())
        {
            value = StrToInt64(strVal);
            errBit2Msg(value);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TformSlave::errBit2Msg(unsigned long long value)
{
	int bit;
    AnsiString strErr;
    richEdErr->Clear();
	for(int i = 0; i < err_info_size; i++)
	{
		bit = value >> p_err_info[i].bit & 0x01;
		if(bit == 0x01)
		{
            //strcpy(strErr.c_str(), p_err_info[i].err);
            richEdErr->Lines->Append("Bit "+AnsiString(p_err_info[i].bit) +": "+ AnsiString(p_err_info[i].err));
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TformSlave::vleErrLogSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    if(Sender != NULL)
    {
        selectCell(ACol, ARow);
    }
}
//---------------------------------------------------------------------------

