//
// Alexandr Block 4-BYTE (C)Copyright 2005
// Basic dialog window base class
// "MyAPIDialog.h"
//

#pragma once
#define     _INC_OLE
#include    <windows.h>
#include    <windowsx.h>
#undef      _INC_OLE
#include   <assert.h>
#include    "MyAPIWnd.h"

//
// class CMyAPIDialog: contain some basic elements of a dialog-box window
// A good beginning is a half of battle.

class CMyAPIDialog: public CMyAPIWnd
{
protected:
   // Value of This pointer's inside base and chaild classes is equal,
   // difference in the types, therefore need conversion to
   // appropriete type of class (havy code) or overloading of the static
   // `This` pointer inside chaild class to make able call new func.s
   // without it :) good luck.

   // make available access to members within static functions
   // with cooperation of `THIS` macro
   static CMyAPIDialog *This;

public:
   CMyAPIDialog(HWND _hPWnd, HINSTANCE _hInstance, UINT _IDD);
   virtual ~CMyAPIDialog();

protected:
   // run dialog box with given Proc function by function pointer
   virtual void RunDialog(BOOL (CALLBACK *_pfProc)(HWND, UINT, WPARAM, LPARAM));
   // window message controller procedure
   static BOOL CALLBACK Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
   //{{ BEGIN MESSAGE MAP // Used in `Proc`
   virtual BOOL OnInitDialog(HWND _hWnd);
   virtual BOOL OnCmdOk();
   virtual BOOL OnCmdCancel();
   virtual BOOL OnDefaultMsg(UINT _uMsg, WPARAM _wParam, LPARAM _lParam);
   //}} END MESSAGE MAP

   virtual void Run();     // run this dialog
   virtual void End();     // end this dialog

public:
   // report error message from error-owner with conversion of his error-code
   // to formated message, if conversion fails GetLastEror() will be shown
   static void ErrorReport(wchar_t* _szErrOwner, LONG _lErrCode);

   // report error message with formated owner error-code, if conversion fails
   // GetLastError() will be shown
   static void ErrorReport(LONG _lErrCode);

   // report error from error-owner by retrieving error-code from
   // the system GetLastEror()
   static void ErrorReport(wchar_t* _szErrOwner);

protected:
   // set caption to the dialog window
   BOOL Caption(LPCTSTR _caption);
   
   // alias of SendDlgItemMessage
   BOOL SendItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam);
   
   // enable or disable chaild control of the current dialog
   BOOL EnableItem(int _iIDDlgItem, BOOL _bEnable);

   // alias of SendMessage
   // if hWnd==HWND_BROADCAST message will be sended to all except child window
   LRESULT SendMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

   // Get UINT of the specified by id chaild control of the current dialog
   UINT GetItemUINT(int _iIDDlgItem);
   
   // Set UINT for specified id of the chaild control of the current dialog
   BOOL SetItemUINT(int _iIDDlgItem, UINT _uiNewValue);

   // Set Text for specified by ID chaild control of the current dialog
   BOOL SetItemTEXT(int _iIDDlgItem, LPCTSTR _szNewText);
};

//? EOF
