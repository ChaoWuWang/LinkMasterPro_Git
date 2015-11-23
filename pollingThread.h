//---------------------------------------------------------------------------

#ifndef pollingThreadH
#define pollingThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "mainForm.h"

//---------------------------------------------------------------------------
class PollingThread : public TThread
{            
private:
		TformMain *main;
protected:
        void __fastcall Execute();
public:
        __fastcall PollingThread(bool CreateSuspended, TformMain *main);
		bool isSuspend;
		bool isEndThread;
};
//---------------------------------------------------------------------------
#endif
