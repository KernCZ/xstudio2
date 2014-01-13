#include "stdafx.h"
#include "CommandSyntax.h"
#include <algorithm>

namespace Logic
{
   namespace Scripts
   {
      // -------------------------------- CONSTRUCTION --------------------------------

      /// <summary>Create command syntax from a declaration</summary>
      /// <param name="d">Declaration</param>
      CommandSyntax::CommandSyntax(const Declaration& d)
         : Group(d.Group), ID(d.ID), Versions(d.Versions), URL(d.URL), Text(d.Syntax), Type(d.Type), Parameters(d.Params)
      {
      }

      CommandSyntax::~CommandSyntax()
      {
      }

      // -------------------------------- STATIC METHODS -------------------------------

      // ------------------------------- PUBLIC METHODS -------------------------------

      /// <summary>Determines whether command is a keyword.</summary>
      /// <returns></returns>
      bool  CommandSyntax::IsKeyword() const
      {
         switch (ID)
         {
         case CMD_END:
         case CMD_ELSE:
         case CMD_BREAK:
         case CMD_END_SUB:
         case CMD_RETURN:
         case CMD_CONTINUE:
         case CMD_GOTO_SUB:
         case CMD_GOTO_LABEL:  return true;
         default:              return false;
         }
      }

		// ------------------------------ PROTECTED METHODS -----------------------------

		// ------------------------------- PRIVATE METHODS ------------------------------

      
   }
}

