// Alexandr Block
// WorkTimeController
// "CProgressBar.cpp"

#pragma once
#include    "CProgressBar.h"

// CONSTRUCTOR:
CProgressBar::CProgressBar(CMyAPIDialog* _dlg)
{
   m_dlg = _dlg;
   m_h = NULL;
   m_uCurPos = 0;
}

// DESTRUCTOR:
CProgressBar::~CProgressBar()
{
   Destroy();
}

// create progress bar on specified coordinates with given style
bool CProgressBar::Create(
   UINT _u100,        // 100%
   UINT _xp, UINT _yp,// left-top x, y position
   UINT _xd,UINT _yd, // x, y dimension
   UINT _uStyle)      // boolean set of styles PBS_*, WS_*
{
   m_u100 = _u100;
                              // create progress bar
   m_h = CreateWindow(PROGRESS_CLASS, NULL, _uStyle,
      _xp, _yp, _xd, _yd,
      m_dlg->m_hWnd, NULL, m_dlg->m_hInstance, NULL);
   assert(m_h);

   SetRange(0, m_u100);
   SetStep(1);
   return true;               // all right
}

void CProgressBar::Show(BOOL _bShow)
{
  if (m_h)
  {
    ::ShowWindow(m_h, ((_bShow)? SW_SHOW : SW_HIDE));
  }
}

// update progress bar to current range and position; independ of `_bExp`
void CProgressBar::Update(UINT _u100, UINT _uPos, bool _bExp/*=true*/)
{
   if (!m_h)
   {
      return;
   }
   if (_bExp)
   {
      m_u100 = _u100;
      SetRange(0, m_u100);
      SetPos(_uPos);
   }
   else
   {
      Reset();
   }
}

// destroy progress bar window
void CProgressBar::Destroy()
{
   if (m_h)
   {
      DestroyWindow(m_h);
      m_h = NULL;
   }
}

// Sets the current position for a progress bar and redraws the bar to reflect the new position
void CProgressBar::SetPos(UINT _uPos)
{
   if (!m_h)
   {
      return;
   }
   m_uCurPos = _uPos;
   SendMessage(m_h, PBM_SETPOS,
      (WPARAM) m_uCurPos,
      (LPARAM) 0); // not used, must be zero
}

// Retrieves the current position of the progress bar
UINT CProgressBar::GetPos()
{
   if (!m_h)
   {
      return 0;
   }
   m_uCurPos = (UINT)SendMessage(m_h, PBM_GETPOS,
      (WPARAM) 0, (LPARAM) 0);  // not used, must be zero
   return m_uCurPos;
}

// set range of progress dialog bar
// Minimum range value. By default, the minimum value is zero
// Maximum range value. By default, the maximum value is 100.
void CProgressBar::SetRange(UINT _nMinRange, UINT _nMaxRange)
{
   if (!m_h)
   {
      return;
   }
   m_uRange = MAKELONG(_nMinRange, _nMaxRange);
   SendMessage(m_h, PBM_SETRANGE,
      (WPARAM) 0, // not used, must be zero
      (LPARAM) m_uRange);
}

// Sets the range of a progress bar control to a 32-bit value
void CProgressBar::SetRange32(int _iLowLim, int _iHighLim)
{
   if (!m_h)
   {
      return;
   }
   m_iLowLim = _iLowLim;
   m_iHighLim = _iHighLim;
   SendMessage(m_h, PBM_SETRANGE32,
      (WPARAM) m_iLowLim,
      (LPARAM) m_iHighLim);
}

// Advances the current position of a progress bar by a specified
// increment and redraws the bar to reflect the new position
void CProgressBar::DeltaPos(int _nIncrement)
{
   if (!m_h)
   {
      return;
   }
   m_nIncrement = _nIncrement;
   SendMessage(m_h, PBM_DELTAPOS,
      (WPARAM)(int) m_nIncrement,
      (LPARAM) 0);  // not used, must be zero
}

// Specifies the step increment for a progress bar
void CProgressBar::SetStep(int _StepInc)
{
   if (!m_h)
   {
      return;
   }
   m_nStep = _StepInc;
   SendMessage(m_h, PBM_SETSTEP,
      (WPARAM) m_nStep,
      (LPARAM) 0); //not used, must be zero
}

// Advances the current position for a progress bar by the step
// increment and redraws the bar to reflect the new position
void CProgressBar::StepIt()
{
   if (!m_h)
   {
      return;
   }

   m_uCurPos += m_nStep;
   SendMessage(m_h, PBM_STEPIT,
      (WPARAM)0, (LPARAM)0);   // not used, must be zero
}

// Sets the color of the progress indicator bar in the progress bar control
void CProgressBar::SetBarColor(COLORREF _clrBar)
{
   if (!m_h)
   {
      return;
   }  

   m_clrBar = _clrBar;
   SendMessage(m_h, PBM_SETBARCOLOR,
      (WPARAM) 0, // not used must be zero
      (LPARAM) m_clrBar);
}

// Sets the background color in the progress bar
void CProgressBar::SetBkColor(COLORREF _clrBk)
{
   if (!m_h)
   {
      return;
   }
   m_clrBk = _clrBk;
   SendMessage(m_h, PBM_SETBKCOLOR,
      (WPARAM) 0, // not used must be zero
      (LPARAM) m_clrBk);
}

// reset progress bar position
void CProgressBar::Reset()
{
   SetPos(0);
}

//? EOF
