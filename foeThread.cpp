//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "foeThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall FoeThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall FoeThread::FoeThread(bool CreateSuspended, TformMain *main)
    : TThread(CreateSuspended)
{
    this->main = main;
}
//---------------------------------------------------------------------------
void __fastcall FoeThread::Execute()
{
    //---- Place thread code here ----
    while(true)
    {
        main->foeCycle();
        Sleep(COE_UPDATE_INTERVAL);
    }
}
//---------------------------------------------------------------------------
