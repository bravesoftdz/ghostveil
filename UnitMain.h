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
	void __fastcall btnGhostClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	DWORD lastStyle;
	DWORD exStyle, newExStyle;
	HWND__* hwnd;
public:		// User declarations
	HWND__* GetWindowAtMouse();
	void GhostIt(HWND__* hwnd);
	void UnGhostIt(HWND__* hwnd);
	bool IsGhosted(HWND__* hwnd);

	__fastcall TfrmGhostVeil(TComponent* Owner);

	//hotkey stuff
	void __fastcall OnHotKey(TWMHotKey &hotkey);
	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_HOTKEY, TWMHotKey, OnHotKey);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGhostVeil *frmGhostVeil;
//---------------------------------------------------------------------------
#endif
