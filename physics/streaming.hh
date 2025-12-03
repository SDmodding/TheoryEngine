#pragma once

namespace UFG
{
	class NativePackfileUtils
	{
	public:
		struct Fixup
		{
			void* addr;
			u32 val;
		};

		struct Fixups
		{
			hkArray<Fixup> nonZeroFixups;
			hkArray<void*> zeroFixups;
		};

		static hkResult validatePackfileHeader(hkPackfileHeader* packfileData);
		static void* loadInPlace(void* ptr, int dataSize, Fixups* fixups);
	};

#ifdef THEORY_IMPL

	hkResult NativePackfileUtils::validatePackfileHeader(hkPackfileHeader* packfileData)
	{
		if (packfileData->m_magic[0] == 0x57E0E057 && packfileData->m_magic[1] == 0x10C0C010
			&& packfileData->m_layoutRules[0] == hkStructureLayout::HostLayoutRules.m_bytesInPointer
			&& packfileData->m_layoutRules[1] == hkStructureLayout::HostLayoutRules.m_littleEndian
			&& packfileData->m_layoutRules[2] == hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization
			&& packfileData->m_layoutRules[3] == hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization
			&& qStringCompare(packfileData->m_contentsVersion, hkVersionUtil::getCurrentVersion()) == 0)
		{
			return HK_SUCCESS;
		}

		return HK_FAILURE;
	}

	void* NativePackfileUtils::loadInPlace(void* ptr, int dataSize, Fixups* fixups)
	{
		auto data = static_cast<hkPackfileHeader*>(ptr);
		if (validatePackfileHeader(data) == HK_FAILURE) {
			return 0;
		}

		auto sections = reinterpret_cast<hkPackfileSectionHeader*>(&data[1]);
		int classOffset = -1;

		for (int i = 0; data->m_numSections > i; ++i)
		{
			auto section = &sections[i];

			if (i == data->m_contentsSectionIndex) {
				classOffset = section->m_absoluteDataStart + data->m_contentsSectionOffset;
			}

			// applyLocalFixups

			u8* buf = &reinterpret_cast<u8*>(data)[section->m_absoluteDataStart];
			int* offset = reinterpret_cast<int*>(&buf[section->m_localFixupsOffset]);

			for (int s = 0; (section->getLocalSize() / 4) > s; s += 2)
			{
				if (offset[0] != -1) {
					*reinterpret_cast<void**>(&buf[offset[0]]) = &buf[offset[1]];
				}

				offset = &offset[2];
			}
		}

		for (int i = 0; data->m_numSections > i; ++i)
		{
			// TODO: GlobalFixups
		}

		for (int i = 0; data->m_numSections > i; ++i)
		{
			// TODO: VirtualFixups
		}

		return reinterpret_cast<void*>(reinterpret_cast<uptr>(data) + classOffset);
	}

#endif
}