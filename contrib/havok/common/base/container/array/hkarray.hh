#pragma once

template <typename T>
class hkArrayBase
{
public:
	T* m_data;
	int m_size;
	int m_capacityAndFlags;
};

template <typename T, typename Allocator = hkContainerHeapAllocator>
class hkArray : public hkArrayBase<T>
{
public:

};