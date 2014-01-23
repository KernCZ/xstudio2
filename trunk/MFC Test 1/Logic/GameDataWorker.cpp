// GameDataWorker.cpp : implementation file
//

#include "stdafx.h"
#include "GameDataWorker.h"
#include "XFileSystem.h"
#include "SyntaxLibrary.h"
#include "StringLibrary.h"
#include "LegacySyntaxReader.h"


namespace Logic
{
   namespace Threads
   {

      // -------------------------------- CONSTRUCTION --------------------------------
   
      GameDataWorker::GameDataWorker() : BackgroundWorker((ThreadProc)ThreadMain)
      {
      }

      GameDataWorker::~GameDataWorker()
      {
      }

      // ------------------------------- STATIC METHODS -------------------------------

      DWORD WINAPI GameDataWorker::ThreadMain(GameDataWorkerData* data)
      {
         try
         {
            XFileSystem vfs;
            HRESULT  hr;

            // Init COM
            if (FAILED(hr=CoInitialize(NULL)))
               throw ComException(HERE, hr);

            // Feedback
            Console << ENDL << Colour::Cyan << L"Loading " << VersionString(data->Version) << L" game data from " << data->GameFolder.c_str() << ENDL;
            data->SendFeedback(ProgressType::Operation, GuiString(L"Loading %s game data from '%s'", VersionString(data->Version).c_str(), data->GameFolder.c_str()));

            // Build VFS. 
            vfs.Enumerate(data->GameFolder, data->Version, data);

            // Enumerate language files
            StringLib.Enumerate(vfs, GameLanguage::English, data);

            // Load legacy syntax file
            data->SendFeedback(ProgressType::Info, L"Loading legacy syntax file");
            SyntaxLib.Enumerate();

            // Cleanup
            data->SendFeedback(ProgressType::Succcess, L"Loaded game data successfully");
            CoUninitialize();
            return 0;
         }
         catch (ExceptionBase& e)
         {
            // Feedback
            data->SendFeedback(ProgressType::Error, GuiString(L"Unable to load game data : ") + e.Message);
            Console << e << ENDL;

            // Cleanup
            CoUninitialize();
            return 0;
         }
      }

      // ------------------------------- PUBLIC METHODS -------------------------------
   
      // ------------------------------ PROTECTED METHODS -----------------------------

      // ------------------------------- PRIVATE METHODS ------------------------------

   }
}




