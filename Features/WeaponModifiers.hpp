#pragma once
#include "../PrecompiledHeader.hpp"
#include "../Reconstructed Classes/Declarations.hpp""

namespace Vega::Feature
{
	class WeaponModifiers
	{
	public:
		std::vector<std::unique_ptr<char[]>> m_Names;
		std::vector<ReconstructedClasses::CWeaponInfo*> m_Infos;
		std::size_t m_Selected = 0;
		
		void initialize();

	};

	extern WeaponModifiers g_WeaponModifiers;
}
