#pragma once

#include "Common.h"
#include "ScriptToken.h"

namespace Logic
{
   namespace Scripts
   {
      namespace Compiler
      {
         
         /// <summary></summary>
         class CommandHash 
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            CommandHash(TokenIterator b, TokenIterator e) : Hash(GenerateHash(b,e)), Parameters(SeparateParams(b,e))
            {}

            // ------------------------ STATIC -------------------------
         private:
            static wstring  GenerateHash(TokenIterator begin, TokenIterator end) 
            {
               wstring h;
               // Assemble text into hash
               for (TokenIterator it = begin; it != end; ++it)
                  if (it->Type == TokenType::Text)
                     h += it->Text;
               return h;
            }

            static TokenArray  SeparateParams(TokenIterator begin, TokenIterator end) 
            {
               TokenArray arr;
               // Separate tokens
               for (TokenIterator it = begin; it != end; ++it)
                  if (it->Type != TokenType::Text)
                     arr.push_back(*it);
               return arr;
            }

            // ---------------------- ACCESSORS ------------------------	

            bool  operator==(const CommandHash& r) { return Hash == r.Hash; }
            bool  operator!=(const CommandHash& r) { return Hash != r.Hash; }

            // ----------------------- MUTATORS ------------------------

            // -------------------- REPRESENTATION ---------------------
         public:
            const wstring     Hash;
            const TokenArray  Parameters;
         };
      }
   }
}

using namespace Logic::Scripts::Compiler;
