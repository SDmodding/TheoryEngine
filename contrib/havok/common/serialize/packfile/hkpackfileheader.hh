#pragma once

class hkPackfileHeader
{
public:
	int m_magic[2];
	int m_userTag;
	int m_fileVersion;
	char m_layoutRules[4];
	int m_numSections;
	int m_contentsSectionIndex;
	int m_contentsSectionOffset;
	int m_contentsClassNameSectionIndex;
	int m_contentsClassNameSectionOffset;
	char m_contentsVersion[16];
	int m_flags;
	int m_pad[1];
};
