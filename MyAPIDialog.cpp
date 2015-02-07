//
// Alexandr Block 4-BYTE (C)Copyright 2005
// Basic dialog window base class
// "MyAPIDialog.cpp"
//

#pragma once
#include    "MyAPIDialog.h"

#undef CLASSNAME
#undef THIS
#define CLASSNAME CMyAPIDialog
#define THIS ((CLASSNAME*)CLASSNAME::This)

// init static pointer on this object
CMyAPIDialog* CMyAPIDialog::This = NULL;

// CONSTRUCTOR
CMyAPIDialog::CMyAPIDialog(HWND _hPWnd, HINSTANCE _hInstance, UINT _IDD)
:CMyAPIWnd(_hPWnd, _hInstance, _IDD)
{
   CMyAPIDialog::This = this;
}

// DESTRUCTOR
CMyAPIDialog::~CMyAPIDialog()
{
   End();

   // becouse of nature of using this pointer is flows inside static functions,
   // there is need to set this pointer to null after object destruction,
   // to prevent from accesing to already destroyed objeect funcs.
   this->This = NULL;
}

// run dialog box with given Proc function by function pointer
//virtual:
void CMyAPIDialog::RunDialog(BOOL (CALLBACK *_pfProc)(HWND, UINT, WPARAM, LPARAM))
{
   for (INT_PTR i = 3, j; i; i--)
   {
      j = DialogBox(m_hInstance, MAKEINTRESOURCE(MY_IDD), m_hParentWnd, _pfProc);
      if (0 == j || -1 == j)
      {// try to launch window agane if dosn't maked at first time
         continue;
      }
      return;
   }
}

// run this dialog
//public:
void CMyAPIDialog::Run()
{
   RunDialog(Proc);
}

// end this dialog
//public:
void CMyAPIDialog::End()
{
   EndDialog(m_hWnd, TRUE);
}

// report error message from error-owner with conversion of his error-code
// to formated message, if conversion fails GetLastEror() will be shown
// public static:
void CMyAPIDialog::ErrorReport(wchar_t* _szErrOwner, LONG _lErrCode)
{
   wchar_t szCaption[] = L" Error report:";
   const int bufSize = MAX_PATH;
   wchar_t *lpMsgBuf = NULL;
   
   try
   {
      lpMsgBuf = (wchar_t *)calloc(bufSize + 1, sizeof(wchar_t));
      if (!lpMsgBuf) { throw "Insufficient heap memory to show detailed error."; }

      if ( !FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL
         , _lErrCode
         , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
         , lpMsgBuf, bufSize, NULL) )
      {
         // if failed to find error description show last error
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL
         , GetLastError()
         , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
         , lpMsgBuf, bufSize, NULL);
      }

      MessageBox(((THIS)? THIS->m_hWnd : NULL)
         , /*msg*/      lpMsgBuf
         , /*caption*/  _szErrOwner
         , MB_OK | MB_ICONERROR);
   }
   catch (wchar_t *_szErrText)
   {
      ::MessageBox(((THIS)? THIS->m_hWnd : NULL), _szErrText, szCaption, MB_OK);
   }

   free(lpMsgBuf);
}

// report error message with formated owner error-code, if conversion fails
// GetLastError() will be shown
// public static:
void CMyAPIDialog::ErrorReport(LONG _lErrCode)
{
   wchar_t szCaption[] = L" Error report:";
   const int bufSize = MAX_PATH;
   wchar_t *lpMsgBuf = NULL;
   
   try
   {
      lpMsgBuf = (wchar_t *)calloc(bufSize + 1, sizeof(wchar_t));
      if (!lpMsgBuf) { throw "Insufficient heap memory to show detailed error."; }

      if ( !FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL
         , _lErrCode
         , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
         , lpMsgBuf, bufSize, NULL) )
      {
         // if failed to find error description show last error
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL
         , GetLastError()
         , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
         , lpMsgBuf, bufSize, NULL);
      }

      MessageBox(((THIS)? THIS->m_hWnd : NULL)
         , /*msg*/      lpMsgBuf
         , /*caption*/  szCaption
         , MB_OK | MB_ICONERROR);
   }
   catch (wchar_t *_szErrText)
   {
      ::MessageBox(((THIS)? THIS->m_hWnd : NULL), _szErrText, szCaption, MB_OK);
   }

   free(lpMsgBuf);
}

// report error from error-owner by retrieving error-code from
// the system GetLastEror()
// public static:
void CMyAPIDialog::ErrorReport(wchar_t* _szErrOwner)
{
   wchar_t szCaption[] = L" Error report:";
   const int bufSize = MAX_PATH;
   wchar_t *lpMsgBuf = NULL;
   
   try
   {
      lpMsgBuf = (wchar_t *)calloc(bufSize + 1, sizeof(wchar_t));
      if (!lpMsgBuf) { throw "Insufficient heap memory to show detailed error."; }

      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL
         , GetLastError()
         , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
         , lpMsgBuf, bufSize, NULL);

      MessageBox(((THIS)? THIS->m_hWnd : NULL)
         , /*msg*/      lpMsgBuf
         , /*caption*/  _szErrOwner
         , MB_OK | MB_ICONERROR);
   }
   catch (wchar_t *_szErrText)
   {
     ::MessageBox(((THIS)? THIS->m_hWnd : NULL), _szErrText, szCaption, MB_OK | MB_ICONHAND);
   }

   free(lpMsgBuf);
}


// set caption to the dialog window
BOOL CMyAPIDialog::Caption(LPCTSTR _caption)
{
   return SetWindowText(m_hWnd, _caption);
}

// alias of SendDlgItemMessage
BOOL CMyAPIDialog::SendItemMessage(int nIDDlgItem, UINT Msg
                                   , WPARAM wParam, LPARAM lParam)
{
   return (BOOL) ::SendDlgItemMessage(m_hWnd, nIDDlgItem, Msg, wParam, lParam);
}

// enable or disable chaild control of the current dialog
BOOL CMyAPIDialog::EnableItem(int _iIDDlgItem, BOOL _bEnable)
{
   return ::EnableWindow(GetDlgItem(m_hWnd, _iIDDlgItem), ((_bEnable)? TRUE : FALSE));
}

// Get UINT of the specified by id chaild control of the current dialog
UINT CMyAPIDialog::GetItemUINT(int _iIDDlgItem)
{
   return ::GetDlgItemInt(m_hWnd, _iIDDlgItem, NULL, false);
}

// Set UINT for specified id of the chaild control of the current dialog
BOOL CMyAPIDialog::SetItemUINT(int _iIDDlgItem, UINT _uiNewValue)
{
   return ::SetDlgItemInt(m_hWnd, _iIDDlgItem, _uiNewValue, false);
}

// Set Text for specified by ID chaild control of the current dialog
BOOL CMyAPIDialog::SetItemTEXT(int _iIDDlgItem, LPCTSTR _szNewText)
{
   return SetDlgItemText(m_hWnd, _iIDDlgItem, _szNewText);
}

// alias of SendMessage
// if hWnd==HWND_BROADCAST message will be sended to all except child window
LRESULT CMyAPIDialog::SendMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
   return ::SendMessage(hWnd, Msg, wParam, lParam);
}

// use in `Proc` on WM_INITDIALOG message
//private:
BOOL CMyAPIDialog::OnInitDialog(HWND _hWnd)
{
   m_hWnd = _hWnd;
   //#for further review/SetWindowText(m_hWnd, "");
   return TRUE;
}

// use in `Proc` on IDOK message, Ok button
//private:
BOOL CMyAPIDialog::OnCmdOk()
{
   return TRUE;
}

// use in `Proc` on IDCANCEL message, Cancel button
//private:
BOOL CMyAPIDialog::OnCmdCancel()
{
   End();
   return TRUE;
}

// use in `Proc` on default message
//private:
BOOL CMyAPIDialog::OnDefaultMsg(UINT /*_uMsg*/, WPARAM /*_wParam*/, LPARAM /*_lParam*/)
{
   return FALSE;
}

// dialog message controller procedure
//static:
BOOL CALLBACK CMyAPIDialog::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   if (!THIS) { return FALSE; }
   
   switch (uMsg)
   {
      case WM_INITDIALOG:              { return THIS->OnInitDialog(hWnd); }
      case WM_COMMAND:
      {
         switch (GET_WM_COMMAND_ID(wParam, lParam))
         {
         case IDOK:                    { return THIS->OnCmdOk(); }
         case IDCANCEL:                { return THIS->OnCmdCancel(); }
         }
      }
      default:                         { return THIS->OnDefaultMsg(uMsg, wParam, lParam); }
   }
}

//? EOF
