// Author: Alexandr Block 2005
// Project: Rainbyte

#define     _INC_OLE
#include    <windows.h>
#include    <tchar.h>
#undef      _INC_OLE
#include "resource.h"
#include "DlgMain.h"

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE /*_hPrevInstance*/
                  , LPSTR /*_lpCmdLine*/, int /*_nCmdShow*/)
{
  CDlgMain *pDlg = NULL;

  try
  {
    pDlg = new CDlgMain(NULL, _hInstance, IDD_DRV, _T("Rainbyte v0.2"));
    
    if (NULL != pDlg)
    {
      pDlg->Run();
    }
  }
  // catch any unhandled events
  catch (...)
  {
    CMyAPIDialog::ErrorReport(_T("Rainbyte - Unhandled error"));
  }

  if (NULL != pDlg) { delete pDlg; pDlg = NULL; }
  
  return FALSE;
}

//? EOF
