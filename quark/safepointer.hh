#pragma once

namespace UFG
{
	template <typename T>
	class qSafePointerBase : public qNode<qSafePointerBase<T>>
	{
	public:
		T* m_pPointer;
	};

	template <typename T, typename U = T>
	class qSafePointer : public qSafePointerBase<T>
	{
	public:
		bool operator==(T* other) { return this->m_pPointer == other; }
		U* Get() { return reinterpret_cast<U*>(this->m_pPointer); }
		U* operator->() { return reinterpret_cast<U*>(this->m_pPointer); }
	};

}