#include    <tchar.h>
#include ".\combownd.h"
//#using <mscorlib.dll>

CComboWnd::CComboWnd(CMyAPIWnd *_wnd, UINT _IDD)
   :CMyAPIWnd(/*parentwnd*/_wnd->m_hWnd, _wnd->m_hInstance, _IDD)
{
   m_ret = 0;
   m_indx = 0;
}

CComboWnd::~CComboWnd(void)
{
}

// CB_, CBS_

UINT CComboWnd::Get()
{
   m_indx = (UINT)SendDlgItemMessage(m_hParentWnd, MY_IDD, CB_GETCURSEL, 0, 0);
   ///m_ret = SendDlgItemMessage(m_hParentWnd, MY_IDD, CB_GETITEMDATA, m_indx, 0);
   return m_indx;
}

int CComboWnd::Add(TCHAR *_p)
{
   m_ret = (int)SendDlgItemMessage(m_hParentWnd, MY_IDD, CB_ADDSTRING, 0, (LPARAM)_p);
   return m_ret;
}

int CComboWnd::Set(UINT _indx)
{
   m_indx = _indx;
   m_ret = (int)SendDlgItemMessage(m_hParentWnd, MY_IDD, CB_SETCURSEL, m_indx, 0);
   return m_ret;
}
