#pragma once

#include "Common.h"
#include "TFile.hpp"

namespace Logic
{
   namespace Types
   {
      /// <summary></summary>
      class TFilenameReader
      {
      public:
         TFilenameReader(Path p) : Valid(true), Type(MainType::Unknown)
         {
            // Get lowercase filename without extension
            wstring name = GuiString(p.RemoveExtension().FileName).ToLower();

            // Identify file
            if (name == L"tlaser")
               Type = MainType::Laser;
            /*else if (name == L"tmissiles")
               Type = MainType::Missile;*/
            /*else if (name == L"tships")
               Type = MainType::Ship;*/
            else if (name == L"tdocks")
               Type = MainType::Dock;
            else if (name == L"tfactories")
               Type = MainType::Factory;
            else if (name == L"twaret")
               Type = MainType::TechWare;
            else if (name == L"twareb")
               Type = MainType::BioWare;
            else if (name == L"twaree")
               Type = MainType::EnergyWare;
            else if (name == L"twarem")
               Type = MainType::MineralWare;
            else if (name == L"twaren")
               Type = MainType::NaturalWare;
            else if (name == L"twaref")
               Type = MainType::FoodWare;
            else
               Valid = false;
         }

         bool      Valid;
         MainType  Type;
      };
      
      /// <summary></summary>
      class GameObject
      {
         // ------------------------ TYPES --------------------------
      private:
         //enum class KnownID : UINT { Undefined=0, UndefinedName=9999, UndefinedRace=34000, UnknownObject1=9001, UnknownObject2=9041, UnknownObject3=17206 };

         // --------------------- CONSTRUCTION ----------------------
      public:
         GameObject(UINT subtype, TObject* obj) : Type(obj->Type), SubType(subtype), ID(obj->id), Name(obj->FullName)
         {}
      private:
         GameObject(const GameObject& r, const wstring& txt) : Type(r.Type), SubType(r.SubType), ID(r.ID), Name(txt)
         {}

         // ---------------------- ACCESSORS ------------------------
      public:
         /// <summary>Appends an object id to name</summary>
         /// <param name="id">object id</param>
         /// <returns>New game object with id appended</returns>
         GameObject operator+(const wstring& id)
         {
            return GameObject(*this, Name+GuiString(L" (%s)", id));
         }

         // -------------------- REPRESENTATION ---------------------
      public:
         const MainType Type;
         const UINT     SubType;
         const wstring  ID, Name, Description;
      };

      /// <summary></summary>
      class GameObjectLibrary
      {
		   // ------------------------ TYPES --------------------------
      private:
	      /// <summary>Defines a {MainType,SubType} pair</summary>
         class ObjectID
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            ObjectID(MainType t, UINT subtype) : Type(t), SubType(subtype)
            {}

            // ---------------------- ACCESSORS ------------------------	
         public:
            bool operator==(const ObjectID& r) const {
               return Type == r.Type && SubType == r.SubType;
            }
            bool operator<(const ObjectID& r) const {
               return Type < r.Type || (Type == r.Type && SubType < r.SubType);
            }

            // -------------------- REPRESENTATION ---------------------
         public:
            const MainType Type;
            const UINT     SubType;
         };

         /// <summary>Collection of game objects sorted by SubType</summary>
         class ObjectCollection : public map<ObjectID, GameObject, less<ObjectID>>
         {
            // --------------------- CONSTRUCTION ----------------------
         public:
            // ---------------------- ACCESSORS ------------------------			

            /// <summary>Finds a game object by SubType</summary>
            /// <param name="type">Main type.</param>
            /// <param name="subtype">SubType.</param>
            /// <returns></returns>
            /// <exception cref="Logic::GameObjectNotFoundException">Object not found</exception>
            GameObject  Find(MainType type, UINT subtype) const
            {
               const_iterator it;
               // Lookup and return string
               if ((it = find(ObjectID(type, subtype))) != end())
                  return it->second;

               // Error: Not found
               throw GameObjectNotFoundException(HERE, (UINT)type, subtype);
            }

            // ----------------------- MUTATORS ------------------------
         public:
            /// <summary>Adds an object to the collection</summary>
            /// <param name="obj">The object</param>
            /// <returns>True if successful, false if MainType/SubType combination already present</returns>
            bool  Add(const GameObject& obj)
            {
               return insert(value_type(ObjectID(obj.Type, obj.SubType), obj)).second;
            }
         };

      public:
         /// <summary>Collection of game objects sorted by text</summary>
         class LookupCollection : public map<wstring, GameObject, less<wstring>>
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
            /// <exception cref="Logic::GameObjectNotFoundException">Object not found</exception>
            GameObject  Find(const wstring& sz) const
            {
               const_iterator it;
               // Lookup and return string
               if ((it = find(sz)) != end())
                  return it->second;

               // Error: Not found
               throw GameObjectNotFoundException(HERE, sz);
            }

            // ----------------------- MUTATORS ------------------------
         public:
            /// <summary>Adds an object</summary>
            /// <param name="obj">The object.</param>
            /// <returns>True if successful, false if key already present</returns>
            bool  Add(const GameObject& obj)
            {
               return insert(value_type(obj.Name, obj)).second;
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
         GameObjectLibrary();
         virtual ~GameObjectLibrary();
		 
		   NO_COPY(GameObjectLibrary);	// No copy semantics
		   NO_MOVE(GameObjectLibrary);	// No move semantics

         // ------------------------ STATIC -------------------------

         // --------------------- PROPERTIES ------------------------
			
         // ---------------------- ACCESSORS ------------------------			

         // ----------------------- MUTATORS ------------------------
      public:
         void  Clear();
         UINT  Enumerate(const XFileSystem& vfs, WorkerData* data);
         TObject*  Find(MainType main, UINT subtype);

      protected:
         UINT  PopulateObjects(WorkerData* data);

         // -------------------- REPRESENTATION ---------------------
      public:
         static GameObjectLibrary  Instance;

         const LookupCollection& Content;
      private:
         vector<TFilePtr>  Files;
         ObjectCollection  Objects;
         LookupCollection  Lookup;
      };
   
      // Access to Game object library singleton
      #define GameObjectLib  GameObjectLibrary::Instance
   }
}

using namespace Logic::Types;
