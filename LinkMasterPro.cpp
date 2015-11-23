//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("mainForm.cpp", formMain);
USEFORM("slaveForm.cpp", formSlave);
USEFORM("infoForm.cpp", formInfo);
USEFORM("ai3216_pin.cpp", formAi3216);
USEFORM("ai3032_pin.cpp", formAi3032);
USEFORM("ao4008_pin.cpp", formAo4008);
USEFORM("di1132_pin.cpp", formDi1132);
USEFORM("do2032_pin.cpp", formDo2032);
USEFORM("ro2308_pin.cpp", formRo2308);
USEFORM("rtd3504_pin.cpp", formRtd3504);
USEFORM("mo7002_pin.cpp", formMo7002);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TformMain), &formMain);
         Application->CreateForm(__classid(TformInfo), &formInfo);
         Application->CreateForm(__classid(TformAi3216), &formAi3216);
         Application->CreateForm(__classid(TformAi3032), &formAi3032);
         Application->CreateForm(__classid(TformAo4008), &formAo4008);
         Application->CreateForm(__classid(TformDi1132), &formDi1132);
         Application->CreateForm(__classid(TformDo2032), &formDo2032);
         Application->CreateForm(__classid(TformRo2308), &formRo2308);
         Application->CreateForm(__classid(TformRtd3504), &formRtd3504);
         Application->CreateForm(__classid(TformMo7002), &formMo7002);
         Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
