#include "../PrecompiledHeader.hpp"
#include "WeaponModifiers.hpp"

#include "../Reconstructed Classes/CWeaponInfo.hpp"
#include "../PatternScanner.hpp"
#include "../Pointers.hpp"

struct CWeaponInfoList
{
	Vega::ReconstructedClasses::CWeaponInfo* weapons;
};

Vega::Feature::WeaponModifiers Vega::Feature::g_WeaponModifiers;

void Vega::Feature::WeaponModifiers::initialize()
{
	auto kek = SIG_SCAN("4C 8B 35 ? ? ? ? 49 8B DD").add(3).rip().as<CWeaponInfoList**>();

	//if (kek)
	if constexpr (false)
	{
		for (auto kk = &(*kek)->weapons; *kk; ++kk)
		{
			g_Pointers->call([&]
			{
				if (kk && *kk && (*kk)->szStatName && *(*kk)->szStatName)
				{
					auto buf = std::make_unique<char[]>(std::strlen((*kk)->szStatName) + 1);
					std::strcpy(buf.get(), (*kk)->szStatName);

					m_Names.push_back(std::move(buf));
					m_Infos.push_back(*kk);
				}
			});
		}
	}
}
