#include "stdafx.h"
#include "LanguageFileReader.h"

namespace Logic
{
   namespace IO
   {
      // -------------------------------- CONSTRUCTION --------------------------------

      /// <summary>Creates an xml reader from an input stream</summary>
      /// <param name="src">The input stream</param>
      /// <exception cref="Logic::ArgumentException">Stream is not readable</exception>
      /// <exception cref="Logic::ArgumentNullException">Stream is null</exception>
      /// <exception cref="Logic::ComException">COM Error</exception>
      LanguageFileReader::LanguageFileReader(StreamPtr in) : XmlReader(in)
      {
      }

      LanguageFileReader::~LanguageFileReader()
      {
      }

      /// <summary>Parses the file ID of a language file</summary>
      /// <param name="fn">The filename including extension</param>
      LanguageFilenameReader::LanguageFilenameReader(const wstring& fn) : HasLanguage(false), FileID(0), Valid(false)
      {
         try
         {
            // Ensure extension is XML/PCK
            if (Path(fn).HasExtension(L".xml") || Path(fn).HasExtension(L".pck"))
            {
               // No language ID: nnnn.pck
               if (fn.length() == 4+4)
                  FileID = ParseFileID(fn.substr(0,4));

               // X3R: LLnnnn.pck
               else if (fn.length() == 6+4)
               {
                  FileID = ParseFileID(fn.substr(2,4));
                  Language = ParseLanguageID(fn.substr(0,2));
                  HasLanguage = true;
               }
               // X3TC: nnnn-L0nn.pck
               else if (fn.length() == 9+4)
               {
                  FileID = ParseFileID(fn.substr(0,4));
                  Language = ParseLanguageID(fn.substr(7,2));
                  HasLanguage = true;
               }
            
               // Validate
               Valid = FileID > 0;
            }
         }
         catch (...)
         {
            Valid = false;
         }
      }

      // ------------------------------- STATIC METHODS -------------------------------

      /// <summary>Parses the language ID of a language file</summary>
      /// <param name="id">The language identifier</param>
      /// <returns>Game language</returns>
      /// <exception cref="Logic::InvalidValueException">Invalid language ID</exception>
      GameLanguage  LanguageFilenameReader::ParseLanguageID(const wstring& id)
      {
         switch (int val = _wtoi(id.c_str()))
         {
         case 44:
         case 48:  
            return (GameLanguage)val; 

         default:
            // "'%s' is not a valid language ID"
            throw InvalidValueException(HERE, GuiString(ERR_LANGUAGE_ID_INVALID, id.c_str()));
         }
      }

      /// <summary>Parses the file ID of a language file</summary>
      /// <param name="id">The file identifier</param>
      /// <returns>File ID</returns>
      UINT  LanguageFilenameReader::ParseFileID(const wstring& id)
      {
         return _wtoi(id.c_str());
      }

      /// <summary>Parses the page identifier</summary>
      /// <param name="pageid">The pageid</param>
      /// <param name="v">The associated game version</param>
      /// <returns>Normalised page ID</returns>
      /// <exception cref="Logic::InvalidValueException">Invalid pageID</exception>
      UINT  LanguageFileReader::ParsePageID(const wstring&  pageid, GameVersion&  v)
      {
         if (pageid.length() <= 4)
         {
            v = GameVersion::Threat;
            return _wtoi(pageid.c_str());
         }
         else if (pageid.length() != 6)
            throw InvalidValueException(HERE, GuiString(L"Invalid page ID '%s'", pageid.c_str()) );

         else if (pageid.compare(0, 2, L"30") == 0)
            v = GameVersion::Reunion;
         
         else if (pageid.compare(0, 2, L"35") == 0)
            v = GameVersion::TerranConflict;

         else if (pageid.compare(0, 2, L"38") == 0)
            v = GameVersion::AlbionPrelude;

         else
            throw InvalidValueException(HERE, GuiString(L"Invalid page ID '%s'", pageid.c_str()) );

         // Convert last four digits of page ID
         return _wtoi(pageid.substr(2).c_str());
      }

		// ------------------------------- PUBLIC METHODS -------------------------------

      /// <summary>Reads the entire language file</summary>
      /// <param name="filename">The filename</param>
      /// <returns>New language file</returns>
      /// <exception cref="Logic::ComException">COM Error</exception>
      /// <exception cref="Logic::FileFormatException">Corrupt XML / Missing elements / missing attributes</exception>
      /// <exception cref="Logic::InvalidValueException">Invalid languageID or pageID</exception>
      /// <exception cref="Logic::IOException">An I/O error occurred</exception>
      LanguageFile LanguageFileReader::ReadFile(wstring filename)
      {
         LanguageFile file;

         try
         {
            // Parse document
            LoadDocument();

            // Get root (as node)
            XML::IXMLDOMNodePtr languageNode(Document->documentElement);

            // Read fileID + language tag
            file.ID = LanguageFilenameReader(filename).FileID;
            file.Language = ReadLanguageTag(languageNode);

            // Read pages
            for (int i = 0; i < languageNode->childNodes->length; i++)
               file.Pages.Merge( ReadPage(languageNode->childNodes->item[i]) );

            return file;
         }
         catch (_com_error& ex) {
            throw ComException(HERE, ex);
         }
      }

      // ------------------------------ PROTECTED METHODS -----------------------------

		// ------------------------------- PRIVATE METHODS ------------------------------

      /// <summary>Reads the language tag and parses the ID</summary>
      /// <param name="element">The language element</param>
      /// <returns>Language ID</returns>
      /// <exception cref="Logic::FileFormatException">Missing language element</exception>
      /// <exception cref="Logic::InvalidValueException">Invalid language ID</exception>
      /// <exception cref="Logic::ComException">COM Error</exception>
      GameLanguage  LanguageFileReader::ReadLanguageTag(XML::IXMLDOMNodePtr&  element)
      {
         // Ensure present: "Missing '%s' element"
         if (element == nullptr)
            throw FileFormatException(HERE, GuiString(ERR_XML_MISSING_ELEMENT, L"language"));

         // Verify tag
         ReadElement(element, L"language");

         // Convert language ID
         return LanguageFilenameReader::ParseLanguageID(ReadAttribute(element, L"id"));
      }

      
      /// <summary>Reads a page tag and all it's string tags</summary>
      /// <param name="element">Page element</param>
      /// <returns>New language page</returns>
      /// <exception cref="Logic::FileFormatException">Missing element or attributes</exception>
      /// <exception cref="Logic::InvalidValueException">Invalid page ID</exception>
      /// <exception cref="Logic::ComException">COM Error</exception>
      LanguagePage  LanguageFileReader::ReadPage(XML::IXMLDOMNodePtr&  element)
      {
         GameVersion ver;

         // Verify page tag
         ReadElement(element, L"page");

         // Read properties
         wstring id(ReadAttribute(element, L"id")),
                 title(TryReadAttribute(element, L"title")),
                 desc(TryReadAttribute(element, L"desc"));
         bool    voice = (TryReadAttribute(element, L"voice") == L"yes");

         // Create page (also normalise PageID)
         LanguagePage page(ParsePageID(id, ver), title, desc, voice);

         // Read strings
         for (int i = 0; i < element->childNodes->length; i++)
            page.Strings.Add( ReadString(element->childNodes->item[i], ver) );

         // Return page
         return page;
      }

      /// <summary>Reads a string tag</summary>
      /// <param name="element">String 't' element</param>
      /// <param name="v">Version of page</param>
      /// <returns>New language string</returns>
      /// <exception cref="Logic::FileFormatException">Missing element or attributes</exception>
      /// <exception cref="Logic::ComException">COM Error</exception>
      LanguageString  LanguageFileReader::ReadString(XML::IXMLDOMNodePtr&  element, GameVersion v)
      {
         // Verify string tag
         ReadElement(element, L"t");

         // Read ID+text
         return LanguageString(_wtoi(ReadAttribute(element, L"id").c_str()), (WCHAR*)element->text, v);
      }
   }
}