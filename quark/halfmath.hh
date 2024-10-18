#pragma once

namespace UFG
{
	class qHalfFloat
	{
	public:
		u16 mRep;

		qHalfFloat() : mRep(0) {}

		qHalfFloat(f32 value) { Set(value); }

		void Set(f32 value);

		f32 Get();
	};

	class qHalfVector2
	{
	public:
		qHalfFloat x, y;

		void operator=(const qVector2& t);

		operator qVector2();
	};

	class qHalfVector3
	{
	public:
		qHalfFloat x, y, z;

		void operator=(const qVector3& t);

		operator qVector3();
	};

	class qHalfVector4
	{
	public:
		qHalfFloat x, y, z, w;

		void operator=(const qVector4& t);

		operator qVector4();
	};

#ifdef THEORY_IMPL

	//-------------------------------------------------------------------
	// Half Float
	//-------------------------------------------------------------------

	void qHalfFloat::Set(f32 value)
	{
		u32 float_bits = *reinterpret_cast<u32*>(&value);
		u32 abs_value = float_bits & 0x7FFFFFFF;
		u16 sign_bit = (float_bits >> 16) & 0x8000;

		/* Inf or NaN */
		if (abs_value >= 0x7F800000)
		{
			mRep = (float_bits & 0x7FFFFF ? 0x7FFF : 0x7C00) | sign_bit;
			return;
		}
		
		/* Too small to be normalized, might be subnormal */
		if (abs_value < 0x38800000) 
		{ 
			u32 mantissa = (float_bits & 0x7FFFFF) | 0x800000;
			int shift = 113 - (float_bits >> 23);
			mRep = (mantissa >> shift) | sign_bit;
			return;
		}

		u32 new_exp = ((float_bits >> 23) - 127 + 15) << 10; 
		u32 new_mantissa = (float_bits >> 13) & 0x3FF;
		mRep = sign_bit | new_exp | new_mantissa;
	}
	
	f32 qHalfFloat::Get()
	{
		u32 exponent = ((mRep >> 10) & 0x1F) + 0x70;
		u32 mantissa = (mRep & 0x3FF) << 13;
		u32 flt = ((exponent << 23) & 0x7F800000) | mantissa;
		flt |= (mRep & 0x8000) << 16;

		return *reinterpret_cast<f32*>(&flt);
	}

	//-------------------------------------------------------------------

	void qHalfVector2::operator=(const qVector2& t)
	{
		x.Set(t.x);
		y.Set(t.y);
	}

	qHalfVector2::operator qVector2()
	{
		return { x.Get(), y.Get() };
	}

	//-------------------------------------------------------------------

	void qHalfVector3::operator=(const qVector3& t)
	{
		x.Set(t.x);
		y.Set(t.y);
		z.Set(t.z);
	}

	qHalfVector3::operator qVector3()
	{
		return { x.Get(), y.Get(), z.Get() };
	}

	//-------------------------------------------------------------------

	void qHalfVector4::operator=(const qVector4& t)
	{
		x.Set(t.x);
		y.Set(t.y);
		z.Set(t.z);
		w.Set(t.w);
	}

	qHalfVector4::operator qVector4()
	{
		return { x.Get(), y.Get(), z.Get(), w.Get() };
	}

#endif
}