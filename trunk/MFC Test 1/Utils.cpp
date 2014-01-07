#include "stdafx.h"
#include "Utils.h"
#include "Exceptions.h"
#include <Strsafe.h>       // C String handling
#include "MFC Test 1.h"    // the app
#include "Shlwapi.h"       // PathFileExists

namespace Library
{

   /// <summary>Assembles a formatted string</summary>
   /// <param name="format">Formatting string</param>
   /// <param name="">Formatting arguments</param>
   /// <returns></returns>
   wstring  StringResource::Format(const WCHAR*  format, ...)
   {
      TCHAR    szBuffer[512];
      va_list  pArgs;

      // Format + Return
      StringCchVPrintf(szBuffer, 512, format, va_start(pArgs, format));
      return szBuffer;
   }

   /// <summary>Assembles a formatted string stored as a resource</summary>
   /// <param name="id">The resource identifier</param>
   /// <param name="">Formatting arguments</param>
   /// <returns></returns>
   wstring  StringResource::Load(UINT  id, ...)
   {
      va_list  pArgs;
      return LoadV(id, va_start(pArgs, id));
   }

   /// <summary>Assembles a formatted string stored as a resource</summary>
   /// <param name="id">The resource identifier</param>
   /// <param name="pArgs">Variable arguments</param>
   /// <returns></returns>
   wstring  StringResource::LoadV(UINT  id, va_list  pArgs)
   {
      TCHAR    szBuffer[512],
               szFormat[512];
      
      // Load string + format
      if (LoadString(theApp.m_hInstance, id, szFormat, 512))
         StringCchVPrintf(szBuffer, 512, szFormat, pArgs);
      else
         StringCchPrintf(szBuffer, 512, L"Error: Missing string '%d'", id);

      // Return
      return szBuffer;
   }

   /// <summary>Formats a system error message</summary>
   /// <param name="id">The error code</param>
   /// <returns></returns>
   wstring  StringResource::FormatMessage(UINT  id)
   {
      WCHAR* szBuffer;
      wstring msg;

      if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, id, NULL, (WCHAR*)&szBuffer, 512, NULL))
      {
         msg = szBuffer;
         LocalFree(szBuffer);
      }

      return msg;
   }

   Path&  Path::AppendPath(wstring  path)
   {
      AppendBackslash();
      append(path);
      
      return *this;
   }

   Path&  Path::AppendBackslash()
   {
      if (length() > 0 && back() != '\\')
         append(L"\\");

      return *this;
   }

   Path&  Path::RemoveBackslash()
   {
      if (length() > 0 && back() == '\\')
         pop_back();

      return *this;
   }

   Path&  Path::RenameExtension(wstring  ext)
   {
      auto_ptr<WCHAR>  path(new WCHAR[MAX_PATH]);
      StringCchCopy(path.get(), MAX_PATH, c_str());

      if (!PathRenameExtension(path.get(), ext.c_str()))
         throw Win32Exception(HERE, L"path exceeds character limit");

      assign(path.get());
      return *this;
   }
   
   /// <summary>Determines whether a file or folder exists</summary>
   /// <returns>true/false</returns>
   bool  Path::Exists()
   { 
      return PathFileExists(c_str()) != FALSE; 
   }

   /// <summary>Determines whether path is directory</summary>
   /// <returns></returns>
   /// <exception cref="Library::Win32Exception"></exception>
   bool  Path::IsDirectory()
   {
      int attr = GetFileAttributes(c_str());
      
      if (attr == INVALID_FILE_ATTRIBUTES)
         throw Win32Exception(HERE);

      return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
   }


   FileSearch::FileSearch(wstring  term)
   {
      ZeroMemory(&Data, sizeof(WIN32_FIND_DATA));
      Handle = FindFirstFileEx(term.c_str(), FindExInfoBasic, &Data, FindExSearchNameMatch, NULL, NULL);
   }

   FileSearch::~FileSearch()
   {
      Close();
   }

   bool  FileSearch::Exists()
   {
      return Handle != INVALID_HANDLE_VALUE;
   }

   void  FileSearch::Close()
   {
      if (Handle != INVALID_HANDLE_VALUE)
      {
         CloseHandle(Handle);
         Handle = INVALID_HANDLE_VALUE;
      }
   }

   bool  FileSearch::Next()
   {
      // Find next result
      if (Exists() && !FindNextFile(Handle, &Data))
         Close();

      return Exists();
   }

}