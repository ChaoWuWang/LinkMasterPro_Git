//---------------------------------------------------------------------------

#ifndef ro2308_pinH
#define ro2308_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include "DBUILib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>

//---------------------------------------------------------------------------
class TformRo2308 : public TForm
{
__published:	// IDE-managed Components
        TDBoolean *DBoolean1;
        void __fastcall FormShow(TObject *Sender);
		void __fastcall __OnClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
		API_Class *api;
		USHORT SlaveNo;
		int Slot;
		TDBoolean	*dBool[RO_NUM];

public:		// User declarations
		int state;
		//bool isWrite;
		__fastcall TformRo2308(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall writeData();
		void __fastcall readData();
		
};
//---------------------------------------------------------------------------
extern PACKAGE TformRo2308 *formRo2308;
//---------------------------------------------------------------------------
#endif
