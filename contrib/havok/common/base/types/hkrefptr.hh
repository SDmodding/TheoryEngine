#pragma once

template <class T>
class hkRefPtr
{
public:
	T* operator->() const { m_pntr; }

private:
	T* m_pntr;
};