#include "stdafx.h"
#include "DebugTests.h"
#include "FileStream.h"
#include "CatalogStream.h"
#include "GZipStream.h"
#include "StringReader.h"
#include "LanguageFileReader.h"
#include "XFileSystem.h"
#include "LegacySyntaxReader.h"
#include "SyntaxLibrary.h"
#include "ScriptReader.h"
#include "StringLibrary.h"

namespace Library
{
   // -------------------------------- CONSTRUCTION --------------------------------

   // ------------------------------- STATIC METHODS -------------------------------

   void  DebugTests::RunAll()
   {
      //Test_LanguageFileReader();
      //Test_CatalogReader();
      //Test_GZip();
      //Test_FileSystem();
      //Test_CommandSyntax();
      Test_StringLibrary();
   }

   ScriptFile  DebugTests::LoadScript(const WCHAR*  path)
   {
      try
      {
         // Load legacy syntax file
         StreamPtr fs( new FileStream(L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\Command Syntax.txt", FileMode::OpenExisting, FileAccess::Read) );
         SyntaxLibrary::Merge( LegacySyntaxReader(fs).ReadFile() );

         // Parse script
         StreamPtr fs2( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         return ScriptReader(fs2).ReadFile();
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to load '%s' : %s\n\n" L"Source: %s()", path, e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
         throw e;
      }
   }

   void DebugTests::Test_CommandSyntax()
   {
      const WCHAR* path = L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\Command Syntax.txt"; 
   
      try
      {
         // Test LegacySyntaxReader
         StreamPtr fs( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         SyntaxLibrary::Merge( LegacySyntaxReader(fs).ReadFile() );

         // Test Lookup
         SyntaxLibrary::Find(CMD_EXPRESSION, GameVersion::TerranConflict);

         // Test script reader
         path = L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\plugin.piracy.enslavepassengers.xml"; 
         StreamPtr fs2( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         auto script = ScriptReader(fs2).ReadFile();
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to load '%s' : %s\n\n" L"Source: %s()", path, e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
      }
   }

   void  DebugTests::Test_LanguageFileReader()
   {
      //const WCHAR* path = L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\testfile.xml"; 
      const WCHAR* path = L"D:\\My Projects\\BearScript\\Data\\Relevant Files\\AP.0001-L044.xml";
   
      try
      {
         // Test LanguageFileReader
         StreamPtr fs( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         auto langFile = LanguageFileReader(fs).ReadFile(Path(path).GetFileName());
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to load '%s' : %s\n\n" L"Source: %s()", path, e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
      }
   }

   void  DebugTests::Test_CatalogReader()
   {
      const WCHAR* path = L"D:\\X3 Albion Prelude\\11.cat";
   
      try
      {
         // Test catalogue reader
         StreamPtr cs( new CatalogStream(path, FileMode::OpenExisting, FileAccess::Read) );
         StringReader rd(cs);
      
         wstring line;
         while (rd.ReadLine(line))
            OutputDebugString((line+L'\n').c_str());
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to load '%s' : %s\n\n" L"Source: %s()", path, e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
      }
   }

   void  DebugTests::Test_GZip()
   {
      const WCHAR* path = L"D:\\X3 Albion Prelude\\scripts\\!config.earth.torus.pck";
   
      try
      {
         // Test GZipStream
         StreamPtr file = StreamPtr( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         StreamPtr zip = StreamPtr( new GZipStream(file, GZipStream::Operation::Decompression) );

         StringReader reader(zip);
         wstring line;

         while (reader.ReadLine(line))
            OutputDebugString((line+L'\n').c_str());
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to load '%s' : %s\n\n" L"Source: %s()", path, e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
      }
   }

   void  DebugTests::Test_FileSystem()
   {
      XFileSystem vfs;
      CString err;
   
      try 
      {
         // Test XFileSystem
         vfs.Enumerate(L"D:\\X3 Albion Prelude", GameVersion::TerranConflict);

         // Test browse
         for (auto f : vfs.Browse(L"D:\\X3 Albion Prelude\\t"))
            OutputDebugString(GuiString(L"Found file: '%s'\n", (const WCHAR*)f.FullPath).c_str());

         // Test search
         XFileInfo f = vfs.Find(L"D:\\X3 Albion Prelude\\types\\TLaser");
         StringReader reader(f.Open());

         // Print file
         wstring line;
         while (reader.ReadLine(line))
            OutputDebugString((line+L'\n').c_str());
      } 
      catch (ExceptionBase&  e) {
         err.Format(L"Unable to enumerate VFS : %s\n\n" L"Source: %s()", e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(err);
         return;
      }
   }

   void  DebugTests::Test_StringLibrary()
   {
      StringLibrary lib;
      XFileSystem vfs;
      CString err;
   
      try 
      {
         // Test enumerate
         vfs.Enumerate(L"D:\\X3 Albion Prelude", GameVersion::TerranConflict);
         lib.Enumerate(vfs, GameLanguage::English);

         // Test search
         list<LanguageString>  res;

         res.push_back( lib.Find(7, 1020112) );
         res.push_back( lib.Find(7, 1020114) );
         res.push_back( lib.Find(7, 1020117) );
         res.push_back( lib.Find(6022, 8) );

         // Print results
         for (LanguageString& s : res)
         {
            OutputDebugString(s.Text.c_str());
            OutputDebugString(L"\n");
         }
      } 
      catch (ExceptionBase&  e) {
         err.Format(L"Unable to enumerate VFS : %s\n\n" L"Source: %s()", e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(err);
         return;
      }
   }

   // ------------------------------- PUBLIC METHODS -------------------------------

	// ------------------------------ PROTECTED METHODS -----------------------------

	// ------------------------------- PRIVATE METHODS ------------------------------

}