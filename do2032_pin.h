//---------------------------------------------------------------------------

#ifndef do2032_pinH
#define do2032_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include "DBUILib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TformDo2032 : public TForm
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
		TDBoolean	*dBool[DO32_NUM];
		AnsiString title;
public:		// User declarations
        //UINT DO32;
        //bool isWrite;
		int state; //0:read, 1:write
        __fastcall TformDo2032(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int slot, AnsiString title);
		void __fastcall writeData();
		void __fastcall readData();
};
//---------------------------------------------------------------------------
extern PACKAGE TformDo2032 *formDo2032;
//---------------------------------------------------------------------------
#endif
