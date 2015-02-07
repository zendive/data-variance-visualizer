#pragma once
#include "myapiwnd.h"

// Usage: Make combobox in dialog resource editor
// create object [new] in OnInitDialog message and fill values programaticaly;

class CComboWnd :
   public CMyAPIWnd
{
private:
   int m_ret;
   UINT m_indx;
public:
   UINT Get();
   int Add(TCHAR *_p);
   int Set(UINT _indx);
public:
   CComboWnd(CMyAPIWnd *_wnd, UINT _IDD);
   ~CComboWnd(void);
};
