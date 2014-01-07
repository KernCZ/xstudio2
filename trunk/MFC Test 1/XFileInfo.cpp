#include "stdafx.h"
#include "XFileSystem.h"

namespace Library
{
   namespace FileSystem
   {

      XFileInfo::XFileInfo(Path p) 
         : Source(FileSource::Physical), FullPath(p), FileSystem(nullptr), Catalog(nullptr), Offset(0) 
      {
      }

      XFileInfo::XFileInfo(XFileSystem& vfs, XCatalog& cat, Path subPath, DWORD pos) 
         : Source(FileSource::Catalog), FullPath(vfs.GetFolder()+subPath), FileSystem(&vfs), Catalog(&cat), Offset(pos) 
      {
      }

      XFileInfo::~XFileInfo() 
      {
      }
   }
}
