#pragma once

namespace UFG
{
	void PostLoadSchemaCreateCheck(qPropertySet* property_set, const char* schema_type = 0, const char* skipparentcheck = 0);

#ifdef THEORY_IMPL

	void PostLoadSchemaCreateCheck(qPropertySet* property_set, const char* schema_type, const char* skipparentcheck)
	{
		if (!property_set) {
			return;
		}

		if (property_set->GetSchemaName() == gNullQSymbol || property_set->GetName() != property_set->GetSchemaName()) {
			return;
		}

		// TODO: Implement remaining stuff...
	}

#endif
}