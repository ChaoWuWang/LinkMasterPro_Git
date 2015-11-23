//---------------------------------------------------------------------------

#ifndef rtd3504_pinH
#define rtd3504_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include <ExtCtrls.hpp>


#define NoThermoError                  0

#define ErrorInvalidThermoType      -601
#define ErrorOutThermoRange         -602
#define ErrorThermoTable            -603


//---------------------------------------------------------------------------
class TformRtd3504 : public TForm
{
__published:	// IDE-managed Components
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
		API_Class *api;
		USHORT SlaveNo;
		int Slot;
        TLabel *LbInfo, *LbIndex;
		TComboBox *CbIndex;
        TLabel *Lb[RTD_NUM];
        TEdit *EditOhm[RTD_NUM];
		TEdit *EditTemp[RTD_NUM];
        
public:		// User declarations
        __fastcall TformRtd3504(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall readData();
		short __fastcall ADC_to_Thermo( double fScaledADC, double* pfTemp );
};
//---------------------------------------------------------------------------
extern PACKAGE TformRtd3504 *formRtd3504;
//---------------------------------------------------------------------------
#endif
