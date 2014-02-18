#pragma once
#include "Logic/Event.h"
#include "Logic/Path.h"
#include "DocumentBase.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


/// <summary>Forward declarations</summary>
namespace GUI
{
   namespace Documents
   {
      class DocumentBase;
   }
   namespace Windows
   {
      class MainWnd;
   }
}


/// <summary>Application state</summary>
enum class AppState  { NoGameData, GameDataPresent };

/// <summary>Raise when application state changes</summary>
typedef Event<AppState>                    AppStateChangedEvent;
typedef AppStateChangedEvent::DelegatePtr  AppStateChangedHandler;

/// <summary>Main thread</summary>
class Application : public CWinAppEx
{
   // ------------------------ TYPES --------------------------
private:
   /// <summary>Document iterator</summary>
   class DocumentIterator : std::iterator<std::forward_iterator_tag, DocumentBase*>
   {
      // --------------------- CONSTRUCTION ----------------------
   public:
      /// <summary>Create document iterator.</summary>
      /// <param name="app">The application.</param>
      /// <param name="tmpl">First document template position.</param>
      DocumentIterator(const Application& app, POSITION tmpl) : App(app), Document(nullptr), DocumentPos(nullptr), TemplatePos(tmpl), Template(nullptr)
      {
         // Find first document
         if (TemplatePos = tmpl)
            while (TemplatePos && (Template=app.GetNextDocTemplate(TemplatePos)) )
               if (DocumentPos = Template->GetFirstDocPosition())
               {
                  Document = (DocumentBase*)Template->GetNextDoc(DocumentPos);
                  break;
               }

         // EOF: Ensure doc/template are null
         if (!Document)
            Template = nullptr;
      }
      // ------------------------ STATIC -------------------------

      // --------------------- PROPERTIES ------------------------
			
      // ---------------------- ACCESSORS ------------------------			
   public:
      const DocumentBase& operator*() const    { return *Document; }
      const DocumentBase* operator->() const   { return Document;  }
      DocumentBase& operator*()                { return *Document; }
      DocumentBase* operator->()               { return Document;  }

      // ----------------------- MUTATORS ------------------------
   public:
      DocumentIterator& operator++() 
      {
         // Find next document
         if (DocumentPos && (Document=(DocumentBase*)Template->GetNextDoc(DocumentPos)) )
            return *this;

         // End-Of-Current-Template: Find next non-empty template
         while (TemplatePos && (Template=App.GetNextDocTemplate(TemplatePos)) )
         {
            if (DocumentPos = Template->GetFirstDocPosition())
               if (Document = (DocumentBase*)Template->GetNextDoc(DocumentPos))
                  return *this;
         }

         // End-Of-All-Templates: EOF
         DocumentPos = nullptr;
         Document = nullptr;
         Template = nullptr;
         return *this; 
      }

      DocumentIterator operator++(int) 
      {
         DocumentIterator tmp(*this); 
         operator++(); 
         return tmp;
      }

      bool operator==(const DocumentIterator& r) const  { return Template==r.Template && Document==r.Document; }
      bool operator!=(const DocumentIterator& r) const  { return Template!=r.Template || Document!=r.Document; }

      // -------------------- REPRESENTATION ---------------------
   private:
      const Application&  App;
      CDocTemplate* Template;
      DocumentBase* Document;

      mutable POSITION  TemplatePos,
                        DocumentPos;
   };
	  
   // --------------------- CONSTRUCTION ----------------------
public:
	Application();

   // ------------------------ STATIC -------------------------
protected:
   DECLARE_MESSAGE_MAP()

public:
   AppStateChangedEvent     StateChanged;
   HINSTANCE                ResourceLibrary;

   // --------------------- PROPERTIES ------------------------
	
   PROPERTY_GET_SET(AppState,State,GetState,SetState);

   // ---------------------- ACCESSORS ------------------------			
   
public:
   DocumentIterator  cbegin() const;
   DocumentIterator  begin();
   DocumentIterator  cend() const;
   DocumentIterator  end();

   GUI::Windows::MainWnd*  GetMainWindow() const;

   DocumentBase& GetDocument(IO::Path p) const;
   DocumentList  GetOpenDocuments() const;
   AppState      GetState() const;
   bool          IsDocumentOpen(IO::Path p) const;

   HICON     LoadIconW(UINT nResID, UINT iSize) const;
   CBitmap*  LoadBitmapW(UINT nResID, int x, int y, UINT flags) const;
   BOOL      ShowError(const GuiString& src, const ExceptionBase& e, const GuiString& msg) const;
   BOOL      ShowError(const GuiString& src, const ExceptionBase& e) const;
   
   // ----------------------- MUTATORS ------------------------
public:
   virtual int   ExitInstance();
	virtual BOOL  InitInstance();
   virtual void  LoadCustomState();
   BOOL          OpenStringLibrary();
	virtual void  PreLoadState();
	virtual void  SaveCustomState();
   void          SetState(AppState s);
   
protected:
	afx_msg void OnAppAbout();

   // -------------------- REPRESENTATION ---------------------
protected:
   AppState    GameDataState;
};

extern Application theApp;