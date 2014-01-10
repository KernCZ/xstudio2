#include "stdafx.h"
#include "DebugTests.h"
#include "FileStream.h"
#include "CatalogStream.h"
#include "GZipStream.h"
#include "StringReader.h"
#include "LanguageFileReader.h"
#include "XFileSystem.h"

namespace Library
{
   // -------------------------------- CONSTRUCTION --------------------------------

   // ------------------------------- STATIC METHODS -------------------------------

   void  DebugTests::RunAll()
   {
      Test_LanguageFileReader();
      Test_CatalogReader();
      Test_GZip();
      Test_FileSystem();
      Test_FileSystem();
   }

   void  DebugTests::Test_LanguageFileReader()
   {
      const WCHAR* path = L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\testfile.xml"; 
   
      try
      {
         // Test LanguageFileReader
         StreamPtr fs( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );
         auto langFile = LanguageFileReader(fs).ReadFile();
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
      
         string line;
         while (rd.ReadLine(line))
            OutputDebugStringA((line+'\n').c_str());
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
         string line;

         while (reader.ReadLine(line))
            OutputDebugStringA((line+'\n').c_str());
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
      const WCHAR* path = L"D:\\X3 Albion Prelude\\types\\TLaser.pck";
   
      try
      {
         // Test XFileSystem
         XFileSystem vfs(L"D:\\X3 Albion Prelude", GameVersion::TerranConflict);
         vfs.Enumerate();

         // Test VFS search
         XFileInfo f = vfs.Find(path, true);

         // Print result
         StringReader reader(f.Open());
         string line;

         while (reader.ReadLine(line))
            OutputDebugStringA((line+'\n').c_str());

         /*AfxMessageBox( StringResource::Format(L"VFS contains '%s': %s\n", path, vfs.Contains(path,true) ? L"true" : L"false").c_str() );

         path = L"D:\\X3 Albion Prelude\\t\\9999-L044.pck";
         AfxMessageBox( StringResource::Format(L"VFS contains '%s': %s\n", path, vfs.Contains(path,false) ? L"true" : L"false").c_str() );*/
      }
      catch (ExceptionBase&  e)
      {
         CString sz;
         sz.Format(L"Unable to access VFS : %s\n\n" L"Source: %s()", e.Message.c_str(), e.Source.c_str());
         AfxMessageBox(sz);
      }
   }

   // ------------------------------- PUBLIC METHODS -------------------------------

	// ------------------------------ PROTECTED METHODS -----------------------------

	// ------------------------------- PRIVATE METHODS ------------------------------

}