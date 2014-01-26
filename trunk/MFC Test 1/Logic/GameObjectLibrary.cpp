#include "stdafx.h"
#include "GameObjectLibrary.h"
#include "XFileSystem.h"
#include "WorkerFeedback.h"
#include "TLaser.h"
#include "TShield.h"
#include "TFactory.h"
#include "TDock.h"
#include "TWare.h"

namespace Logic
{
   namespace Types
   {
      GameObjectLibrary  GameObjectLibrary::Instance;

      // -------------------------------- CONSTRUCTION --------------------------------

      GameObjectLibrary::GameObjectLibrary() : Content(Lookup)
      {
         Clear();
      }


      GameObjectLibrary::~GameObjectLibrary()
      {
      }

      // ------------------------------- STATIC METHODS -------------------------------

      // ------------------------------- PUBLIC METHODS -------------------------------

      /// <summary>Clears all loaded objects</summary>
      void GameObjectLibrary::Clear()
      {
         // Clear files
         Files.clear();
         Files.resize(1+(UINT)MainType::TechWare);

         // Clear objects
         Objects.clear();
         Lookup.clear();
      }

      /// <summary>Enumerates available type files</summary>
      /// <param name="vfs">The VFS.</param>
      /// <param name="data">Worker data.</param>
      /// <returns></returns>
      UINT GameObjectLibrary::Enumerate(const XFileSystem& vfs, WorkerData* data)
      {
         // Clear previous contents
         Clear();

         // Feedback
         data->SendFeedback(Colour::Cyan, ProgressType::Info, 1, L"Enumerating type definition files");

         // Enumerate type files
         for (XFileInfo& f : vfs.Browse(XFolder::Types))
         {
            try
            {
               // Identify file type and skip non-TFiles
               TFilenameReader fn(f.FullPath);
               if (!fn.Valid)
                  continue;

               // Feedback
               data->SendFeedback(ProgressType::Info, 2, GuiString(L"Reading type file '%s'...", (const WCHAR*)f.FullPath));
               Console << L"Reading type file: " << f.FullPath << L"...";

               // Create appropriate reader
               TFileReaderPtr reader;
               switch (fn.Type)
               {
               case MainType::Laser:       reader.reset(new TLaserReader(f.OpenRead()));   break;
               case MainType::Shield:      reader.reset(new TShieldReader(f.OpenRead()));  break;
               case MainType::Dock:        reader.reset(new TDockReader(f.OpenRead()));    break;
               case MainType::Factory:     reader.reset(new TFactoryReader(f.OpenRead())); break;
               case MainType::BioWare:     reader.reset(new TWareReader(f.OpenRead()));    break;
               case MainType::FoodWare:    reader.reset(new TWareReader(f.OpenRead()));    break;
               case MainType::EnergyWare:  reader.reset(new TWareReader(f.OpenRead()));    break;
               case MainType::NaturalWare: reader.reset(new TWareReader(f.OpenRead()));    break;
               case MainType::MineralWare: reader.reset(new TWareReader(f.OpenRead()));    break;
               case MainType::TechWare:    reader.reset(new TWareReader(f.OpenRead()));    break;
               }

               // Read/store file directly 
               Files[(UINT)fn.Type] = reader->ReadFile(fn.Type, vfs.GetVersion());

               Console << Colour::Green << L"Success" << ENDL;
            }
            catch (ExceptionBase& e)
            {
               data->SendFeedback(Colour::Red, ProgressType::Warning, 3, GuiString(L"Failed: ") + e.Message);
            }
         }

         // Populate object collection
         PopulateObjects(data);

         // Return object count
         return Content.size();
      }

      /// <summary>Finds an object</summary>
      /// <param name="main">The main type</param>
      /// <param name="subtype">The subtype</param>
      /// <returns></returns>
      /// <exception cref="Logic::ArgumentException">Matching type file not loaded</exception>
      /// <exception cref="Logic::IndexOutOfRangeException">Invalid index</exception>
      TObject*  GameObjectLibrary::Find(MainType main, UINT subtype)
      {
         // Ensure types loaded
         if (Files[(UINT)main] == nullptr)
            throw ArgumentException(HERE, L"main", GuiString(L"Object types for maintype %d not loaded", main));

         // Lookup object
         return Files[(UINT)main]->FindAt(subtype);
      }

      GameObjectArray  GameObjectLibrary::Query(const wstring& search)
      {
         GameObjectArray Results;
         
         // Linear search for partial substring
         for (auto& pair : Lookup)
            if (search.length() == 0 || pair.first.find(search.c_str()) != wstring::npos)
               Results.push_back(pair.second);

         return Results;
      }

      // ------------------------------ PROTECTED METHODS -----------------------------

      UINT  GameObjectLibrary::PopulateObjects(WorkerData* data)
      {
         typedef list<GameObject> ObjectList;
         typedef map<wstring, ObjectList> ConflictMap;
         
         ConflictMap Conflicts;
         ObjectList Input;

         // Feedback
         data->SendFeedback(Colour::Cyan, ProgressType::Info, 1, L"Generating game objects from type definition files");

         // Extract all objects from files
         for (auto& f : Files)
            if (f != nullptr)
               for (UINT id = 0; id < f->Count; ++id)
                  Input.push_back( GameObject(id, f->FindAt(id)) );

         // Populate lookup
         for (auto& obj : Input)
         {
            // Insert/Ensure unique
            if (!Lookup.Add(obj))
            {
               wstring key = obj.Name;
               
               // Conflict: Add both objects on first occurence, but only conflict on subsequent occurences
               if (Conflicts.find(key) == Conflicts.end())
                  Conflicts[key].push_back(Lookup.Find(key));
               
               Conflicts[key].push_back(obj);
            }
         }

         // Mangle names of conflicting objects
         for (auto& list : Conflicts)
            for (auto& obj : list.second)
               // Append ID, then attempt to insert
               if (!Lookup.Add(obj + obj.ID))
               {  
                  // Feedback: Failed
                  data->SendFeedback(ProgressType::Error, 2, GuiString(L"Unable to add game object '%s'", obj.Name.c_str()) );
                  Console << Colour::Red << L"Unable to add game object: " << Colour::White << obj.Name << ENDL;
               }

         // Populate objects collection from lookup (thereby preserving the name mangling)
         for (auto& pair : Lookup)
            Objects.Add(pair.second);

         // Return object count
         Console << "Generated " << Content.size() << " game objects" << ENDL;
         return Content.size();
      }

      // ------------------------------- PRIVATE METHODS ------------------------------
   
   }
}

