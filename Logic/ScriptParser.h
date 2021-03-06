#pragma once

#include "CommandTree.h"
#include <algorithm>

namespace Testing
{
   class LogicTests;
}

namespace Logic
{
   namespace Scripts
   {
      namespace Compiler
      {
         
         /// <summary>Generates a parse tree from MSCI scripts</summary>
         class LogicExport ScriptParser
         {
            friend class ::Testing::LogicTests;

            // ------------------------ TYPES --------------------------
         protected:
            /// <summary>Special lexer for lexing command comments as ordinary commands</summary>
            class CommentLexer : public CommandLexer
            {
            public:
               /// <summary>Lexes a command comment as a command</summary>
               /// <param name="input">Line of command text</param>
               CommentLexer(const wstring& input) : CommandLexer(StripComment(input), true)
               {}

            private:
               /// <summary>Strips the comment operator from a command</summary>
               /// <param name="in">command text</param>
               /// <returns></returns>
               wstring StripComment(const wstring& in)
               {
                  return in.substr(in.find('*')+1);
               }
            };

            // --------------------- CONSTRUCTION ----------------------
         public:
            ScriptParser(ScriptFile& file, const LineArray& lines, GameVersion  v);
            virtual ~ScriptParser();

            NO_COPY(ScriptParser);	// Cannot copy semantics
            NO_MOVE(ScriptParser);	// Cannot move semantics

            // ------------------------ STATIC -------------------------
         public:
            static CommandSyntaxRef  Identify(ScriptFile script, const wstring& line);
            static ScriptCommand     Parse(ScriptFile script, const wstring& line);
            static CommandNodePtr    Generate(const wstring& line, GameVersion v);

            // --------------------- PROPERTIES ------------------------
         public:
            PROPERTY_GET(bool,Empty,IsEmpty);
            PROPERTY_GET(bool,Successful,IsSuccessful);

         protected:
            PROPERTY_GET(UINT,LineNumber,GetLineNumber);
            PROPERTY_GET(CommandNodePtr,FirstCommand,GetFirstCommand);

            // ---------------------- ACCESSORS ------------------------			
         public:
            void            FindAll(const wstring& name, SymbolType type, SymbolList& results) const;
            bool            IsEmpty() const;
            bool            IsSuccessful() const;
            CommandNodeList ToList() const;

         protected:
            CommandNodePtr GetFirstCommand() const;
            UINT           GetLineNumber() const;
            ErrorToken     MakeError(const wstring& msg, const CommandLexer& lex) const;
            ErrorToken     MakeError(const wstring& msg, const TokenIterator& tok) const;

            bool  MatchArrayIndex(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchAssignment(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchConditional(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchReferenceObject(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchScriptArgument(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchVArgument(const CommandLexer& lex, TokenIterator& pos) const;
            bool  MatchComment(const CommandLexer& lex) const;
            bool  MatchCommand(const CommandLexer& lex) const;
            bool  MatchExpression(const CommandLexer& lex) const;

            // ----------------------- MUTATORS ------------------------
         public:
            void  Compile();
            void  Print();

         protected:
            CommandNodePtr Advance();
            void           ParseRoot();
            void           ParseIfElse(CommandNodePtr& If);
            void           ParseSkipIf(CommandNodePtr& SkipIf);
            
            TokenIterator  ReadAssignment(const CommandLexer& lex, TokenIterator& pos);
            Conditional    ReadConditional(const CommandLexer& lex, TokenIterator& pos);
            TokenIterator  ReadReferenceObject(const CommandLexer& lex, TokenIterator& pos);

            CommandNodePtr ReadComment(const CommandLexer& lex);
            CommandNodePtr ReadCommand(const CommandLexer& lex, bool comment);
            CommandNodePtr ReadExpression(const CommandLexer& lex, bool comment);
            CommandNodePtr ReadLine();

            // -------------------- REPRESENTATION ---------------------
         public:
            ErrorArray     Errors;     // Compilation errors
            ScriptFile&    Script;     // Script

         protected:
            const LineArray&  Input;         // Input text
            const GameVersion Version;       // Script version

            CommandTree     Tree;            // Parse tree
            LineIterator    CurrentLine;     // Line being parsed
            CommandNodePtr  CurrentNode;     // Most recently parsed node
            ErrorArray      CommentErrors;   // Separate error queue used for trying to parse command comments
         };
      }
   }
}

using namespace Logic::Scripts::Compiler;
