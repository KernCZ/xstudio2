#pragma once

#include "Common.h"
#include "ScriptFile.h"
#include "ScriptCommand.h"
#include "RtfWriter.h"

namespace Logic
{
   namespace Scripts
   {
      class ScriptFile;
   }
   namespace IO
   {
         
      /// <summary></summary>
      class ScriptWriter
      {
         // --------------------- CONSTRUCTION ----------------------

      public:
         ScriptWriter() {}
         virtual ~ScriptWriter() {}

         // ------------------------ STATIC -------------------------

         // --------------------- PROPERTIES ------------------------
			
		   // ---------------------- ACCESSORS ------------------------			

		   // ----------------------- MUTATORS ------------------------

         virtual void  Write(ScriptFile& f) PURE;

		   // -------------------- REPRESENTATION ---------------------

      private:
      };

      /// <summary>Writes entire scripts in RTF</summary>
      class RtfScriptWriter
      {
         /// <summary>Writes script commands in RTF</summary>
         class RtfCommandWriter : public RtfWriter
         {
            const static COLORREF Black = RGB(0,0,0), Green = RGB(0,255,0), Blue = RGB(0,0,255), White = RGB(255,255,255), Cyan = RGB(150,220,220),
                                  Red = RGB(255,0,0), Yellow = RGB(255,255,0), Grey = RGB(128,128,128), Purple = RGB(255,0,255); 

            // --------------------- CONSTRUCTION ----------------------   
         public:
            RtfCommandWriter(const wstring& font, UINT size);

            // ------------------------ STATIC -------------------------

         private:
            static list<COLORREF>  GetColours();

            // --------------------- PROPERTIES ------------------------
			
		      // ---------------------- ACCESSORS ------------------------			

		      // ----------------------- MUTATORS ------------------------
         public:
            void  Write(const ScriptCommand& cmd, UINT  indent);

         private:
            void  Write(const ScriptCommand& cmd, const ScriptParameter& p);

            // -------------------- REPRESENTATION ---------------------
         };


         // --------------------- CONSTRUCTION ----------------------
      public:
         RtfScriptWriter(string&  out) : Output(out)
         {}

         // ------------------------ STATIC -------------------------

         // --------------------- PROPERTIES ------------------------
			
		   // ---------------------- ACCESSORS ------------------------			

		   // ----------------------- MUTATORS ------------------------

         void  Write(ScriptFile& f);

		   // -------------------- REPRESENTATION ---------------------
      private:
         string&  Output;
      };

   }
}

using namespace Logic::IO;
