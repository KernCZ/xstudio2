#pragma once
#include "Common.h"
#include "LanguageString.h"

using namespace Logic::Language;

namespace Logic
{
   namespace Language
   {
      /// <summary>Represents a collection of language strings</summary>
      class LanguagePage
      {
      public:
         /// <summary>Collection of language strings, sorted by ID</summary>
         class StringCollection : public map<UINT, LanguageString>
         {
         public:
            StringCollection(UINT page) : PageID(page) {}

            /// <summary>Adds a string to the collection, overwriting any with a lower version</summary>
            /// <param name="s">The string</param>
            /// <returns>True if successful, false otherwise</returns>
            bool  Add(LanguageString& s)
            {
               auto res = insert(value_type(s.ID, s));

               if (res.second)
                  return true;

               // Overwrite if version is greater
               if (!res.second && (UINT)s.Version > (UINT)(res.first->second).Version)
               {
                  res.first->second = s;
                  return true;
               }

               return false;
            }

            /// <summary>Query whether a string is present</summary>
            /// <param name="id">The string id</param>
            /// <returns></returns>
            bool  Contains(UINT  id) const
            {
               return find(id) != end();
            }

            /// <summary>Finds the specified string.</summary>
            /// <param name="id">The string id</param>
            /// <returns></returns>
            /// <exception cref="Logic::StringNotFoundException">String does not exist</exception>
            const LanguageString&  Find(UINT  id) const
            {
               const_iterator it;

               if ((it=find(id)) == end())
                  throw StringNotFoundException(HERE, PageID, id);

               return it->second;
            }

            /// <summary>Finds a string by index</summary>
            /// <param name="index">The index</param>
            /// <returns></returns>
            /// <exception cref="Logic::IndexOutOfRangeException">Index does not exist</exception>
            LanguageString&  FindByIndex(UINT index) 
            {
               UINT i = 0;

               // Linear search
               for (iterator it = begin(); it != end(); ++it)
                  if (index == i++)
                     return it->second;

               // Not found
               throw IndexOutOfRangeException(HERE, index, size());
            }

            /// <summary>Finds the specified string.</summary>
            /// <param name="id">The string id</param>
            /// <returns></returns>
            /// <exception cref="Logic::StringNotFoundException">String does not exist</exception>
            const LanguageString&  operator[](UINT  id) const
            {
               return Find(id);
            }

         private:
            UINT  PageID;
         };

         // --------------------- CONSTRUCTION ----------------------

      public:
         LanguagePage(UINT id, wstring title, wstring desc, bool voice);
         ~LanguagePage() {};

         // --------------------- PROPERTIES ------------------------
			
		   // ---------------------- ACCESSORS ------------------------

         /// <summary>Query whether a string is present</summary>
         /// <param name="id">The string id</param>
         /// <returns></returns>
         bool Contains(UINT  id) const                      { return Strings.Contains(id); }

         /// <summary>Finds the specified string.</summary>
         /// <param name="id">The string id</param>
         /// <returns></returns>
         /// <exception cref="Logic::StringNotFoundException">String does not exist</exception>
         const LanguageString&  Find(UINT  id) const        { return Strings.Find(id);     }

         /// <summary>Finds the specified string.</summary>
         /// <param name="id">The string id</param>
         /// <returns></returns>
         /// <exception cref="Logic::StringNotFoundException">String does not exist</exception>
         const LanguageString&  operator[](UINT  id) const  { return Strings.Find(id);     }

		   // ----------------------- MUTATORS ------------------------

		   // -------------------- REPRESENTATION ---------------------

         UINT             ID;
         wstring          Title,
                          Description;
         bool             Voiced;
         
         StringCollection Strings;

      };
   }
}