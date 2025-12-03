#pragma once

class hkPackfileSectionHeader
{
public:
	char m_sectionTag[19];
	char m_nullByte;
	int m_absoluteDataStart;
	int m_localFixupsOffset;
	int m_globalFixupsOffset;
	int m_virtualFixupsOffset;
	int m_exportsOffset;
	int m_importsOffset;
	int m_endOffset;
	int m_pad[4];

	/// Size in bytes of data part
	int getDataSize() const { return m_localFixupsOffset; }

	/// Size in bytes of intra section pointer patches
	int getLocalSize() const { return m_globalFixupsOffset - m_localFixupsOffset; }

	/// Size in bytes of inter section pointer patches
	int getGlobalSize() const { return m_virtualFixupsOffset - m_globalFixupsOffset; }

	/// Size in bytes of finishing table.
	int getFinishSize() const { return m_exportsOffset - m_virtualFixupsOffset; }
	/// Size in bytes of exports table.

	int getExportsSize() const { return m_importsOffset - m_exportsOffset; }
	/// Size in bytes of imports table.

	int getImportsSize() const { return m_endOffset - m_importsOffset; }
};	