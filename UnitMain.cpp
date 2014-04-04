//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmGhostVeil *frmGhostVeil;
//---------------------------------------------------------------------------
__fastcall TfrmGhostVeil::TfrmGhostVeil(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
HWND__* TfrmGhostVeil::GetWindowAtMouse(){
   tagPOINT point;
   GetCursorPos(&point);
   return WindowFromPoint(point);
}

bool TfrmGhostVeil::IsGhosted(HWND__* hwnd){
   exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
   if (exStyle & (WS_EX_LAYERED | WS_EX_TRANSPARENT)) {
	  return true;
   }else{
       return false;
   }
}

void TfrmGhostVeil::GhostIt(HWND__* hwnd){
  exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  newExStyle = exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT;
  ::SetWindowLong(hwnd, GWL_EXSTYLE, newExStyle);
  ::SetLayeredWindowAttributes(hwnd, 0, trackBarTransparency->Position, LWA_ALPHA);
  ::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

void TfrmGhostVeil::UnGhostIt(HWND__* hwnd){
  exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  newExStyle = exStyle & (!(WS_EX_LAYERED | WS_EX_TRANSPARENT));
  ::SetWindowLong(hwnd, GWL_EXSTYLE, newExStyle);
  ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
  ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

void __fastcall TfrmGhostVeil::btnGhostClick(TObject *Sender)
{
   hwnd = GetForegroundWindow();//GetAncestor(GetWindowAtMouse(), GA_ROOT);
   Caption = IntToStr((int)hwnd);
   if (!IsGhosted(hwnd)) {
		GhostIt(hwnd);
		Caption = "Ghosted "+Caption;
   } else{
	   UnGhostIt(hwnd);
	   Caption = "Unghosted "+Caption;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmGhostVeil::FormCreate(TObject *Sender)
{
	hotkeyRegistered = RegisterHotKey( Handle,
		GHOST_HOTKEY,
		MOD_ALT | MOD_WIN,
		0x47
	);

	if( ! hotkeyRegistered )
	{
		ShowMessage( "Unable to assign Win+Alt+G as hotkey." );
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmGhostVeil::FormDestroy(TObject *Sender)
{
	if( hotkeyRegistered )
	{
		UnregisterHotKey( Handle, GHOST_HOTKEY );
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmGhostVeil::WMHotKey(TWMHotKey& Message)
{
	TForm::Dispatch( &Message );
	if( Message.HotKey == GHOST_HOTKEY )
	{
		   hwnd = GetForegroundWindow();
		   if (!IsGhosted(hwnd)) {
				GhostIt(hwnd);
		   } else{
			   UnGhostIt(hwnd);
		   }
	}
}
