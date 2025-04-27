#pragma once

class ExpressionParameterFloat
{
public:
	BinString text;
	UFG::qOffset64<UEL::Expression*> expression;
	f32 value;
};

namespace Expression
{
	class IMemberMap
	{
	public:
		IMemberMap* mResourceOwner;

		virtual IMemberMap* FindWithOldPath(const char* resourcePath) { return 0; }

		virtual ~IMemberMap() { }

		virtual IMemberMap* CreateClone() { return 0; }
		
		virtual void Serialize(class IActionTreeSerializer* serializer) { }

		virtual const char* GetClassname() = 0;
		virtual u32 GetClassNameUID() = 0;

		virtual void GetResourcePath(char* path, int pathSize) { UFG::qSPrintf(path, pathSize, GetClassname()); }
		virtual void ResolveReferences() { }
		virtual void SetResourceOwner(IMemberMap* owner) { mResourceOwner = owner; }
		virtual IMemberMap* GetResourceOwner() { return mResourceOwner; }
	};
}

#define IMEMBERMAP_CLASSNAME(name) \
	THEORY_SINLINE const char* sClassName = name;  \
	THEORY_SINLINE u32 sClassNameUID = UFG::qStringHash32(name);  \
	const char* GetClassname() override { return sClassName; }  \
	u32 GetClassNameUID() override { return sClassNameUID; }