#pragma once

#include "Common.h"
#include "WorkerFeedback.h"
#include "LanguageFile.h"
#include "StringLibrary.h"

namespace Logic
{
   namespace Scripts
   {
      /// <summary>Represents a script object</summary>
      class ScriptObject : public LanguageString
      {
         // --------------------- CONSTRUCTION ----------------------
      public:
         ScriptObject(KnownPage p, const LanguageString& s) : Page(p), LanguageString(s)
         {}
      private:
         ScriptObject(const ScriptObject& obj, const wstring& newText) : Page(obj.Page), LanguageString(obj.ID, newText, obj.Version)
         {}

         // ---------------------- ACCESSORS ------------------------	

         // ----------------------- MUTATORS ------------------------
      public:
         /// <summary>Appends an object ID</summary>
         /// <param name="id">The id</param>
         /// <returns>New script object</returns>
         ScriptObject operator+(UINT id)
         {
            return ScriptObject(*this, Text+GuiString(L" (%d)", id));
         }

         /// <summary>Appends a game version acronym</summary>
         /// <param name="v">The version</param>
         /// <returns>New script object</returns>
         ScriptObject operator+(GameVersion v)
         {
            return ScriptObject(*this, Text+GuiString(L" (%s)", VersionString(v,true).c_str()));
         }

         /// <summary>Appends an page/category acronym</summary>
         /// <param name="p">The page</param>
         /// <returns>New script object</returns>
         /// <exception cref="Logic::ArgumentException">Incompatible page</exception>
         ScriptObject operator+(KnownPage p)
         {
            const wchar* acronym = nullptr;
            switch (p)
            {
            case KnownPage::OBJECT_COMMANDS:
            case KnownPage::WING_COMMANDS:  
               return ScriptObject(*this, Text+L" (CMD)");
            case KnownPage::CONSTANTS:
               return ScriptObject(*this, Text+L" (CON)");
            case KnownPage::DATA_TYPES:
               return ScriptObject(*this, Text+L" (DT)");
            case KnownPage::FLIGHT_RETURNS:
               return ScriptObject(*this, Text+L" (FLRET)");
            case KnownPage::OBJECT_CLASSES:
               return ScriptObject(*this, Text+L" (OC)");
            case KnownPage::PARAMETER_TYPES:
               return ScriptObject(*this, Text+L" (TYPE)");               
            case KnownPage::STATION_SERIALS:
               return ScriptObject(*this, Text+L" (SS)");
            case KnownPage::TRANSPORT_CLASSES:
               return ScriptObject(*this, Text+L" (TC)");
            case KnownPage::SECTORS:
            {
               int x = _ttoi(Text.substr(3,2).c_str());
               int y = _ttoi(Text.substr(5,2).c_str());
               return ScriptObject(*this, Text+GuiString(L" (%d,%d)", x, y));
            }
            default: throw ArgumentException(HERE, L"operand", L"Incompatible language page ID");
            }
         }

         // -------------------- REPRESENTATION ---------------------
      public:
         const KnownPage Page;
      };

      /// <summary>Provides access to script objects</summary>
      class ScriptObjectLibrary
      {
		   // ------------------------ TYPES --------------------------
      private:
         /// <summary>Defines a {KnownPage,ID} pair</summary>
         class ObjectID
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            ObjectID(KnownPage p, UINT id) : Page(p), ID(id)
            {}

            // ---------------------- ACCESSORS ------------------------	
         public:
            bool operator==(const ObjectID& r) const {
               return Page == r.Page && ID == r.ID;
            }
            bool operator<(const ObjectID& r) const {
               return Page < r.Page || (Page == r.Page && ID < r.ID);
            }

            // -------------------- REPRESENTATION ---------------------
         public:
            const KnownPage Page;
            const UINT      ID;
         };

         /// <summary>Collection of Script objects sorted by ID</summary>
         class ObjectCollection : public map<ObjectID, ScriptObject, less<ObjectID>>
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            // ---------------------- ACCESSORS ------------------------			

            /// <summary>Finds a script object by ID</summary>
            /// <param name="page">The page.</param>
            /// <param name="id">The ID.</param>
            /// <returns></returns>
            /// <exception cref="Logic::StringNotFoundException">Object not found</exception>
            ScriptObject  Find(KnownPage page, UINT id) const
            {
               const_iterator it;
               // Lookup and return string
               if ((it = find(ObjectID(page, id))) != end())
                  return it->second;

               // Error: Not found
               throw StringNotFoundException(HERE, (UINT)page, id);
            }

            // ----------------------- MUTATORS ------------------------
         public:
            /// <summary>Creates and adds a new object</summary>
            /// <param name="page">The page</param>
            /// <param name="str">The string to create the object from</param>
            /// <returns>True if successful, false if ID already present</returns>
            bool  Add(KnownPage page, const LanguageString& str)
            {
               switch (page)
               {
               // Exclude all
               default:
                  return false;

               // Include all
               case KnownPage::DATA_TYPES:
               case KnownPage::FLIGHT_RETURNS:
               case KnownPage::OBJECT_CLASSES:
               case KnownPage::OBJECT_COMMANDS:
               case KnownPage::PARAMETER_TYPES:
               case KnownPage::WING_COMMANDS:
                  break;

               // Exclude 'old' [THIS] from lookup tree
               case KnownPage::CONSTANTS:
                  if (str.ID == 0)
                     return false;
                  break;

               // Skip 6 digit sector names with IDs 20xxx and 30xxx
               case KnownPage::SECTORS:
                  if (str.ID < 1020000)
                     return false;
                  break;

               // Include names, exclude initials
               case KnownPage::RACES:
                  if (str.ID >= 200)
                     return false;
                  break;

               // Exclude the S,M,L,XL,XXL ship/station name modifiers
               case KnownPage::STATION_SERIALS:
                  if (str.ID >= 500)
                     return false;
                  break;

               // Only include the abbreviated versions
               case KnownPage::TRANSPORT_CLASSES:
                  if (str.ID >= 10)
                     return false;
                  break;

               // FRIEND/FOE/NEUTRAL
               case KnownPage::RELATIONS:
                  switch (str.ID)
                  {
                  case 1102422:
                  case 1102423:
                  case 1102424: 
                     break;
                  default:  
                     return false;
                  }
               }

               // Insert
               return insert(value_type(ObjectID(page,str.ID), ScriptObject(page, str))).second;
            }
         };
      
      public:
         /// <summary>Collection of script objects sorted by text</summary>
         class LookupCollection : public map<wstring, ScriptObject, less<wstring>>
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            // ---------------------- ACCESSORS ------------------------			

            /// <summary>Check whether an object is present</summary>
            /// <param name="sz">The text</param>
            /// <returns></returns>
            bool  Contains(const wstring& sz) const
            {
               return find(sz) != end();
            }

            /// <summary>Finds an object by text<summary>
            /// <param name="sz">The text</param>
            /// <returns>Object</returns>
            /// <exception cref="Logic::ScriptObjectNotFoundException">Object not found</exception>
            ScriptObject  Find(const wstring& sz) const
            {
               const_iterator it;
               // Lookup and return string
               if ((it = find(sz)) != end())
                  return it->second;

               // Error: Not found
               throw ScriptObjectNotFoundException(HERE, sz);
            }

            // ----------------------- MUTATORS ------------------------
         public:
            /// <summary>Adds an object</summary>
            /// <param name="obj">The object.</param>
            /// <returns>True if successful, false if key already present</returns>
            bool  Add(const ScriptObject& obj)
            {
               return insert(value_type(obj.Text, obj)).second;
            }

            /// <summary>Creates a new script object and adds to the collection</summary>
            /// <param name="page">The page.</param>
            /// <param name="str">The language string to create object from</param>
            /// <returns>True if successful, false if key already present</returns>
            bool  Add(KnownPage page, const LanguageString& str)
            {
               return insert(value_type(str.Text, ScriptObject(page, str))).second;
            }

            /// <summary>Removes an object from the collection</summary>
            /// <param name="sz">The text</param>
            void  Remove(const wstring& sz)
            {
               erase(sz);
            }
         };
	  
         // --------------------- CONSTRUCTION ----------------------
      public:
         ScriptObjectLibrary();
         virtual ~ScriptObjectLibrary();
		 
		   DEFAULT_COPY(ScriptObjectLibrary);	// Default copy semantics
		   DEFAULT_MOVE(ScriptObjectLibrary);	// Default move semantics

         // ------------------------ STATIC -------------------------

         // --------------------- PROPERTIES ------------------------
			
         PROPERTY_GET(UINT,Count,GetCount);

         // ---------------------- ACCESSORS ------------------------			
      public:
         UINT  GetCount() const;

         // ----------------------- MUTATORS ------------------------
      public:
         void  Clear();
         UINT  Enumerate(WorkerData* data);

      protected:
         bool  InsertConflicts(ScriptObject a, ScriptObject b);
         bool  MangleConflicts(ScriptObject a, ScriptObject b);
         UINT  PopulateLookup(WorkerData* data);
         UINT  PopulateObjects(WorkerData* data);

         // -------------------- REPRESENTATION ---------------------
      public:
         static ScriptObjectLibrary  Instance;
         
         const LookupCollection& Content;
         
      private:
         ObjectCollection  Objects;
         LookupCollection  Lookup;
      };
   }

   /// <summary>Access to the script object library singleton</summary>
   #define ScriptObjectLib  ScriptObjectLibrary::Instance
}

using namespace Logic::Scripts;
