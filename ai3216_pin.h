//---------------------------------------------------------------------------

#ifndef ai3216_pinH
#define ai3216_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TformAi3216 : public TForm
{
__published:	// IDE-managed Components
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
		USHORT SlaveNo;
		int Slot;
		API_Class *api;
        //float VolAi16[AI16_NUM];
        TLabel *LbInfo;
        TLabel *Lb[AI16_NUM];
        TEdit *Edit[AI16_NUM];
        
public:		// User declarations
        __fastcall TformAi3216(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall readData();
};
//---------------------------------------------------------------------------
extern PACKAGE TformAi3216 *formAi3216;
//---------------------------------------------------------------------------
#endif
