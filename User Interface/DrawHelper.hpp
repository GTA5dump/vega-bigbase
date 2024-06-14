#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega
{
	class DrawHelper
	{
	public:
		enum class TextFont
		{
			ChaletLondon = 0,
			HouseScript = 1,
			Monospace = 2,
			WingDings = 3,
			ChaletComprimeCologne = 4,
			Pricedown = 7
		};

		enum class TextAlign
		{
			Left,
			Center,
			Right
		};

		void rect(Vector2 pos, Vector2 size, Color color);
		void sprite(const char* dict, const char* texture, Vector2 pos, Vector2 size, Color color, float heading = 0.f);
		void text(const char* text, Vector2 pos, Vector2 size, Color color, TextAlign align, TextFont font, Vector2 wrap, bool shadow = false, bool outline = false);

		float getTextHeight(TextFont font, float size);
	};

	extern DrawHelper g_DrawHelper;
}
