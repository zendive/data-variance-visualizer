// Alexandr Block @ `4 BYTE`
// GoldUni+ Class Library  in  WorkTimeController
// "CProgressBar.h"

#pragma once
#define     _INC_OLE
#include    <windows.h>
#undef      _INC_OLE
#include    <commctrl.h>
#include    <assert.h>
#include   "MyAPIDialog.h"

// class CProgressBar: progress bar dialog control
// warning: some of the interface functions dosn't tested yet
class CProgressBar
{
// MEMBER DECLARATION:
private:
   CMyAPIDialog* m_dlg;       // owner dialog
   HWND m_h;                  // progress-bar window handler
   UINT m_u100;               // 100 percent
   UINT m_uCurPos;            // current position in progress
   LONG m_uRange;             // MAKELONG(_nMinRange, _nMaxRange)
   int  m_nIncrement;         // delta increment
   int  m_nStep;              // step increment
   COLORREF m_clrBk;          // background color
   COLORREF m_clrBar;         // indicator bar color
   signed int m_iLowLim;
   signed int m_iHighLim;

// CONSTRUCTOR DESTRUCTOR:
public:
      // create object in OnInitDialog() call
   CProgressBar(CMyAPIDialog* _dlg);
   ~CProgressBar();

// FUNCTION MEMBER DECLARATION:
public:
      // create progress bar on specified coordinates with given style
   bool Create(UINT _u100, UINT _tx, UINT _ty, UINT _rx, UINT _ry, UINT _uStyle=NULL);
      // show or hide the window
   void Show(BOOL);
      // Sets the minimum and maximum values for a progress bar and redraws the bar to reflect the new range
   void SetRange(UINT _nMinRange, UINT _nMaxRange);
      // Sets the range of a progress bar control to a 32-bit value
   void SetRange32(int _iLowLim, int _iHighLim);
      // Specifies the step increment for a progress bar
   void SetStep(int);
      // Sets the current position for a progress bar and redraws the bar to reflect the new position
   void SetPos(UINT);
      // Sets the background color in the progress bar
   void SetBkColor(COLORREF);
      // Sets the color of the progress indicator bar in the progress bar control
   void SetBarColor(COLORREF);
      // Retrieves the current position of the progress bar
   UINT GetPos();
      // Advances the current position for a progress bar by the step increment and redraws the bar to reflect the new position
   void StepIt();
      //Advances the current position of a progress bar by a specified increment and redraws the bar to reflect the new position
   void DeltaPos(int);
      // update progress bar to current range and position; independ of `_bExp`
   void Update(UINT _u100, UINT _uPos, bool _bExp=true);
      // reset progress bar position
   void Reset();
      // destroy progress bar window
   void Destroy();
      // Retrieves information about the current high and low limits of a given progress bar control
   ///GetRange();//PBM_GETRANGE
};

//? EOF
