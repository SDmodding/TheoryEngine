#pragma once

class hkStructureLayout
{
public:
	struct LayoutRules
	{
		u8 m_bytesInPointer;
		u8 m_littleEndian;
		u8 m_reusePaddingOptimization;
		u8 m_emptyBaseClassOptimization;
	};

	static inline const LayoutRules HostLayoutRules = { 8, 1, 0, 1 };

protected:
	LayoutRules m_rules;
};