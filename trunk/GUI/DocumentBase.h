#pragma once
#include "../Logic/MatchData.h"

/// <summary>User interface documents</summary>
NAMESPACE_BEGIN2(GUI,Documents)
   
   /// <summary></summary>
   enum class DocumentType { Script, Language, Mission, Project, Diff };

   /// <summary></summary>
   class FileTemplate
   {
      // --------------------- CONSTRUCTION ----------------------
   public:
      FileTemplate(wstring name, DocumentType type, LPCWSTR defExt, UINT icon, LPCWSTR desc, LPCWSTR subpath) 
         : Name(name), Type(type), Extension(defExt), Icon(icon), Description(desc), SubPath(subpath)
      {}

      // --------------------- PROPERTIES ------------------------
	  
      // ---------------------- ACCESSORS ------------------------			

      // ----------------------- MUTATORS ------------------------

      // -------------------- REPRESENTATION ---------------------
   public:
      const wstring      Name;            // Template name
      const LPCWSTR      Extension,       // Default file extension
                         Description,     // Template description
                         SubPath;         // File SubPath: 'Templates\...'
      const DocumentType Type;            // Document type
      const UINT         Icon;            // ImageList index of document icon 
   };


   /// <summary></summary>
   class DocumentBase : public CDocument
   {
      // ------------------------ TYPES --------------------------
   private:
	  
      // --------------------- CONSTRUCTION ----------------------
   protected:
      DocumentBase();
      DocumentBase(DocumentType type, bool isVirtual);
   public:
      virtual ~DocumentBase();
       
      // ------------------------ STATIC -------------------------
      DECLARE_DYNCREATE(DocumentBase)
      DECLARE_MESSAGE_MAP()

   public:
      static DocumentBase* GetActive();

      // --------------------- PROPERTIES ------------------------
   public:
      PROPERTY_GET_SET(Path,FullPath,GetFullPath,SetFullPath);
      PROPERTY_GET_SET(bool,Virtual,GetVirtual,SetVirtual);

      // ---------------------- ACCESSORS ------------------------			
   public:
      virtual bool      FindNext(UINT start, MatchData& m) const;
      virtual CHARRANGE GetSelection() const;
      Path              GetFullPath() const;
      DocumentType      GetType() const;
      bool              GetVirtual() const;

      // ----------------------- MUTATORS ------------------------
   public:
      void          Activate();
      virtual BOOL  OnOpenTemplate(const FileTemplate& t);
      virtual void  Rename(const wstring& name);
      virtual bool  Replace(MatchData& m);
      void          SetFullPath(Path path);
      void          SetModifiedFlag(BOOL bModified = TRUE) override;
      virtual void  SetSelection(CHARRANGE rng);
      void          SetTitle(LPCTSTR title) override;
      void          SetVirtual(bool v);

   protected:
      void  OnQueryCommand(CCmdUI* pCmdUI);
	   
      // -------------------- REPRESENTATION ---------------------
   protected:
      DocumentType  Type;

   private:
      bool          IsVirtual;
   };

   /// <summary>List of documents</summary>
   typedef list<DocumentBase*>  DocumentList;
   

/// <summary>User interface documents</summary>
NAMESPACE_END2(GUI,Documents)
