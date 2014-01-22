
// MFC Test 1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Application.h"
#include "MainWnd.h"

#include "ChildFrm.h"
#include "ScriptDocument.h"
#include "ScriptView.h"
#include "LanguageView.h"
#include "LanguageDocument.h"

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// --------------------------------- GLOBAL --------------------------------

Application theApp;

// --------------------------------- TYPES ---------------------------------

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

// --------------------------------- APP WIZARD ---------------------------------
  
BEGIN_MESSAGE_MAP(Application, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &Application::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// -------------------------------- CONSTRUCTION --------------------------------

Application::Application()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFC Test 1.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

// ------------------------------- STATIC METHODS -------------------------------

// ------------------------------- PUBLIC METHODS -------------------------------

BOOL Application::InitInstance()
{
	
   //VLDEnable();
   VLDDisable();
   //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
   AfxOleInit();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDS_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
   AfxInitRichEdit2();
	AfxInitRichEdit5();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_SCRIPTVIEW,
		RUNTIME_CLASS(GUI::ScriptDocument),
		RUNTIME_CLASS(GUI::CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(ScriptView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

   pDocTemplate = new CMultiDocTemplate(IDR_LANGUAGEVIEW,
		RUNTIME_CLASS(GUI::LanguageDocument),
		RUNTIME_CLASS(GUI::CChildFrame),
		RUNTIME_CLASS(GUI::LanguageView));
   if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// create main MDI Frame window
	MainWnd* pMainFrame = new MainWnd;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int Application::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinAppEx::ExitInstance();
}


// App command to run the dialog
void Application::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Application customization load/save methods

void Application::PreLoadState()
{
	GetContextMenuManager()->AddMenu(GuiString(IDS_EDIT_MENU).c_str(), IDM_EDIT_POPUP);

	GetContextMenuManager()->AddMenu(GuiString(IDS_EXPLORER).c_str(), IDM_PROJECT_POPUP);
}

void Application::LoadCustomState()
{
}

void Application::SaveCustomState()
{
}

// ------------------------------ PROTECTED METHODS -----------------------------

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// ------------------------------- PRIVATE METHODS ------------------------------


