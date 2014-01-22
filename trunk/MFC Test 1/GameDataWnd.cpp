#include "stdafx.h"
#include "GameDataWnd.h"
#include <strsafe.h>

/// <summary>User interface</summary>
NAMESPACE_BEGIN(GUI)

   // --------------------------------- APP WIZARD ---------------------------------
  
   BEGIN_MESSAGE_MAP(CGameDataWnd, CDockablePane)
	   ON_WM_CREATE()
	   ON_WM_SIZE()
	   ON_WM_CONTEXTMENU()
	   ON_WM_PAINT()
	   ON_WM_SETFOCUS()
      ON_NOTIFY(LVN_GETDISPINFO,1,&CGameDataWnd::onRequestItem)
   END_MESSAGE_MAP()

   // -------------------------------- CONSTRUCTION --------------------------------

   CGameDataWnd::CGameDataWnd()
   {
   }


   CGameDataWnd::~CGameDataWnd()
   {
   }

   // ------------------------------- STATIC METHODS -------------------------------

   // ------------------------------- PUBLIC METHODS -------------------------------

   // ------------------------------ PROTECTED METHODS -----------------------------

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

         // Create page
	      if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		      throw Win32Exception(HERE, L"Unable to create dockable pane");

	      // Create List:
	      if (!ListView.CreateEx(LVS_EX_FULLROWSELECT, WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDATA, rectDummy, this, 1))
            throw Win32Exception(HERE, L"Unable to create game data window listview");

         // Insert columns
         ListView.InsertColumn(0, L"Item", LVCFMT_LEFT, 200, 0);
         ListView.InsertColumn(1, L"Group", LVCFMT_LEFT, 100, 1);
         

	      // Load view images:
	      /*Images.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	      ListView.SetImageList(&Images, LVSIL_NORMAL);*/

	      // Resize + Display
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

   void CGameDataWnd::OnSize(UINT nType, int cx, int cy)
   {
	   CDockablePane::OnSize(nType, cx, cy);
	   AdjustLayout();
   }


   void CGameDataWnd::onRequestItem(NMHDR* pNMHDR, LRESULT* pResult)
   {
      LVITEM& item = reinterpret_cast<NMLVDISPINFO*>(pNMHDR)->item;
      auto syntax = Content[item.iItem];
      
      // Text
      if (item.mask & LVIF_TEXT)
         StringCchCopy(item.pszText, item.cchTextMax, syntax->Text.c_str());

      *pResult = TRUE;
   }

   // ------------------------------- PRIVATE METHODS ------------------------------
   
   void CGameDataWnd::AdjustLayout()
   {
	   if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
         return;

	   CRect rcClient;
	   GetClientRect(rcClient);

      ListView.SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
   }

   void  CGameDataWnd::UpdateContent()
   {
      // TODO: Get search text

      // Lookup matches
      Content.clear();
      Content = SyntaxLib.Query(L"", GameVersion::TerranConflict);

      // Display results
      ListView.SetItemCountEx(Content.size());
   }
   
NAMESPACE_END(GUI)
