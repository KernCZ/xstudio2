#pragma once

#include "Common.h"
#include "CommandSyntax.h"
#include "ScriptParameter.h"
#include "ScriptToken.h"
#include "RtfWriter.h"

namespace Logic
{
   namespace Scripts
   {
      class ScriptFile;

      /// <summary></summary>
      enum BranchLogic   { None, NOP, If, While, SkipIf, Else, ElseIf, End, Break, Continue };
      const WCHAR* GetString(BranchLogic l);


      /// <summary></summary>
      class ScriptCommand
      {
         // --------------------- CONSTRUCTION ----------------------
      private:
         ScriptCommand();
      public:
         ScriptCommand(const wstring& text, const CommandSyntax& syntax, const TokenArray& params);
         ScriptCommand(const CommandSyntax& syntax, ParameterArray& params);
         ScriptCommand(const CommandSyntax& syntax, UINT  ref, ParameterArray& params);
         virtual ~ScriptCommand();

         //ScriptCommand& operator=(const ScriptCommand& r) = default;

         // ------------------------ STATIC -------------------------

         // --------------------- PROPERTIES ------------------------
			
         PROPERTY_GET(BranchLogic,Logic,GetLogic);

		   // ---------------------- ACCESSORS ------------------------			

         BranchLogic  GetLogic() const;

		   // ----------------------- MUTATORS ------------------------
      public:
         void  Translate(ScriptFile& f);

		   // -------------------- REPRESENTATION ---------------------
      public:
         static ScriptCommand  Unknown;

      public:
         CommandSyntax  Syntax;
         ParameterArray Parameters;
         wstring        Text;
         UINT           RefIndex;
      };

      typedef list<ScriptCommand>     CommandList;
   }
}

using namespace Logic::Scripts;
