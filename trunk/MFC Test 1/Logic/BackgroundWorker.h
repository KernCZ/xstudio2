#pragma once
#include "stdafx.h"
#include "Event.h"


namespace Logic
{
   namespace Threads
   {

      /// <summary>Feedback message</summary>
      #define WM_FEEDBACK     (WM_USER+1)

      // ------------------------ CLASSES ------------------------

      /// <summary></summary>
      enum class ProgressType : UINT { Operation, Info, Warning, Error, Succcess, Failure };

      /// <summary></summary>
      class WorkerProgress
      {
      public:
         WorkerProgress(ProgressType t, UINT indent, const wstring& sz) : Type(t), Text(sz), Indent(indent)
         {}

         const ProgressType  Type;
         const wstring       Text;
         const UINT          Indent;
      };

      /// <summary></summary>
      class WorkerData
      {
      public:
         WorkerData() : MainWnd(AfxGetMainWnd()), Aborted(false)
         {}
         virtual ~WorkerData()
         {}

         /// <summary>Command thread to stop</summary>
         void  Abort()
         {
            Aborted = true;
         }

         /// <summary>Inform main window of progress</summary>
         void  SendFeedback(ProgressType t, UINT indent, const wstring& sz)
         {
            MainWnd->PostMessageW(WM_FEEDBACK, NULL, (LPARAM)new WorkerProgress(t, indent, sz));
         }

      private:
         CWnd*  MainWnd;
         bool   Aborted;
      };

      /// <summary>Thread function</summary>
      typedef DWORD (WINAPI *ThreadProc)(WorkerData*);

      // ----------------- EVENTS AND DELEGATES ------------------

      typedef Event<WorkerProgress*>      FeedbackEvent;
      typedef FeedbackEvent::DelegatePtr  FeedbackHandler;

      // ------------------------ CLASSES ------------------------

      /// <summary></summary>
      class BackgroundWorker
      {
         // ------------------------ TYPES --------------------------
      private:
	  
         // --------------------- CONSTRUCTION ----------------------
      
      protected:
         BackgroundWorker(ThreadProc pfn) : Proc(pfn), Thread(NULL)
         {}
      public:
         virtual ~BackgroundWorker()
         {}
      
         // ------------------------ STATIC -------------------------
      
         // --------------------- PROPERTIES ------------------------
	  
         // ---------------------- ACCESSORS ------------------------			
      
         // ----------------------- MUTATORS ------------------------
      public:
         bool  Stop()
         {
            if (Thread == nullptr)
               throw InvalidOperationException(HERE, L"Thread not running");

            // Request thread stop
            Data->Abort();

            // Close handle
            CloseHandle(Thread);
            Thread = nullptr;
            return true;
         }

      protected:
         bool  Start(WorkerData* param)
         {
            REQUIRED(param);

            // Ensure not started
            if (Thread != nullptr)
               throw InvalidOperationException(HERE, L"Thread already running");

            // Launch thread
            Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, (void*)(Data=param), NULL, NULL);
            return Thread != NULL;
         }

         // -------------------- REPRESENTATION ---------------------
      
      private:
         ThreadProc   Proc;
         WorkerData*  Data;
         HANDLE       Thread;
      };
   

   }
}

using namespace Logic::Threads;
