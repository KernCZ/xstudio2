#pragma once
#include "../Logic/Common.h"
#include "../Logic/ScriptParser.h"
#include "../Logic/ScriptFileReader.h"

//namespace Logic
//{
//   namespace Scripts
//   {
//      class ScriptVariable;
//   }
//}

namespace Testing
{
   namespace Scripts
   {
      /// <summary>Occurs when a validation test fails</summary>
      class ValidationException : public ExceptionBase
      {
      public:
         /// <summary>Create an ValidationException</summary>
         /// <param name="src">Location of throw</param>
         /// <param name="msg">Message</param>
         ValidationException(wstring  src, wstring  msg) 
            : ExceptionBase(src, wstring(L"Validation failed: ") + msg)
         {}
      };

      /// <summary>Validates the code and text produced by the script compiler</summary>
      class ScriptValidator
      {
         // ------------------------ TYPES --------------------------
      private:

         // --------------------- CONSTRUCTION ----------------------

      public:
         ScriptValidator(Path file);
         virtual ~ScriptValidator();

         DEFAULT_COPY(ScriptValidator);	// Default copy semantics
         DEFAULT_MOVE(ScriptValidator);	// Default move semantics

         // ------------------------ STATIC -------------------------
      private:
         static void       CompileScript(ScriptFile& s, Path truePath);
         static LineArray  GetAllLines(const CommandList& commands);
         static ScriptFile ReadScript(Path truePath, Path displayPath);

         // --------------------- PROPERTIES ------------------------

         // ---------------------- ACCESSORS ------------------------			

         // ----------------------- MUTATORS ------------------------
      public:
         bool  Validate();

         // -------------------- REPRESENTATION ---------------------

      private:
         Path  FullPath;
      };

      /// <summary>Validates the script text generated by the script translator</summary>
      class ScriptTextValidator
      {
         // ------------------------ TYPES --------------------------
      private:

         // --------------------- CONSTRUCTION ----------------------
      public:

         // ------------------------ STATIC -------------------------
      public:
         static bool  Compare(const LineArray& in, const LineArray& out);
         static bool  Compare(const ScriptFile& in, const ScriptFile& out);

      private:
         static bool  Compare(const ScriptVariable& in, const ScriptVariable& out);
         static bool  Compare(UINT line, const ScriptCommand& in, const ScriptCommand& out);
         static bool  Compare(UINT line, UINT param, const ScriptParameter& in, const ScriptParameter& out);

         static ValidationException  TextMismatch(const GuiString& src, const GuiString& prop, const GuiString& a, const GuiString& b);
         static ValidationException  TextMismatch(const GuiString& src, const GuiString& prop, int a, int b);

         // --------------------- PROPERTIES ------------------------

         // ---------------------- ACCESSORS ------------------------			

         // ----------------------- MUTATORS ------------------------
      
         // -------------------- REPRESENTATION ---------------------

      };

      /// <summary>Validates the script XML generated by the script compiler</summary>
      class ScriptCodeValidator
      {
         // ------------------------ TYPES --------------------------
      private:
         class ScriptReader : public SourceValueReader
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            ScriptReader(StreamPtr in) : SourceValueReader(in)
            {
               LoadDocument();   // Parse document on construction
            }

            // --------------------- PROPERTIES ------------------------

            // ---------------------- ACCESSORS ------------------------			

            // ----------------------- MUTATORS ------------------------
         };

         // --------------------- CONSTRUCTION ----------------------
      public:
         ScriptCodeValidator(StreamPtr orig, StreamPtr copy);

         // ------------------------ STATIC -------------------------
      private:
         static ValidationException  CodeMismatch(const GuiString& src, const GuiString& prop, const ParameterValue& a, const ParameterValue& b);
         static ValidationException  CodeMismatch(const GuiString& src, const GuiString& prop, const GuiString& a, const GuiString& b);
         static ValidationException  CodeMismatch(const GuiString& src, const GuiString& prop, int a, int b);

         // --------------------- PROPERTIES ------------------------

         // ---------------------- ACCESSORS ------------------------			

         // ----------------------- MUTATORS ------------------------
      public:
         bool  Compare();

      private:
         bool  Compare(XmlNodePtr parent_in, XmlNodePtr parent_out, UINT index, const wchar* help);
         bool  CompareSize(XmlNodePtr parent_in, XmlNodePtr parent_out, UINT index, const wchar* help);

         // -------------------- REPRESENTATION ---------------------
      private:
         ScriptReader  In, Out;
      };

   }
}

using namespace Testing::Scripts;
