//---------------------------------------------------------------------------

#ifndef mo7002_pinH
#define mo7002_pinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "API_Class.h"
#include "DBUILib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
#include <Grids.hpp>

//#define MO_IO_FILTER_NUM	10
#define MO_IO_POLARITY_NUM	11
#define MO_CONFIG_NUM		6
#define MO_PARA				17
#define INT_SIZE			32

typedef struct _COL_ROW_CONVERT_BIT
{
	int stringGrid_col;
	int stringGrid_row;
	int bit;
	int bitMap;
} COL_ROW_CONVERT_BIT;


//---------------------------------------------------------------------------
class TformMo7002 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *gbAxisFirst;
    TGroupBox *gbAxisSecond;
    TButton *btnSet;
    TLabel *Label0;
    TEdit *edEncoder0;
    TLabel *Label1;
    TEdit *edEncoder1;
    TDBoolean *dbAxis0;
    TDBoolean *dbAxis1;
    TStringGrid *sgAxisConfig;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall __OnClick(TObject *Sender);
    void __fastcall sgAxisConfigMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
	API_Class *api;
	USHORT SlaveNo;
	int moNum;
	int Slot;
	//int Set;
	//String AxisParaValueName[2][MO_PARA];
	UINT AxisParamValue[2][MO_PARA];
	short AxisIoPolarity[INT_SIZE], AxisConfig[INT_SIZE];
	COL_ROW_CONVERT_BIT *p_axis_io_polarity, *p_axis_config;
	int axis_io_polarity_size, axis_config_size;
	TDBoolean	*stAxis0[MO_NUM], *stAxis1[MO_NUM];//, *diBool1[DI32_NUM], *diBool2[DI32_NUM];
	//TComboBox	*cbAxis0Polarity[MO_IO_POLARITY_NUM], *cbAxis0Config[MO_CONFIG_NUM], *cbAxis1Polarity[MO_IO_POLARITY_NUM], *cbAxis1Config[MO_CONFIG_NUM];
	TComboBox	*cbAxisPara[MO_PARA];
	
	void __fastcall ShowAxisComboBox(int ACol, int ARow);
	void __fastcall HideAxisComboBox();
	void __fastcall AxisStringGridComboSelect(TObject *Sender);
	void __fastcall ColRowIdxToBitIdx(COL_ROW_CONVERT_BIT *p_convert, int size, int col, int row, int *bit);
	void __fastcall BitIdxToColRowIdx(COL_ROW_CONVERT_BIT *p_convert, int size, int bit, int *col, int *row);


public:		// User declarations
    __fastcall TformMo7002(TComponent* Owner, API_Class *api, USHORT slaveNo, int adlinkNo, int moNum, int slot);
	int state;
	void __fastcall writeData();
	void __fastcall readData();
	void __fastcall readByAxis(UINT posAddr, TEdit *edPos, UINT statusAddr, TDBoolean** dBool);
};
//---------------------------------------------------------------------------
extern PACKAGE TformMo7002 *formMo7002;
//---------------------------------------------------------------------------
#endif
