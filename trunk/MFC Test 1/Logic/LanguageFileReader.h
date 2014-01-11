#pragma once
#include "Common.h"
#include "LanguageFile.h"
#include "XmlReader.h"


namespace Logic
{
   namespace IO
   {
      /// <summary>Parses the filenames of language files</summary>
      class LanguageFilenameReader
      {
         // --------------------- CONSTRUCTION ----------------------
      public:
         LanguageFilenameReader(const wstring& fn);

         // ------------------------ STATIC -------------------------
      
         static UINT         ParseFileID(const wstring& sz);
         static GameLanguage ParseLanguageID(const wstring& sz);

         // --------------------- PROPERTIES ------------------------
			
			// ---------------------- ACCESSORS ------------------------

			// ----------------------- MUTATORS ------------------------

         // -------------------- REPRESENTATION ---------------------

         bool          Valid,
                       HasLanguage;
         GameLanguage  Language;
         UINT          FileID;
      };

      /// <summary>Reads strings and pages of an X3 language xml file</summary>
      class LanguageFileReader : XmlReader
      {
         // --------------------- CONSTRUCTION ----------------------

      public:
         LanguageFileReader(StreamPtr in);
         ~LanguageFileReader();

         // ------------------------ STATIC -------------------------

      private:
         static UINT    ParsePageID(const wstring&  pageid, GameVersion&  v);

         // --------------------- PROPERTIES ------------------------
			
			// ---------------------- ACCESSORS ------------------------

			// ----------------------- MUTATORS ------------------------

      public:
         LanguageFile   ReadFile(wstring filename);

      private:
         GameLanguage   ReadLanguageTag(XML::IXMLDOMNodePtr&  element);
         LanguagePage   ReadPage(XML::IXMLDOMNodePtr&  element);
         LanguageString ReadString(XML::IXMLDOMNodePtr&  element, GameVersion v);

         // -------------------- REPRESENTATION ---------------------
      };

   }
}

using namespace Logic::IO;
