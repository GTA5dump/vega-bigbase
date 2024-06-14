#pragma once
#include "../PrecompiledHeader.hpp"
#include "../Natives.hpp"
#include "../User Interface/InstructionalButtons.hpp"

namespace Vega::Feature
{
	class SpoonerOptions
	{
	public:
		void loop();

		bool m_Spooner = false;

		struct SavedEntity
		{
			std::int32_t m_Handle = -1;
			char m_Name[64] = {};
			Vector3 m_Position{};
			Vector3 m_Rotation{};
			bool m_Freezed = false;
			bool m_Dynamic = false;
			bool m_Collision = false;
			bool m_Gravity = false;
		};

		std::unique_ptr<InstructionalButtons> m_Buttons = std::make_unique<InstructionalButtons>();

		std::deque<SavedEntity> m_Entities{};
		SavedEntity m_CurrentEntity;
		const char* m_Search = "";
		const char* m_SearchPed = "";

		bool m_SpawnFrozen = true;
		bool m_SpawnDynamic = true;
		bool m_SpawnCollision = true;
		bool m_SpawnGravity = true;

		float m_Precision = 0.1f;

		void applySpooner();
		void spawnProp(const char* hash);
		void spawnPed(const char* hash);
		void Search();
		void SearchPeds();
		void draw_ent_box(std::int32_t ent);
	};

	extern SpoonerOptions g_SpoonerOptions;
}

