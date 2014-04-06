//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
#define GHOST_HOTKEY 0x0000 //0x0000 through 0xBFFF are valid

#include <vector>
using namespace std;

enum jkgesture{GEST_UP = 1, GEST_DOWN};

struct GhostInfo {

};

class TfrmGhostVeil : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *PopupMenu1;
	TTrayIcon *TrayIcon1;
	TMenuItem *Unveilall1;
	TMenuItem *Quit1;
	TMenuItem *GhostVeilactivewindow1;
	TBitBtn *btnGhost;
	TTrackBar *trackBarTransparency;
	TLabel *Label1;
	TCheckBox *cbxMouseOverUnghost;
	TComboBox *ComboBox1;
	TTimer *tmrMouseOver;
	void __fastcall btnGhostClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cbxMouseOverUnghostClick(TObject *Sender);
	void __fastcall tmrMouseOverTimer(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall WMHotKey( TWMHotKey &Message );
	TList* lstGhost;
    std::vector<tagPOINT> vct_points;
	bool hotkeyRegistered;

	DWORD lastStyle;
	DWORD exStyle, newExStyle;
	HWND__* hwnd;

public:		// User declarations
	HWND__* GetWindowAtMouse();
	void GhostIt(HWND__* hwnd);
	void UnGhostIt(HWND__* hwnd);
	void UnGhostAll();
	void GhostToggle(HWND__* hwnd);
	bool IsGhosted(HWND__* hwnd);
	void IncreaseTransparency(HWND__* hwnd, int howmuch = 10);
	void EnumerateWindows();
	std::vector<HWND__*> windows;

	bool GestureIsInSamePlace(vector<tagPOINT>& points);
	bool GestureIsUpOrDown(vector<tagPOINT>& points, jkgesture upordown);

	__fastcall TfrmGhostVeil(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER( WM_HOTKEY, TWMHotKey, WMHotKey )
	END_MESSAGE_MAP( TForm )
};

BOOL CALLBACK EnumWindowsProc(HANDLE hWnd, LPARAM lparam);

//---------------------------------------------------------------------------
extern PACKAGE TfrmGhostVeil *frmGhostVeil;
//---------------------------------------------------------------------------
#endif
