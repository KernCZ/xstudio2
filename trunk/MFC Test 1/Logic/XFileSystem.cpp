#include "stdafx.h"
#include "XFileSystem.h"
#include <algorithm>
#include <functional>

using namespace Logic::IO;

namespace Logic
{
   namespace FileSystem
   {
      // -------------------------------- CONSTRUCTION --------------------------------

      /// <summary>Initializes a new instance of the <see cref="XFileSystem"/> class.</summary>
      XFileSystem::XFileSystem() 
      {
      }

      /// <summary>Releases the locks on the catalogs</summary>
      XFileSystem::~XFileSystem()
      {
         Catalogs.clear();
         Files.clear();
      }
      
		// ------------------------------- PUBLIC METHODS -------------------------------
      
      /// <summary>Searches for all files within a folder</summary>
      /// <param name="path">Full path of folder</param>
      /// <returns>Results collection</returns>
      /// <exception cref="Logic::IOException">I/O error occurred</exception>
      XFileSystem::FileCollection  XFileSystem::Browse(Path  folder) const
      {
         FileCollection results;

         // Ensure trailing backslash
         folder = folder.AppendBackslash();

         // Copy all files with exactly the same folder
         for (auto pair : Files)
            if (folder == pair.first.Folder)
               results.Add(pair.second);

         return results;
      }

      /// <summary>Queries whether file system contains a file</summary>
      /// <param name="path">Full path EXCLUDING extension</param>
      /// <returns></returns>
      bool  XFileSystem::Contains(Path  path) const
      {
         return Files.find(path) != Files.end();
      }

      /// <summary>Enumerates and locks the catalogs and their contents.  Any previous contents are cleared.</summary>
      /// <returns>Number of files found</returns>
      /// <exception cref="Logic::DirectoryNotFoundException">Folder does not exist</exception>
      /// <exception cref="Logic::NotSupportedException">Version is X2 or X-Rebirth</exception>
      /// <exception cref="Logic::FileNotFoundException">Catalog not found</exception>
      /// <exception cref="Logic::IOException">I/O error occurred</exception>
      DWORD  XFileSystem::Enumerate(Path folder, GameVersion version)
      {
         // Clear previous
         Catalogs.clear();
         Files.clear();

         // Ensure folder exists
         if (!folder.Exists())
            throw DirectoryNotFoundException(HERE, folder);

         // Ensure not X2/X4
         if (Version == GameVersion::Threat || version == GameVersion::Rebirth)
            throw NotSupportedException(HERE, L"X2 and Rebirth are not supported");

         // Ensure trailing backslash
         Folder = folder.AppendBackslash();

         // Enumerate catalogs/files
         EnumerateCatalogs();
         return EnumerateFiles();
      }

      /// <summary>Queries whether file system contains a file</summary>
      /// <param name="path">Full path EXCLUDING extension</param>
      /// <returns>File descriptor</returns>
      /// <exception cref="Logic::FileNotFoundException">File not found</exception>
      XFileInfo XFileSystem::Find(Path  path) const
      {
         auto it = Files.find(path);

         // Error: file not found
         if (it == Files.end())
            throw FileNotFoundException(HERE, path);

         // Return result 
         return it->second;
      }

		// ------------------------------ PROTECTED METHODS -----------------------------

		// ------------------------------- PRIVATE METHODS ------------------------------
      
      /// <summary>Enumerates and locks the catalogs</summary>
      /// <returns></returns>
      /// <exception cref="Logic::IOException">I/O error occurred</exception>
      DWORD  XFileSystem::EnumerateCatalogs()
      {
         const WCHAR *formats[2] = { L"%s%02i.cat", L"%saddon\\%02i.cat" },
                     *format;

         // Perform two passes for AlbionPrelude
         for (int pass = 0; pass < 2; pass++)
         {
            format = formats[pass];

            // Search for catalogs
            for (int i = 1; i < 99; i++)
            {
               Path path( StringResource::Format(format, (WCHAR*)Folder, i) );

               // Ensure exists
               if (!path.Exists() || !path.RenameExtension(L".dat").Exists())
                  break;
               
               // Add to FRONT of list  [Ensure highest priority at the front]
               Catalogs.Add(XCatalog(*this, path.RenameExtension(L".cat")));
            }

            if (Version != GameVersion::AlbionPrelude)
               break;
         }
         
         return Catalogs.Count;
      }

      /// <summary>Enumerates the files within the catalogs</summary>
      /// <returns></returns>
      /// <exception cref="Logic::FileNotFoundException">Catalog not found</exception>
      /// <exception cref="Logic::IOException">I/O error occurred</exception>
      DWORD  XFileSystem::EnumerateFiles()
      {
         // Iterate thru catalogs (Highest priority -> Lowest)
         for (const XCatalog& cat : Catalogs)
         {
            CatalogReader  reader(cat.GetReader());
            wstring        path;
            DWORD          size;

            // Iterate thru declarations + insert. Calculate running offset.  (Duplicate files are automatically discarded)
            for (DWORD offset = 0; reader.ReadDeclaration(path, size); offset += size)
               Files.Add( XFileInfo(*this, cat, path, size, offset) );
         }

         // Enumerate physical files
         EnumerateFolder(Folder);

         // Return count
         return Files.size();
      }

      /// <summary>Enumerates physical files within the folder</summary>
      /// <param name="folder">The folder path</param>
      void  XFileSystem::EnumerateFolder(Path  folder)
      {
         folder = folder.AppendBackslash();

         // Enumerate matching files on disc
         for (FileSearch fs(folder + L"*.*"); fs.HasResult(); fs.Next())
         {
            // Skip catalogs/datafiles
            if (fs.FileName == L"." || fs.FileName == L".." || fs.FullPath.HasExtension(L".cat") || fs.FullPath.HasExtension(L".dat"))
               continue;

            // Add files, recurse into folders
            if (!fs.IsDirectory())
               Files.Add(XFileInfo(fs.FullPath));
            else
               EnumerateFolder(fs.FullPath);
         }
         
      }

		// -------------------------------- NESTED CLASSES ------------------------------


   }
}
