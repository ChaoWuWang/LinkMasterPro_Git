//---------------------------------------------------------------------------

#ifndef di1132_pinH
#define di1132_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include <ExtCtrls.hpp>
#include "DBUILib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>

//---------------------------------------------------------------------------
class TformDi1132 : public TForm
{
__published:	// IDE-managed Components
        TDBoolean *DBoolean1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
		API_Class *api;
		USHORT SlaveNo;
		int Slot;
		TDBoolean	*dBool[DI32_NUM];
                
public:		// User declarations
        __fastcall TformDi1132(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall readData();
};
//---------------------------------------------------------------------------
extern PACKAGE TformDi1132 *formDi1132;
//---------------------------------------------------------------------------
#endif
