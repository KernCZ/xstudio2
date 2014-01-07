#include "stdafx.h"
#include "GZipStream.h"

namespace Library
{
   namespace IO
   {
      // -------------------------------- CONSTRUCTION --------------------------------

      /// <summary>Creates a GZip stream using another stream as input</summary>
      /// <param name="src">The input stream</param>
      /// <param name="op">Whether to compress or decompress</param>
      GZipStream::GZipStream(StreamPtr  src, Operation  op) : StreamFacade(src), Mode(op)
      {
         // Clear structs
         ZeroMemory(&ZStream, sizeof(ZStream));
         ZeroMemory(&ZHeader, sizeof(ZHeader));

         // Init stream + extract header
         if (inflateInit2(&ZStream, WINDOW_SIZE+DETECT_HEADER) != Z_OK) 
            throw GZipException(HERE, ZStream.msg);

         // Allocate + set input buffer
         Input.reset(new byte[src->GetLength()]);
         ZStream.next_in = Input.get();
      }

      /// <summary>Closes the stream</summary>
      GZipStream::~GZipStream()
      {
         SafeClose();
      }

      // ------------------------------- PUBLIC METHODS -------------------------------

      /// <summary>Closes the stream.</summary>
      void  GZipStream::Close()
      {
         if (!IsClosed())
         {
            inflateEnd(&ZStream);
            StreamFacade::Close();
         }
      }

      /// <summary>Gets the uncompressed stream length.</summary>
      /// <returns></returns>
      DWORD  GZipStream::GetLength()
      {
         DWORD pos = StreamFacade::GetPosition(),
               size = 0;

         // Extract uncompressed length from last four bytes
         StreamFacade::Seek(-4, SeekOrigin::End);
         StreamFacade::Read((byte*)&size, 4);
         StreamFacade::Seek(pos, SeekOrigin::Begin);
         return size;
      }

      /// <summary>Gets the current position.</summary>
      /// <returns></returns>
      DWORD  GZipStream::GetPosition() const
      {
         return ZStream.total_out;
      }

      
      /// <summary>Closes the stream without throwing.</summary>
      void  GZipStream::SafeClose()
      {
         if (!IsClosed())
         {
            inflateEnd(&ZStream);
            StreamFacade::SafeClose();
         }
      }

      /// <summary>Not supported</summary>
      /// <param name="offset">The offset.</param>
      /// <param name="mode">The mode.</param>
      void  GZipStream::Seek(DWORD  offset, SeekOrigin  mode)
      {
         throw NotSupportedException(HERE, ERR_NO_SEEK_ACCESS);
      }

      /// <summary>Not supported</summary>
      /// <param name="offset">The offset.</param>
      /// <param name="mode">The mode.</param>
      void  GZipStream::SetLength(DWORD  length)
      {
         throw NotSupportedException(HERE, L"Resizing not allowed");
      }

      /// <summary>Reads/decompresses from the stream into the specified buffer.</summary>
      /// <param name="buffer">The destination buffer</param>
      /// <param name="length">The length of the buffer</param>
      /// <returns>Number of bytes read</returns>
      DWORD  GZipStream::Read(BYTE* output, DWORD length)
      {
         REQUIRED(output);

         // Ensure we can read
         if (!StreamFacade::CanRead())
            throw NotSupportedException(HERE, ERR_NO_READ_ACCESS);

         // Supply output buffer
         ZStream.next_out = output;
         ZStream.avail_out = length;

         // Re-Fill input buffer if necessary
         if (ZStream.avail_in == 0)
            ZStream.avail_in += StreamFacade::Read(ZStream.next_in, length);
         
         // Decompress
         switch (int res = inflate(&ZStream, Z_FINISH))
         {
         // Success/EOF: Return count decompressed
         case Z_STREAM_END:
         case Z_BUF_ERROR:
         case Z_OK:
            return ZStream.avail_out;

         // Error: throw
         default:
            throw GZipException(HERE, ZStream.msg);
         }
      }

      /// <summary>Writes/compresses the specified buffer to the stream</summary>
      /// <param name="buffer">The buffer.</param>
      /// <param name="length">The length of the buffer.</param>
      /// <returns>Number of bytes written</returns>
      DWORD  GZipStream::Write(const BYTE* buffer, DWORD length)
      {
         REQUIRED(buffer);

         throw NotImplementedException(HERE, L"GZip compression");
      }

      // ------------------------------ PROTECTED METHODS -----------------------------

		// ------------------------------- PRIVATE METHODS ------------------------------

      /// <summary>Determines whether the stream is closed.</summary>
      /// <returns></returns>
      bool   GZipStream::IsClosed() const
      {
         return ZStream.zalloc == Z_NULL;
      }

      // -------------------------------- NESTED CLASSES ------------------------------
   }
}
