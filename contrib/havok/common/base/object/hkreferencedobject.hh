#pragma once

class hkReferencedObject : public hkBaseObject
{
public:
	u16 m_memSizeAndFlags;
	i16 m_referenceCount;
};