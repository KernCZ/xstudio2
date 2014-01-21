
// MainFrm.cpp : implementation of the MainWnd class
//

#include "stdafx.h"
#include "Application.h"

#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// <summary>User interface</summary>
NAMESPACE_BEGIN(GUI)


   // MainWnd

   IMPLEMENT_DYNAMIC(MainWnd, CMDIFrameWndEx)

   const int  iMaxUserToolbars = 10;
   const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
   const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

   BEGIN_MESSAGE_MAP(MainWnd, CMDIFrameWndEx)
	   ON_WM_CREATE()
	   ON_COMMAND(ID_WINDOW_MANAGER, &MainWnd::OnWindowManager)
	   ON_COMMAND(ID_VIEW_CUSTOMIZE, &MainWnd::OnViewCustomize)
	   ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &MainWnd::OnToolbarCreateNew)
	   ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &MainWnd::OnApplicationLook)
	   ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &MainWnd::OnUpdateApplicationLook)
	   ON_WM_SETTINGCHANGE()
   END_MESSAGE_MAP()

   static UINT indicators[] =
   {
	   ID_SEPARATOR,           // status line indicator
	   ID_INDICATOR_CAPS,
	   ID_INDICATOR_NUM,
	   ID_INDICATOR_SCRL,
   };

   // MainWnd construction/destruction

   MainWnd::MainWnd()
   {
	   // TODO: add member initialization code here
	   theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
   }

   MainWnd::~MainWnd()
   {
   }

   int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
   {
	   if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		   return -1;

      try
      {
         // Document Tabs
	      CMDITabInfo mdiTabParams;
	      mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	      mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	      mdiTabParams.m_bTabIcons = TRUE;    // set to TRUE to enable document icons on MDI taba
	      mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	      mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	      EnableMDITabbedGroups(TRUE, mdiTabParams);


         // MainMenu:
	      if (!m_wndMenuBar.Create(this))
            throw Win32Exception(HERE, L"Unable to create MainWnd menu");

	      m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	      // prevent the menu bar from taking the focus on activation
	      CMFCPopupMenu::SetForceMenuFocus(FALSE);


         // ToolBar:
	      if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
            throw Win32Exception(HERE, L"Unable to create MainWnd toolbar");
	      
	      m_wndToolBar.SetWindowText(GuiString(IDS_TOOLBAR_STANDARD).c_str());
	      m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, GuiString(IDS_TOOLBAR_CUSTOMIZE).c_str());

	      // Allow user-defined toolbars operations:
	      InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);


         // StatusBar:
	      if (!m_wndStatusBar.Create(this))
            throw Win32Exception(HERE, L"Unable to create MainWnd statusBar");
	      m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	      // Make Toolbar/MenuBar dockable
	      m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	      m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	      EnableDocking(CBRS_ALIGN_ANY);
	      DockPane(&m_wndMenuBar);
	      DockPane(&m_wndToolBar);


	      // enable Visual Studio 2005 style docking + AutoHide
	      CDockingManager::SetDockingMode(DT_SMART);
	      EnableAutoHidePanes(CBRS_ALIGN_ANY);

	      // Load menu item image (not placed on any standard toolbars):
	      CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, IDR_MENU_IMAGES);


	      // Project Window:
	      if (!m_wndProject.Create(GuiString(IDS_FILE_VIEW).c_str(), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROJECTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
            throw Win32Exception(HERE, L"Unable to create Project window");
	   
	      // Output Window:
	      if (!m_wndOutput.Create(GuiString(IDS_OUTPUT_WND).c_str(), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	         throw Win32Exception(HERE, L"Unable to create Output window");

	      // Properties Window:
	      if (!m_wndProperties.Create(GuiString(IDS_PROPERTIES_WND).c_str(), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	         throw Win32Exception(HERE, L"Unable to create Properties window");

         // Game Data Window:
	      if (!m_wndGameData.Create(GuiString(L"Game Data").c_str(), this, CRect(0, 0, 200, 200), TRUE, 420, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	         throw Win32Exception(HERE, L"Unable to create GameData window");

         // Set icons
         m_wndProject.SetIcon(theApp.LoadIconW(IDI_PROJECT_WND, ::GetSystemMetrics(SM_CXSMICON)), FALSE);
         m_wndOutput.SetIcon(theApp.LoadIconW(IDI_OUTPUT_WND, ::GetSystemMetrics(SM_CXSMICON)), FALSE);
         m_wndProperties.SetIcon(theApp.LoadIconW(IDI_PROPERTIES_WND, ::GetSystemMetrics(SM_CXSMICON)), FALSE);
         m_wndGameData.SetIcon(theApp.LoadIconW(IDI_PROPERTIES_WND, ::GetSystemMetrics(SM_CXSMICON)), FALSE);
	      UpdateMDITabbedBarsIcons();

         // Dock each window
	      m_wndProject.EnableDocking(CBRS_ALIGN_ANY);
         DockPane(&m_wndProject);
	   
	      m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	      DockPane(&m_wndOutput);

	      m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	      DockPane(&m_wndProperties);

         m_wndGameData.EnableDocking(CBRS_ALIGN_ANY);
         DockPane(&m_wndGameData);



	      // set the visual manager and style based on persisted value
	      OnApplicationLook(theApp.m_nAppLook);

	      // Enable enhanced windows management dialog
	      EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	      // Enable toolbar and docking window menu replacement
	      EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, GuiString(IDS_TOOLBAR_CUSTOMIZE).c_str(), ID_VIEW_TOOLBAR);

	      // enable quick (Alt+drag) toolbar customization
	      CMFCToolBar::EnableQuickCustomization();

	      if (CMFCToolBar::GetUserImages() == NULL)
	      {
		      // load user-defined toolbar images
		      if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		      {
			      CMFCToolBar::SetUserImages(&m_UserImages);
		      }
	      }

	      // enable menu personalization (most-recently used commands)
	      // TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	      CList<UINT, UINT> lstBasicCommands;

	      lstBasicCommands.AddTail(ID_FILE_NEW);
	      lstBasicCommands.AddTail(ID_FILE_OPEN);
	      lstBasicCommands.AddTail(ID_FILE_SAVE);
	      lstBasicCommands.AddTail(ID_FILE_PRINT);
	      lstBasicCommands.AddTail(ID_APP_EXIT);
	      lstBasicCommands.AddTail(ID_EDIT_CUT);
	      lstBasicCommands.AddTail(ID_EDIT_PASTE);
	      lstBasicCommands.AddTail(ID_EDIT_UNDO);
	      lstBasicCommands.AddTail(ID_APP_ABOUT);
	      lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	      lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	      lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	      lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	      lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	      lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	      lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	      CMFCToolBar::SetBasicCommands(lstBasicCommands);

	      // Switch the order of document name and application name on the window title bar. This
	      // improves the usability of the taskbar because the document name is visible with the thumbnail.
	      ModifyStyle(0, FWS_PREFIXTITLE);

	      return 0;
      }
      catch (ExceptionBase& e)
      {
         Console << e << ENDL;
         return -1;
      }
   }

   BOOL MainWnd::PreCreateWindow(CREATESTRUCT& cs)
   {
	   if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		   return FALSE;
	   // TODO: Modify the Window class or styles here by modifying
	   //  the CREATESTRUCT cs

	   return TRUE;
   }



   // MainWnd diagnostics

   #ifdef _DEBUG
   void MainWnd::AssertValid() const
   {
	   CMDIFrameWndEx::AssertValid();
   }

   void MainWnd::Dump(CDumpContext& dc) const
   {
	   CMDIFrameWndEx::Dump(dc);
   }
   #endif //_DEBUG


   // MainWnd message handlers

   void MainWnd::OnWindowManager()
   {
	   ShowWindowsDialog();
   }

   void MainWnd::OnViewCustomize()
   {
	   CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	   pDlgCust->EnableUserDefinedToolbars();
	   pDlgCust->Create();
   }

   LRESULT MainWnd::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
   {
	   LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	   if (lres == 0)
	   {
		   return 0;
	   }

	   CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	   ASSERT_VALID(pUserToolbar);

	   BOOL bNameValid;
	   CString strCustomize;
	   bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	   ASSERT(bNameValid);

	   pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	   return lres;
   }

   void MainWnd::OnApplicationLook(UINT id)
   {
	   CWaitCursor wait;

	   theApp.m_nAppLook = id;

	   switch (theApp.m_nAppLook)
	   {
	   case ID_VIEW_APPLOOK_WIN_2000:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		   break;

	   case ID_VIEW_APPLOOK_OFF_XP:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		   break;

	   case ID_VIEW_APPLOOK_WIN_XP:
		   CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		   break;

	   case ID_VIEW_APPLOOK_OFF_2003:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		   CDockingManager::SetDockingMode(DT_SMART);
		   break;

	   case ID_VIEW_APPLOOK_VS_2005:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		   CDockingManager::SetDockingMode(DT_SMART);
		   break;

	   case ID_VIEW_APPLOOK_VS_2008:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		   CDockingManager::SetDockingMode(DT_SMART);
		   break;

	   case ID_VIEW_APPLOOK_WINDOWS_7:
		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		   CDockingManager::SetDockingMode(DT_SMART);
		   break;

	   default:
		   switch (theApp.m_nAppLook)
		   {
		   case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			   break;

		   case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			   break;

		   case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			   break;

		   case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			   break;
		   }

		   CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		   CDockingManager::SetDockingMode(DT_SMART);
	   }

	   m_wndOutput.UpdateFonts();
	   RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	   theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
   }

   void MainWnd::OnUpdateApplicationLook(CCmdUI* pCmdUI)
   {
	   pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
   }


   BOOL MainWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
   {
	   // base class does the real work

	   if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	   {
		   return FALSE;
	   }


	   // enable customization button for all user toolbars
	   BOOL bNameValid;
	   CString strCustomize;
	   bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	   ASSERT(bNameValid);

	   for (int i = 0; i < iMaxUserToolbars; i ++)
	   {
		   CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		   if (pUserToolbar != NULL)
		   {
			   pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		   }
	   }

	   return TRUE;
   }


   void MainWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
   {
	   CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	   m_wndOutput.UpdateFonts();
   }

NAMESPACE_END(GUI)

