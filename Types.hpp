#pragma once
#include "PrecompiledHeader.hpp"
#include "ScriptHandle.hpp"

namespace Vega
{
	struct Vector3;
	namespace Natives
	{
		using Void = std::uint32_t;
		using Any = std::uint32_t;
		using uint = std::uint32_t;
		using Hash = std::uint32_t;
		using Entity = std::int32_t;
		using Player = std::int32_t;
		using FireId = std::int32_t;
		using Ped = Entity;
		using Vehicle = Entity;
		using Cam = std::int32_t;
		using CarGenerator = std::int32_t;
		using Group = std::int32_t;
		using Train = std::int32_t;
		using Object = Entity;
		using Pickup = Object;
		using Weapon = std::int32_t;
		using Interior = std::int32_t;
		using Blip = std::int32_t;
		using Texture = std::int32_t;
		using TextureDict = std::int32_t;
		using CoverPoint = std::int32_t;
		using Camera = std::int32_t;
		using TaskSequence = std::int32_t;
		using ColourIndex = std::int32_t;
		using Sphere = std::int32_t;
		using ScrHandle = std::int32_t;

#	pragma warning(push)
#	pragma warning(disable : 4324) // structure was padded due to alignas
		struct Vector3
		{
			Vector3() = default;
			Vector3(float X, float Y, float Z);
			Vector3(Vega::Vector3 that);
			Vector3(Vector3 const& that);

			alignas(std::uintptr_t) float x;
			alignas(std::uintptr_t) float y;
			alignas(std::uintptr_t) float z;
		};
#	pragma warning(pop)
	}

#pragma pack(push, 1)

	struct Vector2
	{
		float x = 0.f;
		float y = 0.f;
	};

	struct Vector3
	{
		Vector3() = default;
		Vector3(float X, float Y, float Z);
		Vector3(Natives::Vector3 that);
		Vector3(Vector3 const& that);

		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	};

	struct Vector4
	{
		operator Vector3()
		{
			return { x, y, z };
		}

		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 0.f;
	};

	struct Matrix4x4
	{
		union
		{
			float floats[16];
			Vector4 vectors[4] = {};
		};
	};

	struct Color
	{
		std::uint8_t r = 0;
		std::uint8_t g = 0;
		std::uint8_t b = 0;
		std::uint8_t a = 0;
	};

	struct Raycast
	{
		bool hit = false;
		Natives::Vector3 coords{};
		Natives::Vector3 surface{};
		Natives::Entity ent = -1;
		bool hitGround = false;
	};

#pragma pack(pop)

	Vector3 operator-(Vector3 vec);
	Vector3 operator+(const Vector3 vec, float amount);
	Vector3 operator-(Vector3 vec, float amount);
	Vector3 operator*(Vector3 vec, float amount);
	Vector3 operator/(Vector3 vec, float amount);

	Vector3 operator+(Vector3 left, Vector3 right);
	Vector3 operator-(Vector3 left, Vector3 right);
	Vector3 operator*(Vector3 left, Vector3 right);
	Vector3 operator/(Vector3 left, Vector3 right);


	static_assert(sizeof(Vector3) == sizeof(float) * 3);
	static_assert(sizeof(Natives::Vector3) == sizeof(float) * 6);

	static_assert(offsetof(Natives::Vector3, x) == sizeof(float) * 0);
	static_assert(offsetof(Natives::Vector3, y) == sizeof(float) * 2);
	static_assert(offsetof(Natives::Vector3, z) == sizeof(float) * 4);
}
