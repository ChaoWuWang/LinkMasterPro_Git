//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pollingThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall PollingThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall PollingThread::PollingThread(bool CreateSuspended, TformMain *main)
        : TThread(CreateSuspended)
{
	this->main = main;
	this->Priority = tpIdle; 
}
//---------------------------------------------------------------------------
void __fastcall PollingThread::Execute()
{
    //---- Place thread code here ----
    while(!isEndThread)
    {
    	main->pollingCycle();

        if(this->isSuspend)
        {
            this->Suspend();
            //SuspendThread(this);
        }
		//Sleep(COE_UPDATE_INTERVAL);
    }
}
//---------------------------------------------------------------------------
