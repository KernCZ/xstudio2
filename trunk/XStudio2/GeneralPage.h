#pragma once
#include "PreferencesPage.h"


/// <summary>User interface</summary>
NAMESPACE_BEGIN2(GUI,Preferences)
   
   /// <summary></summary>
   class GeneralPage : public PreferencesPage
   {
      // ------------------------ TYPES --------------------------
   public:
      enum { IDD = IDD_GENERAL_PAGE };

   private:
	  
      // --------------------- CONSTRUCTION ----------------------
   public:
      GeneralPage();    
      virtual ~GeneralPage();
       
      // ------------------------ STATIC -------------------------
      DECLARE_DYNAMIC(GeneralPage)
      DECLARE_MESSAGE_MAP()
	  
      // --------------------- PROPERTIES ------------------------
	  
      // ---------------------- ACCESSORS ------------------------			
   public:

      // ----------------------- MUTATORS ------------------------
   protected:
      void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	  
      // -------------------- REPRESENTATION ---------------------
      
   private:
   };
   
NAMESPACE_END2(GUI,Preferences)
