#pragma once

namespace UFG
{
	enum qPropertyDepth
	{
		DEPTH_LOCAL,
		DEPTH_RECURSE
	};

	enum ePropertyType
	{
		UID_int8,
		UID_int16,
		UID_int32,
		UID_int64,
		UID_int128,
		UID_uint8,
		UID_uint16,
		UID_uint32,
		UID_uint64,
		UID_bool,
		UID_float,
		UID_double,
		UID_string,
		UID_rangedFloat,
		UID_rangedUint32,
		UID_rangedInt32,
		UID_weightedList,
		UID_qVector2,
		UID_qVector3,
		UID_qVector4,
		UID_qMatrix44,
		UID_qResHandle,
		UID_qSymbol,
		UID_qSymbolUC,
		UID_qWiseSymbol,
		UID_list,
		UID_propertyset,
		UID_TransRot,
		UID_qTransQuat,
		UID_Invalid,
		PROPERTY_SET_DATA_TYPE_COUNT
	};
}