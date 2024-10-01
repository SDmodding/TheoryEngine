#pragma once

namespace UFG
{
	class qVector2
	{
	public:
		float x, y;

		inline qVector2() : x(0.f), y(0.f) {}
		inline qVector2(float fX, float fY) : x(fX), y(fY) {}
	};

	class qVector3
	{
	public:
		float x, y, z;

		inline qVector3() : x(0.f), y(0.f), z(0.f) {}
		inline qVector3(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {}
	};

	class qVector4
	{
	public:
		float x, y, z, w;

		inline qVector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		inline qVector4(float fX, float fY, float fZ, float fW) : x(fX), y(fY), z(fZ), w(fW) {}
	};

	class qQuaternion
	{
	public:
		float x, y, z, w;

		inline qQuaternion() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		inline qQuaternion(float fX, float fY, float fZ, float fW) : x(fX), y(fY), z(fZ), w(fW) {}
	};
}