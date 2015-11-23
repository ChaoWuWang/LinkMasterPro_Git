#ifndef _ESI_EXPORT_H_
#define _ESI_EXPORT_H_

#ifndef BusCouple_Eeprom_Length

#define _EEPROM_4096_	1

#ifdef _EEPROM_4096_
#define BusCouple_Eeprom_Length         4096  // 4K Bytes
#else
#define BusCouple_Eeprom_Length         2048  // 16K bits, 2K Bytes
#endif

#endif

#define PDO_ENTRY_INDEX_BY_SLOT 0
#define PDO_ENTRY_INDEX_BY_AXIS 1

#define OBJECT_INDEX_BY_SLOT    0
#define OBJECT_INDEX_BY_AXIS    1


#define DEFTYPE_NONE            0x0000
// Table 63 - Basic Data Type
#define DEFTYPE_BOOLEAN         0x0001
#define DEFTYPE_INTEGER8        0x0002
#define DEFTYPE_INTEGER16       0x0003
#define DEFTYPE_INTEGER32       0x0004
#define DEFTYPE_UNSIGNED8       0x0005
#define DEFTYPE_UNSIGNED16      0x0006
#define DEFTYPE_UNSIGNED32      0x0007
#define DEFTYPE_REAL32          0x0008
#define DEFTYPE_VISIBLE_STRING  0x0009
#define DEFTYPE_OCTET_STRING    0x000A
#define DEFTYPE_UNICODE_STRING  0x000B
#define DEFTYPE_TIME_OF_DAY     0x000C
#define DEFTYPE_TIME_DIFFERENCE 0x000D
//#define DEFTYPE_Reserved        0x000E
#define DEFTYPE_DOMAIN          0x000F
#define DEFTYPE_INTEGER24       0x0010
#define DEFTYPE_REAL64          0x0011
#define DEFTYPE_INTEGER40       0x0012
#define DEFTYPE_INTEGER48       0x0013
#define DEFTYPE_INTEGER56       0x0014
#define DEFTYPE_INTEGER64       0x0015
#define DEFTYPE_UNSIGNED24      0x0016
//#define DEFTYPE_Reserved        0x0017
#define DEFTYPE_UNSIGNED40      0x0018
#define DEFTYPE_UNSIGNED48      0x0019
#define DEFTYPE_UNSIGNED56      0x001A
#define DEFTYPE_UNSIGNED64      0x001B
//#define DEFTYPE_Reserved        0x001C-0x002C
#define DEFTYPE_BITARR8         0x002D
#define DEFTYPE_BITARR16        0x002E
#define DEFTYPE_BITARR32        0x002F


#define RxPDO_TYPE 0
#define TxPDO_TYPE 1

#define MAX_DEVICE_PDO 4
#define MAX_DEVICE_PDO_ENTRIES 32
// #define MAX_OBJECT_INFO_SUBITEMS 16
#define MAX_OBJECT_INFO_SUBITEMS_EX 10
#define MAX_OBJECT_INFO_SUBITEMS 40

#define ESI_OBJECT_PDO_ENTRYNAME_LENGTH       40 // 30
#define ESI_DEVICE_PDO_NAME_LENGTH            40
#define ESI_OBJECT_INFO_SUBITEM_NAME_LENGTH   40
#define ESI_OBJECT_NAME_LENGTH                50 // 40

typedef struct _ESI_OBJECT_PDO_ENTRY_
{
	USHORT wEntryIdxBase; // 0x6000, 0x7000, 0x8000
	USHORT wEntryIdxMultiple; // Entry Index = wEntryIdxBase + (wEntryIdxMultiple * nSlotNum);
	USHORT wSubIndex;
	USHORT wBitLen;
	USHORT wDataType;
	BYTE   bNameStringIdx; // Index in STRINGS Category
	CHAR EntryName[ESI_OBJECT_PDO_ENTRYNAME_LENGTH]; //   
	CHAR DataType[20]; // 

} ESI_OBJECT_PDO_ENTRY;

typedef struct _ESI_DEVICE_PDO_
{
	CHAR PDOType; // 0: RxPDO, 1: TxPDO
	bool bMandatory; // true, false
	bool bFixed; // true, false
	int SynchM; // related Synch. Manager
  
	USHORT wIdxBase; // 0x1600, 0x1A00
	USHORT wIdxMultiple; // wIndex = wIdxBase + (wIdxMultiple * nSlotNum);
	USHORT wEntryIdxRefer; // PDO_ENTRY_INDEX_BY_SLOT / PDO_ENTRY_INDEX_BY_AXIS;	
  
  BYTE bNameStringIdx; // Index in STRINGS Category
	CHAR PdoName[ESI_DEVICE_PDO_NAME_LENGTH]; //

	int ValidEntries;	 

	ESI_OBJECT_PDO_ENTRY PdoEntry[MAX_DEVICE_PDO_ENTRIES];	
}ESI_DEVICE_PDO, *PESI_DEVICE_PDO;

typedef struct _ESI_OBJECT_INFO_SUBITEM_
{
	CHAR ItemName[ESI_OBJECT_INFO_SUBITEM_NAME_LENGTH]; // 
	struct
	{
		USHORT wDataType; // 20141119 added to indicate the length-of-number
		//long DefaultData;
		unsigned long DefaultData;		
	}Info;
} ESI_OBJECT_INFO_SUBITEM;

typedef struct _ESI_OBJECT_INFO_
{
	USHORT wDataType; // 20141119 added to indicate the length-of-number
	//int InfoDefaultData;
	unsigned long InfoDefaultData;	
	int ValidSubItems;
	ESI_OBJECT_INFO_SUBITEM SubItem[MAX_OBJECT_INFO_SUBITEMS];
	
} ESI_OBJECT_INFO;

typedef struct _ESI_OBJECT_INFO_SUBITEM_EX_
{
	CHAR ItemName[ESI_OBJECT_INFO_SUBITEM_NAME_LENGTH]; // 
	struct
	{
		USHORT wDataType; // 20141119 added to indicate the length-of-number
		CHAR bValidData; // indicate the DefaultData is valid; otherwise the MinValue/MaxValue/DefaultValue are valid
		//long DefaultData;
		unsigned long DefaultData;
		
		unsigned long MinValue; // added for 3504
		unsigned long MaxValue;
		unsigned long DefaultValue;

	}Info;
} ESI_OBJECT_INFO_SUBITEM_EX;

typedef struct _ESI_OBJECT_INFO_EX_
{
	USHORT wDataType; // 20141119 added to indicate the length-of-number
	//int InfoDefaultData;
	unsigned long InfoDefaultData;	
	int ValidSubItems;
	ESI_OBJECT_INFO_SUBITEM_EX SubItem[MAX_OBJECT_INFO_SUBITEMS_EX];
	
} ESI_OBJECT_INFO_EX;

typedef struct _ESI_OBJECT_DT9000_INFO_
{
	USHORT wDataType; // 20141119 added to indicate the length-of-number
	//int InfoDefaultData;
	unsigned long InfoDefaultData;	
	int ValidSubItems;
	
	ESI_OBJECT_INFO_SUBITEM SubIndex0;
	CHAR ItemName1[ESI_OBJECT_INFO_SUBITEM_NAME_LENGTH];
	CHAR Type[20]; // /w NULL terminator
	CHAR ItemName2[ESI_OBJECT_INFO_SUBITEM_NAME_LENGTH];
	CHAR Name[10]; // /w NULL terminator
	ESI_OBJECT_INFO_SUBITEM SubItem[14];
	
} ESI_OBJECT_DT9000_INFO;

typedef struct _ESI_OBJECT_FLAGS_
{
	CHAR Access[6]; // ro, rw
	CHAR Category[6]; // o 
	CHAR PdoMapping[6]; // R 
	
} ESI_OBJECT_FLAGS;

typedef struct _ESI_OBJECT_DT9000_
{
	USHORT wIdxBase; // 0x1600, 0x1A00, 0x6000, 0x7000, 0x8000
	USHORT wIdxMultiple; // wIndex = wIdxBase + (wIdxMultiple * nSlotNum);
	CHAR ObjectName[ESI_OBJECT_NAME_LENGTH]; // 
	CHAR ObjectType[30]; //
	int BitSize;
	
	// Info
	ESI_OBJECT_DT9000_INFO ObjectInfo; 
	
	ESI_OBJECT_FLAGS ObjectFlags;
	
}ESI_OBJECT_DT9000;

typedef struct _ESI_OBJECT_
{
	USHORT wIdxBase; // 0x1600, 0x1A00, 0x6000, 0x7000, 0x8000
	USHORT wIdxMultiple; // wIndex = wIdxBase + (wIdxMultiple * nSlotNum);
	USHORT wObjectIdxRefer; // OBJECT_INDEX_BY_SLOT / OBJECT_INDEX_BY_AXIS;		
	CHAR ObjectName[ESI_OBJECT_NAME_LENGTH]; // 
	CHAR ObjectType[30]; //
	int BitSize;
	
	// Info
	ESI_OBJECT_INFO ObjectInfo; 
	
	ESI_OBJECT_FLAGS ObjectFlags;
	
}ESI_OBJECT;

typedef struct _ESI_OBJECT_EX_
{
	USHORT wIdxBase; // 0x1600, 0x1A00, 0x6000, 0x7000, 0x8000
	USHORT wIdxMultiple; // wIndex = wIdxBase + (wIdxMultiple * nSlotNum);
	USHORT wObjectIdxRefer; // OBJECT_INDEX_BY_SLOT / OBJECT_INDEX_BY_AXIS;		
	CHAR ObjectName[ESI_OBJECT_NAME_LENGTH]; // 
	CHAR ObjectType[30]; //
	int BitSize;
	
	// Info
	ESI_OBJECT_INFO_EX ObjectInfo; 
	
	ESI_OBJECT_FLAGS ObjectFlags;
	
}ESI_OBJECT_EX;

typedef struct _SII_MODULE_PDO_
{
    int nSlotNumber; // indicate the slot that the module is installed
	int nPdoNumber;	 // the total PDOs 
    int nAxisNumber; // indicate the starting-axis of the MO module

	PESI_DEVICE_PDO pPdoList[MAX_DEVICE_PDO];
}SII_MODULE_PDO;

#endif // _ESI_EXPORT_H_