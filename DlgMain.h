#pragma once
#define _WIN32_WINNT 0x0500
#include "myapidialog.h"
#include ".\combownd.h"
#include ".\CProgressBar.h"

class CDlgMain : public CMyAPIDialog
{
protected: static CDlgMain* This;

public:
  // GUI handlers
  wchar_t* m_sCaption;                 // dialog caption
  CComboWnd* m_cbbits;                  // bits combox
  BOOL m_bCreateAll;                    // check box
  CProgressBar* m_pcPBar;               // progress bar

  wchar_t m_sSourceFn[MAX_PATH];       // source file name
  DWORD m_dwSourceSize;                 // source file size
  LONG m_udmx, m_udmy;                  // dimension x & y
  
  static const UINT s_uiBitSetLength = 6;
  static const char s_achBitSet[s_uiBitSetLength];// array of bits by selection [32..1]
  static HANDLE s_hDispatcherThread;

  void EnableMainControls(BOOL _bEnable);
  void EnableCreationControls(BOOL _bEnable);
  void ShowDimension(DWORD _dwSize);
  bool CheckForFreeSpace(wchar_t *_sLocation, DWORD _dwRequired);
  int ReverseFind(wchar_t *_str, wchar_t _key);

  static const DWORD s_dwFileReadBufferSize = 4096; // bytes
  static enum enumBITS {BITS_32=0, BITS_24, BITS_16, BITS_8, BITS_4, BITS_1};

  static BOOL CALLBACK Proc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam);
  // manage threads that will create the rainbytes
  static DWORD WINAPI ThreadProc_CreateRainbyteDispatcher(LPVOID lpParameter);
  // create single rainbyte
  static DWORD WINAPI ThreadProc_CreateRainbyte(LPVOID lpParameter);
  //{{ BEGIN MESSAGE MAP
  BOOL OnInitDialog(HWND _hWnd);        // on WM_INITDIALOG
  BOOL OnOpenFile();
  BOOL OnCreate();
  BOOL OnChangeCreateAll();
  //}} END MESSAGE MAP
  void Run();

  CDlgMain(HWND _hPWnd, HINSTANCE _hInstance, UINT _IDD, wchar_t *_sCaption);
  ~CDlgMain(void);
};

// params passed to the ThreadProc_CreateRainbyte
struct SThreadParams_CreateRainbyte
{
  wchar_t* sFileName;   // source file
  DWORD dwSourceSize;
  WORD wBits;           // BMP bits
  LONG lWidth;
  LONG lHeight;
  HWND hWnd;            // main dialog window handle
  wchar_t* sCaption;
  CProgressBar* pPBar;
};