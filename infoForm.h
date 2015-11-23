//---------------------------------------------------------------------------

#ifndef infoFormH
#define infoFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>


#define LINKMASTERPRO_VER       "1.0.15.11131"
//---------------------------------------------------------------------------
class TformInfo : public TForm
{
__published:	// IDE-managed Components
        TButton *btnOk;
        TLabel *Label1;
    TLabel *Label2;
        TImage *imgLogo;
    TGroupBox *gbSupport;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;

        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        //API_Class *api;
public:		// User declarations
        __fastcall TformInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformInfo *formInfo;
//---------------------------------------------------------------------------
#endif
