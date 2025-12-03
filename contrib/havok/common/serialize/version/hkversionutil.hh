#pragma once

namespace hkVersionUtil
{
	const char* getCurrentVersion();

#ifdef HAVOK_IMPL

	const char* getCurrentVersion() { return "hk_2013.2.0-r1"; }

#endif;
}