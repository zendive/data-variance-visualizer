//
// Alexandr Block 4-BYTE (C)Copyright 2005
// Basic dialog window base class
// "MyAPIWnd.h"
//

#pragma once
#define     _INC_OLE
#include    <windows.h>
#undef      _INC_OLE

// The base class for almost all elements witch have own window, I hope
class CMyAPIWnd
{
public:
   HWND m_hWnd;                              // this dialog handler
   const HWND m_hParentWnd;                  // parent dialog window handler
   const HINSTANCE m_hInstance;              // program instance handler
   const UINT MY_IDD;                        // this dialog resource identifier

public:
   // DEFAULT CONSTRUCTOR:
   // creation uninitialized window
   CMyAPIWnd()
      : m_hParentWnd( NULL )
      , m_hInstance( NULL )
      , MY_IDD( NULL )
   {
      m_hWnd = NULL;
   }

   // Creation of main window
   // `_hPWnd` - parent window handler
   // `_hInstance` - application instance handler
   // `_IDD` - window resource descriptor
   CMyAPIWnd(HINSTANCE _hInstance, UINT _IDD)
      : m_hParentWnd( NULL )
      , m_hInstance( _hInstance )
      , MY_IDD( _IDD )
   {
      m_hWnd = NULL;
   }

   // Creation chaild window
   // `_hPWnd` - parent window handler
   // `_hInstance` - application instance handler
   // `_IDD` - window resource descriptor
   CMyAPIWnd(HWND _hPWnd, HINSTANCE _hInstance, UINT _IDD)
      : m_hParentWnd( _hPWnd )
      , m_hInstance( _hInstance )
      , MY_IDD( _IDD )
   {
      m_hWnd = NULL;
   }

   // COPY CONSTRUCTOR:
   CMyAPIWnd(const CMyAPIWnd &obj)
      : m_hParentWnd( obj.m_hParentWnd )
      , m_hInstance( obj.m_hInstance )
      , MY_IDD ( obj.MY_IDD )
   {
      m_hWnd = obj.m_hWnd;
   }

   // DESTRUCTOR:
   virtual ~CMyAPIWnd(){ ; }
};

//? EOF
