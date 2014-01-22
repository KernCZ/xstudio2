#include "stdafx.h"
#include "SyntaxLibrary.h"
#include "CommandLexer.h"
#include "FileStream.h"
#include "LegacySyntaxReader.h"

namespace Logic
{
   namespace Scripts
   {
      SyntaxLibrary  SyntaxLibrary::Instance;

      // -------------------------------- CONSTRUCTION --------------------------------

      SyntaxLibrary::SyntaxLibrary()
      {
      }


      SyntaxLibrary::~SyntaxLibrary()
      {
      }

      // ------------------------------- STATIC METHODS -------------------------------

      // ------------------------------- PUBLIC METHODS -------------------------------
      
      /// <summary>Clears the library</summary>
      void  SyntaxLibrary::Clear()
      {
         Commands.clear();
         NameTree.Clear();
         Groups.clear();
      }


      /// <summary>Populates the library from the syntax files</summary>
      /// <returns>Commands enumerated</returns>
      /// <exception cref="Logic::FileFormatException">Missing syntax component</exception>
      /// <exception cref="Logic::InvalidValueException">Unknown command group / parameter type</exception>
      /// <exception cref="Logic::IOException">An I/O error occurred</exception>
      UINT  SyntaxLibrary::Enumerate()
      {
         const Path path(L"D:\\My Projects\\MFC Test 1\\MFC Test 1\\Command Syntax.txt");

         // Clear previous contents
         Clear();
         
         // Load legacy syntax file
         Console << ENDL << Colour::Cyan << L"Reading legacy syntax file: " << (const WCHAR*)path << ENDL;
         StreamPtr fs( new FileStream(path, FileMode::OpenExisting, FileAccess::Read) );

         // Merge contents
         Merge( LegacySyntaxReader(fs).ReadFile() );
         Console << Colour::Green << L"Legacy syntax loaded successfully" << ENDL;
         
         // Return commands read
         return Commands.size();
      }


      /// <summary>Finds syntax by ID</summary>
      /// <param name="id">command ID</param>
      /// <param name="ver">Game version</param>
      /// <returns></returns>
      /// <exception cref="Logic::SyntaxNotFoundException">Not found</exception>
      CommandSyntax  SyntaxLibrary::Find(UINT id, GameVersion ver) const
      { 
         // Search all syntax with matching ID for a compatible version
         for (auto it = Commands.find(id); it != Commands.end() && it->first == id; ++it)
            if (it->second.Versions & (UINT)ver)
               return it->second;

         // Not found
         throw SyntaxNotFoundException(HERE, id, ver);
      }

      /// <summary>Gets the groups collection</summary>
      /// <returns></returns>
      SyntaxLibrary::GroupCollection  SyntaxLibrary::GetGroups() const
      {
         return Groups;
      }

      /// <summary>Finds syntax by name</summary>
      /// <param name="pos">First token</param>
      /// <param name="end">End of tokens</param>
      /// <param name="v">Game version</param>
      /// <returns>Syntax if found, otherwise sentinel syntax</returns>
      CommandSyntax  SyntaxLibrary::Identify(TokenIterator& pos, const TokenIterator& end, GameVersion ver) const
      {
         return NameTree.Find(pos, end, ver);
      }

      /// <summary>Search for all syntax containing a given term</summary>
      /// <param name="str">Search term</param>
      /// <param name="ver">Game version</param>
      /// <returns>Array of matching Syntax</returns>
      SyntaxLibrary::ResultCollection  SyntaxLibrary::Query(const wstring& str, GameVersion ver) const
      {
         ResultCollection results;

         // Search commands
         for (const auto& pair : Commands)
         {
            const CommandSyntax& syntax = pair.second;

            // Check compatibility. Check search term (if any)
            if (syntax.Group != CommandGroup::HIDDEN 
             && syntax.IsCompatible(ver) 
             && (!str.length() || syntax.Text.find(str) != wstring::npos))
               results.push_back(&syntax);
         }

         return results;
      }

		// ------------------------------ PROTECTED METHODS -----------------------------

		// ------------------------------- PRIVATE METHODS ------------------------------
      
      /// <summary>Merges a syntax file into the library</summary>
      /// <param name="f">The file</param>
      void  SyntaxLibrary::Merge(SyntaxFile&& f)
      { 
         // Merge commands arrays
         for (auto pair : f.Commands)
            Commands.Add(pair.second);

         // Merge groups & lookup names
         for (auto pair : f.Groups)
            Groups[pair.second] = GuiString(IDS_FIRST_COMMAND_GROUP + (UINT)pair.second);

         // Insert into syntax tree
         for (auto& pair : Commands)
         {
            CommandSyntax& syntax = pair.second;
            
            try
            {
               // Skip commands that are manually matched by parser
               if (syntax.ID == CMD_COMMENT || syntax.ID == CMD_COMMAND_COMMENT || syntax.ID == CMD_EXPRESSION)
                  continue;

               // Lex syntax string
               CommandLexer lex(syntax.Text);
               TokenArray tokens;

               // Duplicate token array WITHOUT RefObj & RetVar
               for (const ScriptToken& t : lex.Tokens)
               {
                  if (t.Type != TokenType::Variable)
                     tokens.push_back(t);
                  else
                  {
                     // Lookup parameter by physical index marker
                     auto param = syntax.Parameters[t.Text[1]-48];

                     if (!param.IsRefObj() && !param.IsRetVar())
                        tokens.push_back(t);
                  }
               }

               // Insert command
               NameTree.Insert(syntax, tokens.begin(), tokens.end()); 
            }
            catch (ExceptionBase& e)
            {
               Console << Colour::Red << L"Unable to insert syntax: " << syntax.Text << L" : " << Colour::Yellow << e.Message << ENDL;
            }
         }
      }

   }
}

