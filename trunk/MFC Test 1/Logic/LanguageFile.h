#pragma once
#include "Common.h"
#include "LanguagePage.h"

namespace Logic
{
   namespace Language
   {

      /// <summary>Represents an X3 language file, provides a collection of language pages</summary>
      class LanguageFile
      {
         // --------------------- CONSTRUCTION ----------------------

      public:
         LanguageFile() {};
         ~LanguageFile() {};

         // --------------------- PROPERTIES ------------------------
			
		   // ---------------------- ACCESSORS ------------------------

		   // ----------------------- MUTATORS ------------------------

		   // -------------------- REPRESENTATION ---------------------

         std::vector<LanguagePage> Pages;
         GameLanguage   Language;
      };

   }

}

using namespace Logic::Language;
