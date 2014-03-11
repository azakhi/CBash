/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is CBash code.
 *
 * The Initial Developer of the Original Code is
 * Waruddar.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

 /**
    @file CBash.h
    @brief This file declares the C API functions.
*/

#pragma once
// CBash.h
//#define _UNICODE
#include "MacroDefinitions.h"
#include "Collection.h"

#define COMPILING_STATIC
#ifdef COMPILING_STATIC
    #define DLLEXTERN
#else
#ifdef COMPILING_CBASH
    #define DLLEXTERN extern "C" __declspec(dllexport)
#else
    #define DLLEXTERN extern "C" __declspec(dllimport)
#endif
#endif

//Exported Functions
/**************************//**
    @name Version Functions
******************************/
///@{

/**
    @brief Get CBash's minor version number.
    @returns Cbash's major version number.
*/
DLLEXTERN UINT32 GetVersionMajor();

/**
    @brief Get CBash's minor version number.
    @returns Cbash's major version number.
*/
DLLEXTERN UINT32 GetVersionMinor();

/**
    @brief Get CBash's revision (patch) version number.
    @returns Cbash's revision (patch) version number.
*/
DLLEXTERN UINT32 GetVersionRevision();

///@}
/**************************//**
    @name Logging action functions
******************************/
///@{

/**
    @brief Register a callback function for logging messages.
    @details Register a callback function for logging messages.
    @param _LoggingCallback A pointer to a callback function. If `NULL`, messages are printed to `stdout`. The callback function must take a message string argument, and returns a #SINT32 with value equal to the number of characters in the message, or `-1` if an error occurred.
*/
DLLEXTERN void RedirectMessages(SINT32 (*_LoggingCallback)(const STRING));

/**
    @brief Register a callback function for tracing function calls.
    @details Register a callback function for tracing function calls. This function is called by many functions if they encounter an error, and is passed the name of the function it is called from. This makes it potentially useful for debugging purposes.
    @param _RaiseCallback A pointer to a function that takes a string argument and returns nothing. If `NULL`, no function call tracing occurs.
*/
DLLEXTERN void AllowRaising(void (*_RaiseCallback)(const STRING));

///@}
/**************************//**
    @name Collection action functions
******************************/
///@{

/**
    @brief Create a plugin collection.
    @details Create a plugin collection. Collections are used to manage groups of mod plugins and their data in CBash.
    @param ModsPath Specifies the path to the folder containing the mod plugins that are to be added to the collection.
    @param CollectionType Specifies the type of game the collection is for. Valid game types are given by ::whichGameTypes.
    @returns A pointer to the newly-created collection object.
*/
DLLEXTERN Collection * CreateCollection(STRING const ModsPath, const UINT32 CollectionType);

/**
    @brief Delete a plugin collection.
    @details Delete a plugin collection. Deleting a collection frees all associated memory, invalidating associated pointers.
    @param CollectionID A pointer to the collection to be deleted.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 DeleteCollection(Collection *CollectionID);

/**
    @brief Loads a collection of plugins.
    @details Loads the records from the plugins in the given collection into memory, where their data can be accessed.
    @param CollectionID A pointer to the collection to load.
    @param _ProgressCallback A pointer to a function to use as a progress callback. If `NULL`, no progress is reported. The function arguments are the load order position of the plugin currently being loaded, the maximum load order position, and the plugin filename. The function returns a boolean that is currently ignored, but may in future be used to signal cancellation of loading by the client.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 LoadCollection(Collection *CollectionID, bool (*_ProgressCallback)(const UINT32, const UINT32, const STRING) = NULL);

/**
    @brief Unloads a collection of plugins.
    @details Unloads any records from the plugins in the given collection that have previously been loaded into memory, without deleting the collection.
    @param CollectionID A pointer to the collection to unload.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 UnloadCollection(Collection *CollectionID);

/**
    @brief Get the game type a collection was created for.
    @param CollectionID The collection to get the game type for.
    @returns The collection game type. Valid game types are given by ::whichGameTypes.
*/
DLLEXTERN SINT32 GetCollectionType(Collection *CollectionID);

/**
    @brief Unload all collections of plugins that have been created by CBash.
    @details Unloads all loaded collections from memory, without deleting them. Has the same effect as calling UnloadCollection() for each collection that has been created.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 UnloadAllCollections();

/**
    @brief Delete all plugin collections created by CBash.
    @details Has the same effect as calling UnloadCollection() for each collection that has been created.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 DeleteAllCollections();

///@}
/**************************//**
    @name Mod action functions
******************************/
///@{

/**
    @brief Add a plugin to a collection.
    @param CollectionID The collection to add the plugin to.
    @param ModName The filename of the plugin to add.
    @param ModFlagsField Flags that determine how the plugin is loaded and what can be edited once it has been loaded. These flags are given in ModFlags::modFlags.
    @returns A pointer to the plugin object added to the collection.
*/
DLLEXTERN ModFile * AddMod(Collection *CollectionID, STRING const ModName, const UINT32 ModFlagsField);

/**
    @brief Load a single plugin.
    @details Loads the records from the given plugin into memory.
    @param ModID A pointer to the plugin object to load.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 LoadMod(ModFile *ModID);

/**
    @brief Unload a single plugin.
    @details Unloads the records from the given plugin.
    @param ModID A pointer to the plugin object to unload.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 UnloadMod(ModFile *ModID);

/**
    @brief Remove unreferenced masters from a plugin.
    @details This function removes any entries in the given plugin's list of masters that aren't referenced in any of the plugin's records. Note that unreferenced masters are sometimes added to plugins to make explicit an otherwise implicit dependency.
    @param ModID A pointer to the plugin object for which unreferenced masters should be removed.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 CleanModMasters(ModFile *ModID);

/**
    @brief Save a single plugin's data to a plugin file.
    @param ModID A pointer to the plugin object to save.
    @param SaveFlagsField Flags that determine how the plugin is saved. These flags are given in SaveFlags::saveFlags.
    @param DestinationName The output plugin filename.
    @returns `0` on success, `-1` if an error occurred.
*/
DLLEXTERN SINT32 SaveMod(ModFile *ModID, const UINT32 SaveFlagsField, STRING const DestinationName);

///@}
/**************************//**
    @name Mod info functions
******************************/
///@{

/**
    @brief
    @details
    @param CollectionID
    @returns
*/
DLLEXTERN SINT32 GetAllNumMods(Collection *CollectionID);

/**
    @brief
    @details
    @param CollectionID
    @param ModIDs
    @returns
*/
DLLEXTERN SINT32 GetAllModIDs(Collection *CollectionID, MODIDARRAY ModIDs);

/**
    @brief
    @details
    @param CollectionID
    @returns
*/
DLLEXTERN SINT32 GetLoadOrderNumMods(Collection *CollectionID);

/**
    @brief
    @details
    @param CollectionID
    @param ModIDs
    @returns
*/
DLLEXTERN SINT32 GetLoadOrderModIDs(Collection *CollectionID, MODIDARRAY ModIDs);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN STRING GetFileNameByID(ModFile *ModID);

/**
    @brief
    @details
    @param CollectionID
    @param ModIndex
    @returns
*/
DLLEXTERN STRING GetFileNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN STRING GetModNameByID(ModFile *ModID);

/**
    @brief
    @details
    @param CollectionID
    @param ModIndex
    @returns
*/
DLLEXTERN STRING GetModNameByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);

/**
    @brief
    @details
    @param CollectionID
    @param ModName
    @returns
*/
DLLEXTERN ModFile * GetModIDByName(Collection *CollectionID, STRING const ModName);

/**
    @brief
    @details
    @param CollectionID
    @param ModIndex
    @returns
*/
DLLEXTERN ModFile * GetModIDByLoadOrder(Collection *CollectionID, const UINT32 ModIndex);

/**
    @brief
    @details
    @param CollectionID
    @param ModName
    @returns
*/
DLLEXTERN SINT32 GetModLoadOrderByName(Collection *CollectionID, STRING const ModName);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN SINT32 GetModLoadOrderByID(ModFile *ModID);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN ModFile * GetModIDByRecordID(Record *RecordID);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN Collection * GetCollectionIDByRecordID(Record *RecordID);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN Collection * GetCollectionIDByModID(ModFile *ModID);

/**
    @brief
    @details
    @param CollectionID
    @param ModID
    @param ModName
    @returns
*/
//DLLEXTERN SINT32 GetShortIDIndex(Collection *CollectionID, const SINT32 ModID, STRING const ModName);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN UINT32 IsModEmpty(ModFile *ModID);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN SINT32 GetModNumTypes(ModFile *ModID);

/**
    @brief
    @details
    @param ModID
    @param RecordTypes
    @returns
*/
DLLEXTERN SINT32 GetModTypes(ModFile *ModID, UINT32ARRAY RecordTypes);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN SINT32 GetModNumEmptyGRUPs(ModFile *ModID);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN SINT32 GetModNumOrphans(ModFile *ModID);

/**
    @brief
    @details
    @param ModID
    @param FormIDs
    @returns
*/
DLLEXTERN SINT32 GetModOrphansFormIDs(ModFile *ModID, FORMIDARRAY FormIDs);

///@}
/**************************//**
    @name FormID functions
******************************/
///@{

/**
    @brief
    @details
    @param RecordID
    @param FormID
    @param IsMGEFCode
    @returns
*/
DLLEXTERN STRING GetLongIDName(Record *RecordID, const UINT32 FormID, const bool IsMGEFCode);

/**
    @brief
    @details
    @param ModID
    @param ObjectID
    @param IsMGEFCode
    @returns
*/
DLLEXTERN UINT32 MakeShortFormID(ModFile *ModID, const UINT32 ObjectID, const bool IsMGEFCode);

///@}
/**************************//**
    @name Record action functions
******************************/
///@{

/**
    @brief
    @details
    @param ModID
    @param RecordType
    @param RecordFormID
    @param RecordEditorID
    @param ParentID
    @param CreateFlags
    @returns
*/
DLLEXTERN Record * CreateRecord(ModFile *ModID, const UINT32 RecordType, const FORMID RecordFormID, STRING const RecordEditorID, Record *ParentID, const UINT32 CreateFlags);

/**
    @brief
    @details
    @param RecordID
    @param DestModID
    @param DestParentID
    @param DestRecordFormID
    @param DestRecordEditorID
    @param CreateFlags
    @returns
*/
DLLEXTERN Record * CopyRecord(Record *RecordID, ModFile *DestModID, Record *DestParentID, const FORMID DestRecordFormID, STRING const DestRecordEditorID, const UINT32 CreateFlags);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN SINT32 UnloadRecord(Record *RecordID);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN SINT32 ResetRecord(Record *RecordID);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN SINT32 DeleteRecord(Record *RecordID);

///@}
/**************************//**
    @name Record info functions
******************************/
///@{

/**
    @brief
    @details
    @param ModID
    @param RecordFormID
    @param RecordEditorID
    @returns
*/
DLLEXTERN Record * GetRecordID(ModFile *ModID, const FORMID RecordFormID, STRING const RecordEditorID);

/**
    @brief
    @details
    @param ModID
    @param RecordType
    @returns
*/
DLLEXTERN SINT32 GetNumRecords(ModFile *ModID, const UINT32 RecordType);

/**
    @brief
    @details
    @param ModID
    @param RecordType
    @param RecordIDs
    @returns
*/
DLLEXTERN SINT32 GetRecordIDs(ModFile *ModID, const UINT32 RecordType, RECORDIDARRAY RecordIDs);

/**
    @brief
    @details
    @param RecordID
    @param GetExtendedConflicts
    @returns
*/
DLLEXTERN SINT32 IsRecordWinning(Record *RecordID, const bool GetExtendedConflicts);

/**
    @brief
    @details
    @param RecordID
    @param GetExtendedConflicts
    @returns
*/
DLLEXTERN SINT32 GetNumRecordConflicts(Record *RecordID, const bool GetExtendedConflicts);

/**
    @brief
    @details
    @param RecordID
    @param RecordIDs
    @param GetExtendedConflicts
    @returns
*/
DLLEXTERN SINT32 GetRecordConflicts(Record *RecordID, RECORDIDARRAY RecordIDs, const bool GetExtendedConflicts);

/**
    @brief
    @details
    @param RecordID
    @param RecordIDs
    @returns
*/
DLLEXTERN SINT32 GetRecordHistory(Record *RecordID, RECORDIDARRAY RecordIDs);

/**
    @brief
    @details
    @param ModID
    @returns
*/
DLLEXTERN SINT32 GetNumIdenticalToMasterRecords(ModFile *ModID);

/**
    @brief
    @details
    @param ModID
    @param RecordIDs
    @returns
*/
DLLEXTERN SINT32 GetIdenticalToMasterRecords(ModFile *ModID, RECORDIDARRAY RecordIDs);

/**
    @brief
    @details
    @param RecordID
    @returns
*/
DLLEXTERN SINT32 IsRecordsFormIDsInvalid(Record *RecordID);

///@}
/**************************//**
    @name Mod or Record action functions
******************************/
///@{

/**
    @brief
    @details
    @param ModID
    @param RecordID
    @param OldFormIDs
    @param NewFormIDs
    @param Changes
    @param ArraySize
    @returns
*/
DLLEXTERN SINT32 UpdateReferences(ModFile *ModID, Record *RecordID, FORMIDARRAY OldFormIDs, FORMIDARRAY NewFormIDs, UINT32ARRAY Changes, const UINT32 ArraySize);

///@}
/**************************//**
    @name Mod or Record info functions
******************************/
///@{

/**
    @brief
    @details
    @param CollectionID
    @param RecordID
    @returns
*/
DLLEXTERN SINT32 GetRecordUpdatedReferences(Collection *CollectionID, Record *RecordID);

///@}
/**************************//**
    @name Field action functions
******************************/
///@{

/**
    @brief
    @details
    @param RecordID
    @param FormID
    @param EditorID
    @returns
*/
DLLEXTERN SINT32 SetIDFields(Record *RecordID, const FORMID FormID, STRING const EditorID);

/**
    @brief
    @details
    @param RecordID
    @param FIELD_IDENTIFIERS
    @param FieldValue
    @param ArraySize
    @returns
*/
DLLEXTERN void   SetField(Record *RecordID, FIELD_IDENTIFIERS, void *FieldValue, const UINT32 ArraySize);

/**
    @brief
    @details
    @param RecordID
    @param FIELD_IDENTIFIERS
    @returns
*/
DLLEXTERN void   DeleteField(Record *RecordID, FIELD_IDENTIFIERS);

///@}
/**************************//**
    @name Field info functions
******************************/
///@{

/**
    @brief
    @details
    @param RecordID
    @param FIELD_IDENTIFIERS
    @param WhichAttribute
    @returns
*/
DLLEXTERN UINT32 GetFieldAttribute(Record *RecordID, FIELD_IDENTIFIERS, const UINT32 WhichAttribute);

/**
    @brief
    @details
    @param RecordID
    @param FIELD_IDENTIFIERS
    @param FieldValues
    @returns
*/
DLLEXTERN void * GetField(Record *RecordID, FIELD_IDENTIFIERS, void **FieldValues);

///@}
