#pragma once
#include "Resource.h"
#include "STL.h"

#ifdef FormatMessage
#undef FormatMessage
#endif

/// Macro: WIDEN(x)  
// Description: Creates a wide char equivilent of an input string (from MSDN)
#define WIDEN2(x)                L ## x
#define WIDEN(x)                 WIDEN2(x)

/// Macro: __WFILE__
// Description: Wide character equivilent of __FILE__
#define __WFILE__                WIDEN(__FILE__)

/// Macro: __WFUNCTION__
// Description: Wide character equivilent of __FUNCTION__
#define __WFUNCTION__            WIDEN(__FUNCTION__)

// Macro: Prevents a class from being moved
#define NO_MOVE(type)            type::type(type&&) = delete;  type& operator=(type&&) = delete;

// Macro: Prevents a class from being copied
#define NO_COPY(type)            type::type(const type&) = delete;  type& operator=(const type&) = delete;

namespace Library
{
   /// <summary>A unique pointer to a byte array</summary>
   typedef unique_ptr<BYTE, default_delete<BYTE[]>>  ByteArrayPtr;

   /// <summary>Provides helper functions for loading/formatting strings from resources</summary>
   class StringResource
   {
   public:
      static wstring  Format(const WCHAR*  format, ...);
      static wstring  Load(UINT  id, ...);
      static wstring  LoadV(UINT  id, va_list  pArgs);

      static wstring  FormatMessage(UINT  id);
   };

   /// <summary>Represents a file or folder path</summary>
   class Path : public wstring
   {
   public:
      Path() {}
      Path(const WCHAR* path) : wstring(path) {}
      Path(wstring path) : wstring(path) {}

      Path&  AppendPath(wstring  path);
      Path&  RenameExtension(wstring  ext);
      Path&  AppendBackslash();
      Path&  RemoveBackslash();
      bool  Exists();
      bool  IsDirectory();

   };

   /// <summary>Provides the ability to search for files</summary>
   class FileSearch
   {
   public:
      FileSearch(wstring  term);
      ~FileSearch();

      void  Close();
      bool  Exists();
      bool  Next();

   private:
      WIN32_FIND_DATA Data;
      HANDLE          Handle;
   };


}