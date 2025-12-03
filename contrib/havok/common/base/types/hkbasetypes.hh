#pragma once

/// a simple success/failure enum.
enum hkResultEnum
{
	HK_SUCCESS = 0,
	HK_FAILURE = 1
};

struct hkResult
{
	hkResult() {}
	hkResult(hkResultEnum b) : m_enum(b) {}

	hkResult& operator=(hkResultEnum e)
	{
		m_enum = e;
		return *this;
	}

	bool operator==(hkResultEnum e) const { return m_enum == e; }
	bool operator!=(hkResultEnum e) const { return m_enum != e; }

	bool isSuccess() const { return m_enum ^ HK_FAILURE; }

	hkResultEnum m_enum;
};