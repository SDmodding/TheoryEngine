#pragma once

class hkStringPtr
{
public:
	const char* m_stringAndFlag;

	operator const char*() const { return m_stringAndFlag; }
};