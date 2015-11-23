//---------------------------------------------------------------------------

#ifndef foeThreadH
#define foeThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "mainForm.h"
//---------------------------------------------------------------------------
class FoeThread : public TThread
{            
private:
    TformMain *main;
protected:
    void __fastcall Execute();
public:
    __fastcall FoeThread(bool CreateSuspended, TformMain *main);
    bool isEndThread;
};
//---------------------------------------------------------------------------
#endif
