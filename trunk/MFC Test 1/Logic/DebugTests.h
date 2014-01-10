#pragma once

#include "Common.h"

namespace Library
{
   /// <summary></summary>
   class DebugTests
   {
      // --------------------- CONSTRUCTION ----------------------

   public:
      DebugTests() {};
      virtual ~DebugTests() {};

      // ------------------------ STATIC -------------------------

   public:
      static void  RunAll();

   private:
      static void  Test_LanguageFileReader();
      static void  Test_CatalogReader();
      static void  Test_FileSystem();
      static void  Test_GZip();

      // --------------------- PROPERTIES ------------------------
			
		// ---------------------- ACCESSORS ------------------------			

		// ----------------------- MUTATORS ------------------------

		// -------------------- REPRESENTATION ---------------------

   private:
   };
}

using namespace Library;
