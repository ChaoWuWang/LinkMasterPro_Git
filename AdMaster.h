#ifndef _AD_MASTER_H
#define _AD_MASTER_H


#ifndef ETHERCAT_SLAVE_MAX
#define ETHERCAT_SLAVE_MAX		65535
#endif

#define AD_SLAVE_TYPE_UNKNOWN               0xFFFF
#define AD_SLAVE_TYPE_BC6000                0x0001

#include "ESI_Export.h"

#ifndef DEVICE_STATE_INIT
#define	DEVICE_STATE_INIT						0x0001
#endif

#ifndef DEVICE_STATE_PREOP
#define	DEVICE_STATE_PREOP					0x0002
#endif

#ifndef DEVICE_STATE_BOOTSTRAP
#define	DEVICE_STATE_BOOTSTRAP				0x0003
#endif 


// Error code definitions

#define ECMaster_ERROR_SUCCESS					      0
#define ECMaster_ERROR_INACTIVE					     -1
#define ECMaster_ERROR_INVALID_SLAVE_NUM		     -2
#define ECMaster_ERROR_SLAVE_UNSUPPORT			     -3
#define ECMaster_ERROR_INITIALIZE_FAILED		     -4
#define ECMaster_ERROR_PREOP_TIMEOUT			     -5
#define ECMaster_ERROR_BUFFER_ALLOCATE			     -6
#define ECMaster_ERROR_INTERNAL_ERROR			     -7
#define ECMaster_ERROR_INVALID_TARGET_FILENAME       -8
#define ECMaster_ERROR_MALLOC_FAILED                 -9
#define ECMaster_ERROR_MSXML2_FAILED                 -10
#define ECMaster_ERROR_FoE_UNEXPECTED_RESPONSE_SIZE  -11
#define ECMaster_ERROR_INVALID_SOURCE_FILENAME       -12
#define ECMaster_ERROR_NO_ACTIVE_SLAVE               -13
#define ECMaster_ERROR_INVALID_MASTER_STATE          -14
#define ECMaster_ERROR_MASTER_SYNCH_BUSY             -15
#define ECMaster_ERROR_UNSUPPORTED_MASTER_REQ_STATE  -16
#define ECMaster_ERROR_ZERO_WORKING_COUNT            -17  // 20150620 added
#define ECMaster_ERROR_NOT_ENOUGH_EEPROM             -19  // 20151101 added

#define ECMaster_ERROR_EXTERNAL_ERROR				-100


#define CREATE_NEW_MULTIPLE_DEVICES_ESI               0
#define APPEND_TO_MULTIPLE_DEVICES_ESI                1


#ifdef __cplusplus
extern "C" {
#endif

short __stdcall StartECMaster( BYTE* pMacAddr);
short __stdcall StopECMaster();
short __stdcall ReScanSlaves( BYTE* pMacAddr);
short __stdcall GetSlaves( int* pAvailSlaves, USHORT DevType[] );
short __stdcall SendSyncCoEUploadReq( USHORT wSlaveIndex, USHORT wDataBytes, int Index, int SubIndex, PVOID pCoEData, ULONG* pDataLen );
short __stdcall SendSyncCoEDownloadReq( USHORT wSlaveIndex, USHORT wDataBytes, int Index, int SubIndex, PVOID pCoEData );
short __stdcall SendSyncEcFrame(USHORT wSlaveIndex, BYTE bCommand, USHORT wADO, ULONG nData, PUCHAR pSndBuf, PUCHAR pRcvBuf );
// ESI function
short __stdcall ESIObjectDT9000Export( long nSlotNum, ESI_OBJECT_DT9000 *pESIObject, TCHAR strXMLSourceFileName[], TCHAR strXMLTargetFileName[] );
short __stdcall ESIObjectExport( long nSlotNum, ESI_OBJECT *pESIObject, TCHAR strXMLSourceFileName[], TCHAR strXMLTargetFileName[], long nAxisNum );
short __stdcall ESIObjectExExport(long nSlotNum, ESI_OBJECT_EX *pESIObjectEx, TCHAR strXMLSourceFileName[], TCHAR strXMLTargetFileName[], long nAxisNum );
short __stdcall ESIPDOExport( long nSlotNum, ESI_DEVICE_PDO *pESIPDO, TCHAR strXMLSourceFileName[], TCHAR strXMLTargetFileName[], long nAxisNum );
short __stdcall ESISyncMChange( TCHAR strXMLTargetFileName[], int PDOType, long nDefaultSize, long nPDONumber, unsigned short wPdoIndex[] );
short __stdcall ESIProductCodeChange( TCHAR strXMLTargetFileName[], unsigned long dwProductCode );
short __stdcall ESIDevicePhysicsChange( TCHAR strXMLTargetFileName[], unsigned long dwProductCode, unsigned long dwRevisionNo );
short __stdcall ESIProfileChange( TCHAR strXMLTargetFileName[], long nProfileNo, long nChannelCount );
short __stdcall ESISlotChange( TCHAR strXMLTargetFileName[], int SlotNo, unsigned short wMouldeID );
short __stdcall ESISlotsDelete( TCHAR strXMLTargetFileName[] );
short __stdcall ESIDeviceAppend( TCHAR strXMLSourceFileName[], TCHAR strXMLTargetFileName[], USHORT wFileOperation );
// EEPROM functions
short __stdcall SII_EEPROM_Init( SII_MODULE_PDO SIIModulePDO[], int nModulePDO, UCHAR BusCouple_Eeprom[], unsigned long dwProductCode, unsigned long dwRevisionNo );
// short __stdcall SII_EEPROM_Init( SII_MODULE_PDO SIIModulePDO[], int nModulePDO, UCHAR BusCouple_Eeprom[], unsigned long dwProductCode );
short __stdcall SII_EEPROM_Write( USHORT wSlaveIndex, UCHAR BusCouple_Eeprom[] );
short __stdcall EEPROM_ProdoctCode_Read( USHORT wSlaveIndex, ULONG *pdwProductCode );
short __stdcall Slave_EEPROM_Read( USHORT wSlaveIndex, ULONG dwEepromAddr, ULONG *pdwData );
short __stdcall SII_EEPROM_Write_All( USHORT wSlaveIndex, UCHAR BusCouple_Eeprom[] ); // for QC use only

// BootStrap support
short __stdcall StateMachineGet(USHORT* pCurrState);
short __stdcall StateMachineRequest(USHORT wReqState);


// FoE Fucntions
short __stdcall FoE_Download_FileOpen(USHORT wSlaveIndex, TCHAR FwFileName[], ULONG* pFileHandle);
short __stdcall FoE_Download_DataWrite(USHORT wSlaveIndex, ULONG dwFileHandle, BYTE* pWrtData, ULONG dwWrtDataLen);
short __stdcall FoE_Download_FileClose(USHORT wSlaveIndex, ULONG dwFileHandle);

void __stdcall LastEcError_Get( long *nEcError);

#ifdef __cplusplus
}
#endif

#endif // _AD_MASTER_H
