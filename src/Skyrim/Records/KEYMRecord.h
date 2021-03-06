/* ***** BEGIN LICENSE BLOCK *****
* Version: MPL 1.1/GPL 2.0/LGPL 2.1
*
* The KEYMents of this file are subject to the Mozilla Public License Version
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
* KEYMributor(s):
*  Jacob "Lojack" Lojewski
*
* Alternatively, the KEYMents of this file may be used under the terms of
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
#pragma once
#include "../../GenericRecord.h"
#include "../SkyrimChunks.h"
#include "../SkyrimCommon.h"

namespace Sk {

	struct KEYMDATA {
		uint32_t value;
		float weight;

		bool operator ==(const KEYMDATA &other) const {
			return value == other.value &&
				weight == other.weight;
		}
		bool operator !=(const KEYMDATA &other) const {
			return !(*this == other);
		}
	};

	class KEYMRecord : public TES5Record // Acoustic Space Record
	{
	public:
		StringRecord EDID; // Editor ID
		VMADRecord VMAD; // VM data
		ReqSubRecord<GENOBND> OBND; // Object Bounds
		LStringRecord FULL; // ambient
		MODEL MODL; //Model
		OptSimpleSubRecord<FORMID> YNAM; // Sound open name
		OptSimpleSubRecord<FORMID> ZNAM; // Sound close name
		OptCounted<OrderedPackedArray<FORMID>, uint32_t, REV32(KSIZ)> KWDA; // Keywords
		OptSubRecord<struct KEYMDATA> DATA;

		KEYMRecord(unsigned char *_recData = NULL);
		KEYMRecord(KEYMRecord *srcRecord);
		~KEYMRecord();

		bool VisitFormIDs(FormIDOp &op);

		uint32_t GetType();
		char * GetStrType();

		int32_t ParseRecord(unsigned char *buffer, unsigned char *end_buffer, bool CompressedOnDisk = false);
		int32_t Unload();
		int32_t WriteRecord(FileWriter &writer);
		char *GetEditorIDKey() { return EDID.value; }

		bool operator ==(const KEYMRecord &other) const;
		bool operator !=(const KEYMRecord &other) const;
		bool equals(Record *other);
	};
}