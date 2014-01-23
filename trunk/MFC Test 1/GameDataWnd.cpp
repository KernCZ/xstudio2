#include "stdafx.h"
#include "GameDataWnd.h"
#include <strsafe.h>
#include "ListView.h"

/// <summary>User interface</summary>
NAMESPACE_BEGIN(GUI)

   // --------------------------------- APP WIZARD ---------------------------------
  
   BEGIN_MESSAGE_MAP(CGameDataWnd, CDockablePane)
	   ON_WM_CREATE()
	   ON_WM_SIZE()
	   ON_WM_CONTEXTMENU()
	   ON_WM_PAINT()
	   ON_WM_SETFOCUS()
      ON_EN_CHANGE(2, &CGameDataWnd::OnSearchTermChanged)
   END_MESSAGE_MAP()

   // -------------------------------- CONSTRUCTION --------------------------------

   CGameDataWnd::CGameDataWnd() : fnAppStateChanged(theApp.StateChanged.Register(this, &CGameDataWnd::onAppStateChanged))
   {
   }


   CGameDataWnd::~CGameDataWnd()
   {
   }

   // ------------------------------- STATIC METHODS -------------------------------

   // ------------------------------- PUBLIC METHODS -------------------------------

   // ------------------------------ PROTECTED METHODS -----------------------------

   void CGameDataWnd::onAppStateChanged(AppState s)
   {
      UpdateContent();
   }

   void CGameDataWnd::OnContextMenu(CWnd* pWnd, CPoint point)
   {
	   //CTreeCtrl* pWndTree = (CTreeCtrl*) &TreeView;
	   //ASSERT_VALID(pWndTree);

	   //if (pWnd != pWndTree)
	   //{
		  // CDockablePane::OnContextMenu(pWnd, point);
		  // return;
	   //}

	   //if (point != CPoint(-1, -1))
	   //{
		  // // Select clicked item:
		  // CPoint ptTree = point;
		  // pWndTree->ScreenToClient(&ptTree);

		  // UINT flags = 0;
		  // HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		  // if (hTreeItem != NULL)
		  // {
			 //  pWndTree->SelectItem(hTreeItem);
		  // }
	   //}

	   //pWndTree->SetFocus();
	   //theApp.GetContextMenuManager()->ShowPopupMenu(IDM_PROJECT_POPUP, point.x, point.y, this, TRUE);
   }


   int CGameDataWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
   {
      try
      {
         CRect rectDummy;
	      rectDummy.SetRectEmpty();

         // Base 
	      if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		      throw Win32Exception(HERE, L"Unable to create dockable pane");

	      // ListView
	      if (!ListView.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT |  LVS_ALIGNTOP | LVS_NOCOLUMNHEADER, rectDummy, this, 1))
            throw Win32Exception(HERE, L"Unable to create game data window listview");

         // Insert columns
         ListView.InsertColumn(0, L"Item", LVCFMT_LEFT, 200, 0);
         //ListView.InsertColumn(1, L"Group", LVCFMT_LEFT, 100, 1);
         ListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);
         ListView.SetFont(&afxGlobalData.fontRegular);
         ListView.EnableGroupView(TRUE);
         
         // ListView ImageList:
	      //Images.Create(IDB_GAMEDATA_ICONS, 16, 0, RGB(255, 0, 255));
         CBitmap* icons = theApp.LoadBitmapW(IDB_GAMEDATA_ICONS, 0, 0, LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
         Images.Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 5);
         //Images.SetBkColor(RGB(255,0,255));
         Images.Add(icons, RGB(255,0,255));
	      ListView.SetImageList(&Images, LVSIL_SMALL);
         delete icons;

         // Edit
	      if (!Search.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL, rectDummy, this, 2))
            throw Win32Exception(HERE, L"Unable to create game data window edit control");
         Search.SetFont(&afxGlobalData.fontRegular);
         Search.SetCueBanner(L"Enter search term...");

         // Combobox
	      if (!Groups.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, rectDummy, this, 3))
            throw Win32Exception(HERE, L"Unable to create game data window combo box");
         Groups.SetFont(&afxGlobalData.fontRegular);

         // Populate groups
         Groups.AddString(L"All Groups");
         for (UINT g = (UINT)CommandGroup::ARRAY; g < (UINT)CommandGroup::HIDDEN; ++g)
            Groups.AddString(GuiString(IDS_FIRST_COMMAND_GROUP + g).c_str());
         Groups.SetCurSel(0);

	      // Layout controls + Populate
	      AdjustLayout();
         UpdateContent();
	      return 0;
      }
      catch (ExceptionBase& e)
      {
         Console << e << ENDL;
         return -1;
      }
   }

   void CGameDataWnd::OnPaint()
   {
	   CPaintDC dc(this); 

	   CRect rc;
	   ListView.GetWindowRect(rc);
	   ScreenToClient(rc);

      // Draw border
	   rc.InflateRect(1, 1);
	   dc.Draw3dRect(rc, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
   }

   void CGameDataWnd::OnSetFocus(CWnd* pOldWnd)
   {
	   CDockablePane::OnSetFocus(pOldWnd);

	   ListView.SetFocus();
   }

   void CGameDataWnd::OnSearchTermChanged()
   {
      UpdateContent();
   }

   void CGameDataWnd::OnSize(UINT nType, int cx, int cy)
   {
	   CDockablePane::OnSize(nType, cx, cy);
	   AdjustLayout();
   }

   // ------------------------------- PRIVATE METHODS ------------------------------
   
   void CGameDataWnd::AdjustLayout()
   {
      // Destroyed/Minimised
	   if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
         return;

	   CRect client, edit, combo;
	   GetClientRect(client);
      Search.GetClientRect(edit);
      Groups.GetClientRect(combo);

      // Anchor Edit to top
      Search.SetWindowPos(NULL, client.left, client.top, client.Width(), combo.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

      // Anchor Group below Edit
      Groups.SetWindowPos(NULL, client.left, client.top+edit.Height(), client.Width(), combo.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

      // Stretch ListView over remaining area
      UINT header = edit.Height()+combo.Height();
      ListView.SetWindowPos(NULL, client.left, client.top+header, client.Width(), client.Height()-header, SWP_NOACTIVATE | SWP_NOZORDER);

      // Stretch ListView column
      ListView.SetColumnWidth(0, client.Width());
   }

   void  CGameDataWnd::UpdateContent()
   {
      try
      {
         // Clear prev content
         Content.clear();
         ListView.RemoveAllGroups();
         ListView.DeleteAllItems();

         // Display nothing if no game data
         if (theApp.State == AppState::GameDataPresent)
         {
            // Get search term
            CString searchTerm;
            Search.GetWindowTextW(searchTerm);

            // Lookup matches
            Content = SyntaxLib.Query((const WCHAR*)searchTerm, GameVersion::TerranConflict);
            ListView.SetItemCount(Content.size());
            
            // Redefine groups
            for (auto pair : SyntaxLib.GetGroups())
            {
               const wstring& name = pair.second;
               UINT id = (UINT)pair.first;
               
               // Insert group
               LVGroup g(id, name);
               if (ListView.InsertGroup(g.iGroupId, (LVGROUP*)&g) != g.iGroupId)
                  throw Win32Exception(HERE, GuiString(L"Unable to insert command group ") + name);
            }

            // Generate/insert display text for each command
            for (UINT i = 0; i < Content.size(); ++i)
            {
               LVItem item(i, Content[i]->GetDisplayText(), (UINT)Content[i]->Group, LVIF_TEXT | LVIF_GROUPID | LVIF_IMAGE);
               item.iImage = 0;

               // Insert item
               if (ListView.InsertItem((LVITEM*)&item) == -1)
                  throw Win32Exception(HERE, GuiString(L"Unable to insert command '%s' (item %d, group %d)", item.pszText, i, item.iGroupId));
            }
         }
      }
      catch (ExceptionBase& e)
      {
         Console << e << ENDL;
      }
   }
   
NAMESPACE_END(GUI)
