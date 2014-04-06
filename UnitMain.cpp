//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmGhostVeil *frmGhostVeil;


BOOL CALLBACK EnumWindowsProc(HANDLE hwnd, LPARAM lparam){
	frmGhostVeil->windows.push_back((HWND__*)hwnd);
}

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

void TfrmGhostVeil::IncreaseTransparency(HWND__* hwnd, int howmuch){
	trackBarTransparency->Position += howmuch;
	::SetLayeredWindowAttributes(hwnd, 0, trackBarTransparency->Position, LWA_ALPHA);
}

void TfrmGhostVeil::GhostIt(HWND__* hwnd){
  exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  newExStyle = exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT;
  ::SetWindowLong(hwnd, GWL_EXSTYLE, newExStyle);
  ::SetLayeredWindowAttributes(hwnd, 0, trackBarTransparency->Position, LWA_ALPHA);
  ::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
  lstGhost->Add((void*)hwnd);
}

void TfrmGhostVeil::UnGhostIt(HWND__* hwnd){
  exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  newExStyle = exStyle & (!(WS_EX_LAYERED | WS_EX_TRANSPARENT));
  ::SetWindowLong(hwnd, GWL_EXSTYLE, newExStyle);
  ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
  ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
  lstGhost->Delete(lstGhost->IndexOf((void*) hwnd));
}

void TfrmGhostVeil::UnGhostAll(){
  for (int i = lstGhost->Count-1; i >= 0; i--) {
	  UnGhostIt((HWND__*)lstGhost->Items[i]);
  }
}

void __fastcall TfrmGhostVeil::btnGhostClick(TObject *Sender)
{
   hwnd = GetForegroundWindow();//GetAncestor(GetWindowAtMouse(), GA_ROOT);
   Caption = IntToStr((int)hwnd);
   if (!IsGhosted(hwnd)) {
		GhostIt(hwnd);
		Caption = "Ghosted "+Caption;
		lstGhost->Add(hwnd);
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

	lstGhost = new TList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmGhostVeil::FormDestroy(TObject *Sender)
{
    UnGhostAll();
	if( hotkeyRegistered )
	{
		UnregisterHotKey( Handle, GHOST_HOTKEY );
	}
	delete	lstGhost;
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

void TfrmGhostVeil::GhostToggle(HWND__* hwnd){
	   if (!IsGhosted(hwnd)) {
			GhostIt(hwnd);
	   } else{
		   UnGhostIt(hwnd);
	   }
}

void __fastcall TfrmGhostVeil::cbxMouseOverUnghostClick(TObject *Sender)
{
  if (cbxMouseOverUnghost->Checked) {
	 tmrMouseOver->Enabled = true;
  }  else{
	 tmrMouseOver->Enabled = false;
  }
}
//---------------------------------------------------------------------------
bool TfrmGhostVeil::GestureIsInSamePlace(vector<tagPOINT>& points){

   for (int i = 1; i < points.size(); i++) {
	   if (points[i-1].x != points[i].x &&
		   points[i-1].y != points[i].y){
			return false;
	   }
   }
   return true;
}

bool TfrmGhostVeil::GestureIsUpOrDown(vector<tagPOINT>& points, jkgesture upordown){
	 if (points.size() <= 1){
		 return false;
	 }

	 int start = upordown == GEST_UP ? 0 : (int)points.size()-1;
	 int finish = upordown == GEST_UP ? (int)points.size()-1 : 0;
	 int prev = start;
	 int badcount = 0;
	 for (int i = start; i <= finish;) {

		 if (i == start){

			continue;
		 }
		 if (points[i].y > points[prev].y){
			badcount++;
		 }
		 prev = i;
	 }
	 Caption = "BadCount " + IntToStr(badcount);
	 float percent_ok = (badcount*1.0 / points.size());
	 return percent_ok > .75;
}

void __fastcall TfrmGhostVeil::tmrMouseOverTimer(TObject *Sender)
{
	//do nothing if special keyboard key isn't pressed
	int fixTranspInstead;
	if (GetAsyncKeyState(VK_SHIFT)){
		fixTranspInstead = 1;
	}else if (GetAsyncKeyState(VK_MENU)) {
		fixTranspInstead = -1;
	}else if (GetAsyncKeyState(VK_CONTROL)) {
		fixTranspInstead = 0;
	}else{
	  //remove all points not gathered while using hotkeys
	   vct_points.erase(vct_points.begin(),vct_points.end());
	   return;
	}


	tagPOINT point;
	GetCursorPos(&point);
	vct_points.push_back(point);

	if (vct_points.size() < 30 - abs(fixTranspInstead*5)) {
	   return; //do not let it work unless there are at least x mouse points
	}

	if (GestureIsInSamePlace(vct_points)) {
	   return; //mouse has to be moving
	}

	if (lstGhost->Count == 0) {
		return;
	}




	for (int i = lstGhost->Count-1; i >= 0; i--) {
		tagRECT rect;
		HWND__* hwnd = (HWND__*)lstGhost->Items[i];
		GetWindowRect(hwnd, &rect);
		if (PtInRect(rect, point)) {
			vct_points.erase(vct_points.begin(),vct_points.end());
			if (fixTranspInstead) {
				if (IsGhosted(hwnd)){
				   IncreaseTransparency(hwnd, fixTranspInstead*30);
				}
			}else{
				GhostToggle(hwnd);
			}
			break;
		}
	}


	EnumerateWindows();
	tagRECT rect;
	Caption = IntToStr((int)windows.size())+ " windows.";
	for (int i = 0; i < windows.size(); i++) {
		GetWindowRect(windows[i], &rect);
		if (PtInRect(rect, point)) {
		    Caption = "Mouse in Window " + IntToStr((int)windows[i]);
			vct_points.erase(vct_points.begin(),vct_points.end());
			GhostToggle(windows[i]);
			break;
		}
	}
}
//---------------------------------------------------------------------------
void TfrmGhostVeil::EnumerateWindows(){
	windows.erase(windows.begin(), windows.end());
	EnumWindows((WNDENUMPROC )EnumWindowsProc, NULL);
}


//---------------------------------------------------------------------------

