// LanguageEditView.cpp : implementation file
//

#include "stdafx.h"
#include "LanguageEditView.h"
#include "ListView.h"
#include "afxcview.h"

/// <summary>User interface</summary>
NAMESPACE_BEGIN2(GUI,Views)

   // --------------------------------- APP WIZARD ---------------------------------
  
   // -------------------------------- CONSTRUCTION --------------------------------

   LanguageEditView::LanguageEditView() : CFormView(LanguageEditView::IDD)
   {

   }

   LanguageEditView::~LanguageEditView()
   {
   }

   // ------------------------------- STATIC METHODS -------------------------------

   IMPLEMENT_DYNCREATE(LanguageEditView, CFormView)

   BEGIN_MESSAGE_MAP(LanguageEditView, CFormView)
      ON_WM_SIZE()
   END_MESSAGE_MAP()

   // ------------------------------- PUBLIC METHODS -------------------------------

   #ifdef _DEBUG
   void LanguageEditView::AssertValid() const
   {
	   CFormView::AssertValid();
   }

   void LanguageEditView::Dump(CDumpContext& dc) const
   {
	   CFormView::Dump(dc);
   }

   LanguageDocument* LanguageEditView::GetDocument() const // non-debug version is inline
   {
	   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(LanguageDocument)));
	   return (LanguageDocument*)m_pDocument;
   }
   #endif //_DEBUG

   LanguageStringView*  LanguageEditView::GetStringView() const
   {
      // Iterate thru views
      for (POSITION pos = GetDocument()->GetFirstViewPosition(); pos != NULL; )
      {
         auto pView = dynamic_cast<LanguageStringView*>(GetDocument()->GetNextView(pos));
         if (pView != nullptr)
            return pView;
      }   

      throw GenericException(HERE, L"Cannot find language string View");
   }

   // ------------------------------ PROTECTED METHODS -----------------------------

   void  LanguageEditView::AdjustLayout()
   {
      // Destroyed/Minimised
	   if (RichEdit.GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
         return;
         
      CRect view;
      GetClientRect(view);

      // TODO: Anchor toolbar to top

      // Stretch RichEdit over remainder
      RichEdit.SetWindowPos(nullptr, 0, 40, view.Width(), view.Height()-40, SWP_NOZORDER | SWP_NOACTIVATE);
   }

   void LanguageEditView::DoDataExchange(CDataExchange* pDX)
   {
      CFormView::DoDataExchange(pDX);
      DDX_Control(pDX, IDC_STRING_EDIT, RichEdit);
   }

   
   void LanguageEditView::OnInitialUpdate()
   {
      CFormView::OnInitialUpdate();

      // Listen for string SelectionChanged
      fnStringSelectionChanged = GetStringView()->SelectionChanged.Register(this, &LanguageEditView::onStringSelectionChanged);
   }

   void LanguageEditView::onStringSelectionChanged()
   {
      // Clear text
      RichEdit.SetWindowTextW(L"");

      // Re-populate
      if (LanguageString* str = GetStringView()->GetSelectedString())
         RichEdit.SetWindowTextW(str->Text.c_str());
   }
   
   void LanguageEditView::OnSize(UINT nType, int cx, int cy)
   {
      CFormView::OnSize(nType, cx, cy);
      AdjustLayout();
   }

   // ------------------------------- PRIVATE METHODS ------------------------------

   
NAMESPACE_END2(GUI,Views)



