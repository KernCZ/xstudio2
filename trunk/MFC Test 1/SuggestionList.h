#pragma once
#include "afxcmn.h"


/// <summary>User interface</summary>
NAMESPACE_BEGIN2(GUI,Controls)
   
   class ScriptEdit;

   /// <summary></summary>
   class SuggestionList : public CListCtrl
   {
      // ------------------------ TYPES --------------------------
   private:
      typedef vector<wstring>  StringArray;
	  
      // --------------------- CONSTRUCTION ----------------------
   public:
      SuggestionList();
      virtual ~SuggestionList();
       
      // ------------------------ STATIC -------------------------
   
      DECLARE_DYNAMIC(SuggestionList)
      DECLARE_MESSAGE_MAP()
	  
      // --------------------- PROPERTIES ------------------------
	  
      // ---------------------- ACCESSORS ------------------------			
   public:
   #ifdef _DEBUG
	   virtual void AssertValid() const;
	   virtual void Dump(CDumpContext& dc) const;
   #endif  
      ScriptEdit* GetParent() const;
      wstring GetSuggestion() const;

      // ----------------------- MUTATORS ------------------------
   public:
      BOOL Create(ScriptEdit* parent, CPoint& pt);
      void MatchSuggestion(const wstring& txt);

   protected:
      void AdjustLayout();
      int  Populate();
	  
      afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnKillFocus(CWnd* pNewWnd);
      afx_msg void OnRetrieveItem(NMHDR *pNMHDR, LRESULT *pResult);
	   afx_msg void OnSize(UINT nType, int cx, int cy);
	  
      // -------------------- REPRESENTATION ---------------------
      
   private:
      const CSize DefaultSize = CSize(300,200);
      
      StringArray  Content;
   };
   

NAMESPACE_END2(GUI,Controls)
