//---------------------------------------------------------------------------

#ifndef ao4008_pinH
#define ao4008_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"

//---------------------------------------------------------------------------
class TformAo4008 : public TForm
{
__published:	// IDE-managed Components
        TButton *btnApply;
        TButton *btnReset;
        void __fastcall btnApplyClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnResetClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        API_Class *api;
        USHORT SlaveNo;
        int Slot;
        //USHORT Raw[AO_NUM];
        TLabel *LbInfo;
        TLabel *Lb[AO_NUM];
        TScrollBar *ScrBar[AO_NUM];
        TEdit *Edit[AO_NUM];
        void __fastcall __ScrollBar_OnChange( TObject *Sender );
        void __fastcall barPosToVol(int i);
		USHORT __fastcall barPosToRawTransfer(USHORT input);
public:		// User declarations
		int state;	//0:read, 1:do nothing, 2:write
		__fastcall TformAo4008(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot);
		void __fastcall writeData();
		void __fastcall readData();

};
//---------------------------------------------------------------------------
extern PACKAGE TformAo4008 *formAo4008;
//---------------------------------------------------------------------------
#endif
