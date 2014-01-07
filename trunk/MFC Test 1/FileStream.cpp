#include "stdafx.h"
#include "FileStream.h"

namespace Library
{
   namespace IO
   {

      FileStream::FileStream(Path path, FileMode mode, FileAccess access, FileShare share) : FullPath(path), Mode(mode), Access(access), Share(share) 
      {
         // Ensure exists
         if (!path.Exists())
            throw FileNotFoundException(HERE, path);

         // Open file
         Handle = CreateFile(path.c_str(), (DWORD)access, (DWORD)share, NULL, (DWORD)mode, FILE_ATTRIBUTE_NORMAL, NULL);

         if (Handle == INVALID_HANDLE_VALUE)
            throw Win32Exception(HERE);
      }

      //FileStream::FileStream(const FileStream& s) : FullPath(s.FullPath), Mode(s.Mode), Access(s.Access), Share(s.Share) 
      //{
      //   // Duplicate handle
      //   if (!DuplicateHandle(GetCurrentProcess(), s.Handle, GetCurrentProcess(), &Handle, NULL, TRUE, DUPLICATE_SAME_ACCESS))
      //      throw Win32Exception(HERE);
      //}

      FileStream::~FileStream() 
      {
         SafeClose();
      }

      DWORD  FileStream::GetLength() 
      { 
         LARGE_INTEGER  size = {0LL};

         // Lookup file size
         if (!GetFileSizeEx(Handle, &size))
            throw Win32Exception(HERE);

         return (DWORD)size.QuadPart;
      }
      
      DWORD  FileStream::GetPosition() const
      {
         DWORD  position = 0;
         
         // Get position from null move
         if (SetFilePointer(Handle, 0, (LONG*)&position, (DWORD)SeekOrigin::Current) == INVALID_SET_FILE_POINTER)
            throw Win32Exception(HERE);

         return position;
      }

      void  FileStream::Close()
      {
         if (Handle != INVALID_HANDLE_VALUE) 
         {
            if (!CloseHandle(Handle))
               throw Win32Exception(HERE);

            Handle = INVALID_HANDLE_VALUE;
         }
      }

      void  FileStream::Flush()
      {
         if (!FlushFileBuffers(Handle))
            throw Win32Exception(HERE);
      }

      void  FileStream::SafeClose()
      {
         if (Handle != INVALID_HANDLE_VALUE) 
         {
            CloseHandle(Handle);
            Handle = INVALID_HANDLE_VALUE;
         }
      }

      void  FileStream::Seek(LONG  offset, SeekOrigin  mode)
      {
         // Set position 
         if (SetFilePointer(Handle, offset, NULL, (DWORD)mode) == INVALID_SET_FILE_POINTER)
            throw Win32Exception(HERE);
      }

      void  FileStream::SetLength(DWORD  length)
      {
         DWORD  position = GetPosition();

         // Seek to desired position and set EOF
         Seek(length, SeekOrigin::Begin);
         if (!SetEndOfFile(Handle))
            throw Win32Exception(HERE);

         // Restore original position
         Seek(position, SeekOrigin::Begin);
      }


      DWORD  FileStream::Read(BYTE* buffer, DWORD length)
      {
         REQUIRED(buffer);
         
         // Check access
         if (!CanRead())
            throw InvalidOperationException(HERE, ERR_NO_READ_ACCESS);

         DWORD debugPos = GetPosition();

         // Read bytes
         DWORD count = 0;
         if (!ReadFile(Handle, buffer, length, &count, NULL))
            throw Win32Exception(HERE);

         // Return bytes read
         return count;
      }


      DWORD  FileStream::Write(const BYTE* buffer, DWORD length)
      {
         REQUIRED(buffer);
         
         // Check access
         if (!CanWrite())
            throw InvalidOperationException(HERE, ERR_NO_WRITE_ACCESS);

         // Write bytes
         DWORD count = 0;
         if (!WriteFile(Handle, buffer, length, &count, NULL))
            throw Win32Exception(HERE);

         // Return bytes written
         return count;
      }


   }
}