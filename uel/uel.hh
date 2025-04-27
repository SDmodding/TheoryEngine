#pragma once

namespace UEL
{
	struct ParametersBase
	{
	};

	class Value
	{
	public:
		struct Type
		{
			enum BaseType
			{
				None,
				Integer,
				Symbol,
				String,
				Real,
				Boolean,
				Enum,
				Vector,
				Struct,
				Lookup,
				Getter,
				Bit,
				Array,
				IntegerArray,
				SymbolArray,
				StringArray,
				RealArray,
				BooleanArray,
				EnumArray,
				VectorArray,
				StructArray,
				LookupArray,
				GetterArray,
				BitField,
				NUM_BASE_VALUE_TYPES
			};

			qEnum<BaseType, u16> mBaseType;
			UFG::qSymbol mDetailedType;
		};

		union
		{
			s64 integer;
			u32 qSymbol_mUID;
			const char* string;
			f32 real;
			bool boolean;
			int enumeration;
			ParametersBase* structPtr;
			//fastdelegate::FastDelegate0<Value>* getter;
			//fastdelegate::FastDelegate1<UFG::qSymbol const&, Value>* lookup;
			f32 qVector_x;
		};

		f32 qVector_y;
		f32 qVector_z;
		f32 qVector_w;
		Type type;
	};

	class Runtime
	{
	public:
		//UFG::qTreeRB<FragmentBindingEntry, FragmentBindingEntry, 1>* mFragmentBindings;
		//UFG::qTreeRB<InvocableBindingEntry, InvocableBindingEntry, 1>* mInvocableBindings;
		//UFG::qTreeRB<ParameterBindingEntry, ParameterBindingEntry, 1>* mParameterBindings;
		//UFG::qTreeRB<EnumBindingEntry, EnumBindingEntry, 1>* mEnumBindings;
	};

	class Expression
	{
	public:
		enum Type
		{
			Unary,
			Binary,
			Constant,
			Invocation,
			Parameter,
			Fragment,
			Argument,
			FragmentInvocation,
			ArgumentUsage
		};

		qEnum<Type, u16> mType;
		Value::Type mValueType;
		u16 mResolved;
		u16 mNeedsRuntimeResolve;

		virtual u32 GetClassSize() = 0;
		virtual const char* GetClassname() = 0;
		virtual bool Resolve(Runtime* instance) = 0;
		virtual Value Eval(ParametersBase* parameters, UFG::qArray<Value>* args, UFG::qStringBuilder* debugStringBuilder) = 0;
	};
}