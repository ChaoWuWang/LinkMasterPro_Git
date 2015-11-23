//---------------------------------------------------------------------------

#ifndef ai3032_pinH
#define ai3032_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TformAi3032 : public TForm
{
__published:	// IDE-managed Components
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
		USHORT SlaveNo;
		int Slot;
		API_Class *api;
        //float VolAi32[32];
        TLabel *LbInfo;
        TLabel *Lb[AI32_NUM];
        TEdit *Edit[AI32_NUM];
        
public:		// User declarations
        __fastcall TformAi3032(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall readData();
};
//---------------------------------------------------------------------------
extern PACKAGE TformAi3032 *formAi3032;
//---------------------------------------------------------------------------
#endif
