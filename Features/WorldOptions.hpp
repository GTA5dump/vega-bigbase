#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class WorldOptions
	{
	public:
		void loop();

		float m_TrafficManagerRadius = 1000.f;
		void trafficManagerLaunch();
		void trafficManagerBoostForward();
		void trafficManagerBoostBackward();
		void trafficManagerKickflip();
		void trafficManagerExplode();
		void trafficManagerKillDriver();
		void trafficManagerRandomPaintJob();
		void trafficManagerNegativeGravity();
		void trafficManagerEarthGravity();
		void trafficManagerMoonGravity();
		void trafficManagerSunGravity();
		void trafficManagerNoGravity();

		bool m_BlackHoleToggle = false;
		Vector3 m_BlackholePosition = { 0, 0, 0 };
		void applyBlackHole();

	};

	extern WorldOptions g_WorldOptions;
}
