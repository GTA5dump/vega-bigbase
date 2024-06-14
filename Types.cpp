#include "PrecompiledHeader.hpp"

namespace Vega
{
	namespace Natives
	{
		Natives::Vector3::Vector3(float X, float Y, float Z) :
			x(X),
			y(Y),
			z(Z)
		{
		}

		Natives::Vector3::Vector3(const Vega::Vector3 that) :
			Natives::Vector3(that.x, that.y, that.z)
		{
		}

		Natives::Vector3::Vector3(Natives::Vector3 const& that) :
			Natives::Vector3(that.x, that.y, that.z)
		{
		}
	}

	Vector3::Vector3(float X, float Y, float Z) :
		x(X),
		y(Y),
		z(Z)
	{
	}

	Vector3::Vector3(const Natives::Vector3 that) :
		Vector3(that.x, that.y, that.z)
	{
	}

	Vega::Vector3::Vector3(Vega::Vector3 const& that) :
		Vector3(that.x, that.y, that.z)
	{
	}

	Vega::Vector3 operator-(const Vega::Vector3 vec)
	{
		return { -vec.x, -vec.y, -vec.z };
	}

	Vega::Vector3 operator+(const Vega::Vector3 vec, const float amount)
	{
		return { vec.x + amount, vec.y + amount, vec.z + amount };
	}

	Vega::Vector3 operator-(const Vega::Vector3 vec, const float amount)
	{
		return { vec.x - amount, vec.y + amount, vec.z + amount };
	}

	Vega::Vector3 operator*(const Vega::Vector3 vec, const float amount)
	{
		return { vec.x * amount, vec.y * amount, vec.z * amount };
	}

	Vega::Vector3 operator/(const Vega::Vector3 vec, const float amount)
	{
		return { vec.x / amount, vec.y / amount, vec.z / amount };
	}

	Vega::Vector3 operator+(Vega::Vector3 left, Vega::Vector3 right)
	{
		return { left.x + right.x, left.y + right.y, left.z + right.z };
	}

	Vega::Vector3 operator-(Vega ::Vector3 left, Vega::Vector3 right)
	{
		return { left.x - right.x, left.y - right.y, left.z - right.z };
	}

	Vega::Vector3 operator*(Vega::Vector3 left, Vega::Vector3 right)
	{
		return { left.x * right.x, left.y * right.y, left.z * right.z };
	}

	Vega::Vector3 operator/(Vega::Vector3 left, Vega::Vector3 right)
	{
		return { left.x / right.x, left.y / right.y, left.z / right.z };
	}

}
