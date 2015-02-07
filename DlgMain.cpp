#include    <tchar.h>
#include    <io.h>
#include    <process.h>       // _beginthread(...), _endthread()
#include ".\dlgmain.h"
#include    <math.h>
#include "resource.h"

#undef  THIS
#define THIS ((CDlgMain*)CDlgMain::This)

CDlgMain* CDlgMain::This = NULL;
const char CDlgMain::s_achBitSet[CDlgMain::s_uiBitSetLength] = {32, 24, 16, 8, 4, 1};
HANDLE CDlgMain::s_hDispatcherThread = NULL;

CDlgMain::CDlgMain(HWND _hPWnd, HINSTANCE _hInstance, UINT _IDD, wchar_t* _sCaption)
        :CMyAPIDialog(_hPWnd, _hInstance, _IDD)
{
  This = this;
  m_sCaption = _sCaption;
  m_bCreateAll = TRUE;
}

CDlgMain::~CDlgMain(void)
{
  if (NULL != m_cbbits) { delete m_cbbits; m_cbbits = NULL; }
  if (NULL != m_pcPBar) { delete m_pcPBar; m_pcPBar = NULL; }
  if (NULL != s_hDispatcherThread)
  {
    ::CloseHandle(s_hDispatcherThread);
    s_hDispatcherThread = NULL;
  }
}

void CDlgMain::Run()
{
  RunDialog(Proc);
}

BOOL CALLBACK CDlgMain::Proc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
  switch (_uMsg)
  {
  case WM_INITDIALOG:             return THIS->OnInitDialog(_hWnd);
  case WM_COMMAND:
    switch (GET_WM_COMMAND_ID(_wParam, _lParam))
    {
    case IDC_btnOpenFile:           return THIS->OnOpenFile();
    case IDC_btnCreate:             return THIS->OnCreate();
    case IDC_chkCreateAll:          return THIS->OnChangeCreateAll();
    case IDCANCEL:                  return THIS->OnCmdCancel();
    }
  default:                        return THIS->OnDefaultMsg(_uMsg, _wParam, _lParam);
  }
}

BOOL CDlgMain::OnInitDialog(HWND _hWnd)
{
  CMyAPIDialog::OnInitDialog(_hWnd);

  m_cbbits = new CComboWnd(this, IDC_BITS);
  m_pcPBar = new CProgressBar(this);

  if (NULL != m_cbbits)
  {
    m_cbbits->Add(_T("32/4.29e9"));
    m_cbbits->Add(_T("24/16.8e6"));
    m_cbbits->Add(_T("16/65.5e3"));
    m_cbbits->Add(_T("8/256"));
    m_cbbits->Add(_T("4/16"));
    m_cbbits->Add(_T("1/2"));
    
    // set default selection
    m_cbbits->Set(CDlgMain::BITS_24);
  }

  if (NULL != m_pcPBar)
  {
    m_pcPBar->Create(CDlgMain::s_uiBitSetLength, 12, 63, 330, 14, PBS_SMOOTH | WS_CHILD | WS_VISIBLE);
    m_pcPBar->SetBkColor(RGB(0, 0, 0));
    m_pcPBar->SetBarColor(RGB(0, 255, 0));
    m_pcPBar->Reset();
    m_pcPBar->Show(FALSE);
  }
  
  this->Caption(m_sCaption);
  this->EnableCreationControls(FALSE);
  CheckDlgButton(m_hWnd, IDC_chkCreateAll, (UINT) m_bCreateAll);
  
  return TRUE;
}

//CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES) NULL,
//           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);

void CDlgMain::EnableCreationControls(BOOL _bEnable)
{
  EnableItem(IDC_btnCreate, _bEnable);
  EnableItem(IDC_BITS, ((_bEnable == FALSE)? FALSE : !m_bCreateAll));
  EnableItem(IDC_chkCreateAll, _bEnable);
}

void CDlgMain::EnableMainControls(BOOL _bEnable)
{
  this->EnableCreationControls(_bEnable);
  EnableItem(IDC_btnOpenFile, _bEnable);
}

BOOL CDlgMain::OnChangeCreateAll()
{
  m_bCreateAll = !m_bCreateAll;
  EnableItem(IDC_BITS, !m_bCreateAll);

  return TRUE;
}

void CDlgMain::ShowDimension(DWORD _dwSize)
{
  if (0.0 == _dwSize)
  {
    SetItemTEXT(IDC_txtDimension, _T(""));
  }
  else
  {
    //m_udmx = (LONG) sqrt(((double) _dwSize)/2.0);
    m_udmx = (LONG) sqrt(
        ((double) _dwSize)
          *
        (CDlgMain::s_achBitSet[m_cbbits->Get()]/32.0)
      );
    m_udmy = m_udmx;

    wchar_t str[MAX_PATH];
    wsprintf(str, _T("Dim: %upx"), m_udmx);
    SetItemTEXT(IDC_txtDimension, str);
  }
}

BOOL CDlgMain::OnOpenFile()
{
  OPENFILENAME ofn;                 // common dialog box structure
  SecureZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFile = m_sSourceFn;
  // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
  // use the contents of szFile to initialize itself.
  ofn.lpstrFile[0] = 0x00;
  ofn.nMaxFile = sizeof(m_sSourceFn);
  ofn.lpstrFilter =
    _T("All\0*.*\0Exe, Dll\0*.exe;*.dll\0Text\0*.txt\0Vtdpf\0*.vt?\0")
    _T("Archive\0*.zip;*.gz;*.tar;*.tgz;*.rar;*.arj;*.7z;*.cab\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER 
    | OFN_FORCESHOWHIDDEN | OFN_DONTADDTORECENT | OFN_READONLY | OFN_NONETWORKBUTTON;

  // Display the Open dialog box
  BOOL bSelected = GetOpenFileName(&ofn);
  BOOL bReadable = FALSE;
  
  if (bSelected)
  {
    // test shared readability
    HANDLE hFile = CreateFile(m_sSourceFn, GENERIC_READ, FILE_SHARE_READ
      , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

    bReadable = (hFile != INVALID_HANDLE_VALUE);
    if (bReadable)
    {
      // get and test source file size
      DWORD dwHightSize = 0;
      m_dwSourceSize = ::GetFileSize(hFile, &dwHightSize);

      if (dwHightSize != 0)
      {
        ::MessageBox(m_hWnd, _T("The file size is to big to continue operation.")
          _T("\nKnown BMP format can't handle files bigger than 2^31 byte.")
          , m_sCaption, MB_OK | MB_ICONASTERISK);

        m_dwSourceSize = 0;
        bSelected = FALSE;
      }

      CloseHandle(hFile); hFile = NULL;
    }
    else
    {
      MessageBox(m_hWnd, _T("Can't access specified file!!!"), m_sCaption, MB_OK | MB_ICONERROR);
    }
  }
  
  // reflect GUI
  this->EnableCreationControls(bSelected && bReadable);
  this->ShowDimension(m_dwSourceSize);
  this->SetItemTEXT(IDC_txtSourceFN, ((bSelected && bReadable)? m_sSourceFn : _T("{choose target file}")));
  
  return TRUE;
}

// Launch dispatcher thread to process multiple Rainbyte threads
BOOL CDlgMain::OnCreate()
{
  LPTHREAD_START_ROUTINE lpStartAddress = &CDlgMain::ThreadProc_CreateRainbyteDispatcher;
  DWORD dwThreadId = NULL;

  if (NULL != s_hDispatcherThread)
  {
    ::CloseHandle(s_hDispatcherThread);
    s_hDispatcherThread = NULL;
  }

  s_hDispatcherThread = CreateThread(NULL, 0, lpStartAddress, (LPVOID)this, NULL, &dwThreadId);
  if (NULL == s_hDispatcherThread || INVALID_HANDLE_VALUE == s_hDispatcherThread)
  {
    CMyAPIDialog::ErrorReport(_T("Rainbyte - Failed to create dispatcher thread"));
  }
  
  return TRUE;
}

DWORD WINAPI CDlgMain::ThreadProc_CreateRainbyteDispatcher(LPVOID lpParameter)
{
  CDlgMain* pDlg = (CDlgMain *)lpParameter;

  HANDLE hThread = NULL;
  LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
  SIZE_T dwStackSize = 0;
  LPTHREAD_START_ROUTINE lpStartAddress = &CDlgMain::ThreadProc_CreateRainbyte;
  DWORD dwCreationFlags = NULL;
  DWORD dwThreadId = NULL;
  SThreadParams_CreateRainbyte *pRainbyteParam
      = (SThreadParams_CreateRainbyte *)calloc(1, sizeof(SThreadParams_CreateRainbyte));
  if (NULL == pRainbyteParam)
  {
    ::ExitThread(-1);
  }
  
  pDlg->EnableMainControls(FALSE);

  // init progress bar update parameters
  pDlg->m_pcPBar->SetRange32(0, ((pDlg->m_bCreateAll)? pDlg->m_dwSourceSize * CDlgMain::s_uiBitSetLength : pDlg->m_dwSourceSize));
  pDlg->m_pcPBar->Reset();
  pDlg->m_pcPBar->Show(TRUE);

  pRainbyteParam->sFileName = pDlg->m_sSourceFn;
  pRainbyteParam->dwSourceSize = pDlg->m_dwSourceSize;
  pRainbyteParam->hWnd = pDlg->m_hWnd;
  pRainbyteParam->sCaption = pDlg->m_sCaption;
  pRainbyteParam->pPBar = pDlg->m_pcPBar;

  DWORD dwExitCode = 0;
  UINT i = ((pDlg->m_bCreateAll)? 0 : pDlg->m_cbbits->Get());
  for (; ((pDlg->m_bCreateAll)? (i < CDlgMain::s_uiBitSetLength) : (i == pDlg->m_cbbits->Get())); i++)
  {
    if (!pDlg->CheckForFreeSpace(pDlg->m_sSourceFn, pDlg->m_dwSourceSize + 54))
    {
      continue;
    }

    pRainbyteParam->wBits = CDlgMain::s_achBitSet[i % CDlgMain::s_uiBitSetLength];
    pRainbyteParam->lWidth = pDlg->m_udmx;
    pRainbyteParam->lHeight = pDlg->m_udmy;
    
    hThread = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress,
      (LPVOID)pRainbyteParam, dwCreationFlags, &dwThreadId);

    if (NULL == hThread || INVALID_HANDLE_VALUE == hThread)
    {
      free(pRainbyteParam);
      pDlg->EnableMainControls(TRUE);
      pDlg->m_pcPBar->Show(FALSE);
      pDlg->m_pcPBar->Reset();
      
      CMyAPIDialog::ErrorReport(_T("Rainbyte - Failed to create thread"));

      ::ExitThread(-1);
    }

    ::SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);
    
    do
    {
      ::MsgWaitForMultipleObjects(1, &hThread, TRUE, 50, QS_ALLEVENTS | QS_ALLINPUT | QS_ALLPOSTMESSAGE);
      ::GetExitCodeThread(hThread, &dwExitCode);
    }
    while (dwExitCode == STILL_ACTIVE);
    
    if (0 != dwExitCode)
    {
      CMyAPIDialog::ErrorReport(_T("Rainbyte - Thread ended with error"));
    }
    
    CloseHandle(hThread);
  }

  pDlg->m_pcPBar->Show(FALSE);
  pDlg->m_pcPBar->Reset();
  free(pRainbyteParam);
  pDlg->EnableMainControls(TRUE);
  ::ExitThread(0);
}

DWORD WINAPI CDlgMain::ThreadProc_CreateRainbyte(LPVOID lpParameter)
{
  SThreadParams_CreateRainbyte *pParam = (SThreadParams_CreateRainbyte *)lpParameter;

  BITMAPINFO bi;                        // bitmap info-header 
  SecureZeroMemory(&bi, sizeof(bi));
  bi.bmiHeader.biWidth = pParam->lWidth;
  bi.bmiHeader.biHeight = (-pParam->lHeight);// negative value makes top-down display
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  //((bi.bmiHeader.biWidth * pParam->wBits +31) & ~31) /8 * bi.bmiHeader.biHeight;
  bi.bmiHeader.biSizeImage = 0;
  bi.bmiHeader.biBitCount = pParam->wBits;
  bi.bmiHeader.biClrUsed = 0;
  bi.bmiHeader.biXPelsPerMeter = 0;
  bi.bmiHeader.biYPelsPerMeter = 0;
  bi.bmiHeader.biPlanes = 1;    //reserved
  bi.bmiHeader.biClrImportant = 0;
  bi.bmiHeader.biCompression = BI_RGB;//BI_RGB BI_RLE4 BI_RLE8 BI_BITFIELDS BI_JPEG BI_PNG
  //------------

  BITMAPFILEHEADER hdr;                 // bitmap file-header 
  SecureZeroMemory(&hdr, sizeof(hdr));
  hdr.bfSize = (DWORD) sizeof(BITMAPFILEHEADER) + bi.bmiHeader.biSize + pParam->dwSourceSize;
  hdr.bfType = 0x4d42;                  // "BM" format mask
  hdr.bfReserved1 = hdr.bfReserved2 = 0;
  hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + bi.bmiHeader.biSize;
  //------------
  
  HANDLE hfs = CreateFile(pParam->sFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
  if (hfs == INVALID_HANDLE_VALUE)
  {
    MessageBox(pParam->hWnd, _T("Can't access specified file!"), pParam->sCaption, MB_OK);
    ExitThread(-1);
  }
  
  TCHAR sDestinationFn[MAX_PATH];
  wsprintf((LPTSTR)sDestinationFn, _T("%s.%.02d.Bmp"), pParam->sFileName, pParam->wBits);

  HANDLE hfd = CreateFile(sDestinationFn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
  if (hfd == INVALID_HANDLE_VALUE)
  {
    TCHAR sText[MAX_PATH + 100];
    wsprintf(sText, _T("Can't create output file: {%s}"), sDestinationFn);
    MessageBox(pParam->hWnd, sText, pParam->sCaption, MB_OK);
    
    if (NULL != hfs) { CloseHandle(hfs); hfs = NULL; }
    ExitThread(-1);
  }
  
  char buf[CDlgMain::s_dwFileReadBufferSize];
  SecureZeroMemory(&buf, CDlgMain::s_dwFileReadBufferSize);
  DWORD rdn, wrn;
  BOOL bGoodReading = FALSE;
  BOOL bGoodWriting = FALSE;

  WriteFile(hfd, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), &wrn,  NULL);
  WriteFile(hfd, (LPVOID)&bi
    , sizeof(BITMAPINFOHEADER) + (bi.bmiHeader.biClrUsed * sizeof(RGBQUAD)), &wrn, NULL);
  
  do
  {
    bGoodReading = ::ReadFile(hfs, buf, CDlgMain::s_dwFileReadBufferSize, &rdn, NULL);
    if (bGoodReading)
    {
      bGoodWriting = ::WriteFile(hfd, buf, rdn, &wrn, NULL);
    }
    pParam->pPBar->DeltaPos(rdn);
  }
  while (rdn == CDlgMain::s_dwFileReadBufferSize && bGoodReading && bGoodWriting);
  
  DWORD dwErrorCode = ::GetLastError();
  if (dwErrorCode != ERROR_SUCCESS && dwErrorCode != ERROR_ALREADY_EXISTS)
  {
    CMyAPIDialog::ErrorReport(pParam->sCaption);
  }
  
  if (NULL != hfs) { CloseHandle(hfs); hfs = NULL; }
  if (NULL != hfd) { CloseHandle(hfd); hfd = NULL; }
  
  ExitThread(0);
}

// Find character in string, looking from the end. ret -1 if not found
int CDlgMain::ReverseFind(wchar_t *_str, wchar_t _key)
{
  if (NULL == _str) return -1;
  
  int i = 0;
  while (_str[i] != 0) { i++; }
  
  while ((--i) != -1)
  {
    if (_str[i] == _key)
    {
      break;
    }
  }

  return i;
}

// ret true if OK
bool CDlgMain::CheckForFreeSpace(wchar_t *_sLocation, DWORD _dwRequired)
{
  int iPos = this->ReverseFind(_sLocation, _T('\\'));
  const int MSG_LEN = 3 * MAX_PATH;
  wchar_t sMsg[MSG_LEN];

  if (-1 == iPos)
  {
    wsprintf(sMsg, _T("Sorry, unable to determine folder of the selected file:")
      _T("\n- \"%s\"\n\nDo you wan't to continue?"), _sLocation);
    int iRet = ::MessageBox(m_hWnd, sMsg, m_sCaption, MB_ICONQUESTION | MB_YESNO);

    return iRet == IDYES;
  }

  wchar_t sFolder[MAX_PATH + 3];
  memcpy(sFolder, _sLocation, sizeof(wchar_t)*(iPos + 1));
  sFolder[iPos + 1]= 0x00;
  //DEBUG:
  //wprintf(_T("CDlgMain::CheckForFreeSpace()> sFolder = {%s}"), sFolder);
  
  ULARGE_INTEGER i64FreeBytesToCaller;
  ULARGE_INTEGER i64TotalBytes;
  ULARGE_INTEGER i64FreeBytes;
  BOOL bRet = FALSE;

  while (true)
  {
    bRet = ::GetDiskFreeSpaceExW(sFolder, &i64FreeBytesToCaller, &i64TotalBytes, &i64FreeBytes);
    if (FALSE == bRet)
    {
      wsprintf(sMsg, _T("Sorry, unable to determine free space on folder:")
        _T("\n- \"%s\"\n\nDo you wan't to continue?"), sFolder);
      int iRet = ::MessageBox(m_hWnd, sMsg, m_sCaption, MB_ICONQUESTION | MB_YESNO);

      return iRet == IDYES;
    }
    
    if (i64FreeBytesToCaller.QuadPart < (ULONGLONG) _dwRequired)
    { // ."));
      wsprintf(sMsg, _T("Not enough space on volume to save the result of:")
        _T("\n- \"%s\"\n\n----------------------------")
        _T("\nNeed more about = %.03I64u Bytes"),
        _sLocation, ((ULONGLONG)_dwRequired - i64FreeBytesToCaller.QuadPart));
        
      int iRet = ::MessageBox(m_hWnd, sMsg, m_sCaption, MB_RETRYCANCEL | MB_ICONWARNING);

      if (iRet == IDRETRY)
      {
        continue;
      }
      else
      {
        return false;
      }
    }
    else
    { 
      return true;
    }
  }
}
