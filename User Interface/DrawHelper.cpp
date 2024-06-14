#include "../PrecompiledHeader.hpp"
#include "DrawHelper.hpp"
#include "../Reconstructed Classes/CTextInfo.hpp"
#include "../Pointers.hpp"
#include "../Natives.hpp"

namespace Vega
{

	DrawHelper g_DrawHelper;

	void DrawHelper::rect(const Vector2 pos, const Vector2 size, const Color color)
	{
		//Natives::GRAPHICS::DRAW_RECT(pos.x, pos.y, size.x, size.y, color.r, color.g, color.b, color.a, 0);
		g_Pointers->call(g_Pointers->m_DrawRect, pos.x, pos.y, size.x, size.y, color.r, color.g, color.b, color.a);
	}

	void DrawHelper::sprite(const char* const dict, const char* const texture, const Vector2 pos, const Vector2 size, const Color color, const float heading)
	{
		if (Natives::GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict))
		{
			//Natives::GRAPHICS::DRAW_SPRITE(dict, texture, pos.x, pos.y, size.x, size.y, heading, color.r, color.g, color.b, color.a, 0);
			g_Pointers->call(g_Pointers->m_DrawSprite, dict, texture, pos.x, pos.y, size.x, size.y, heading, color.r, color.g, color.b, color.a);
		}
		else
		{
			Natives::GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, 0);
		}
	}

	void DrawHelper::text(const char* text, const Vector2 pos, const Vector2 size, const Color color, const TextAlign align, const TextFont font, const Vector2 wrap, const bool shadow, const bool outline)
	{
		g_Pointers->m_TextInfo->color.r = color.r;
		g_Pointers->m_TextInfo->color.g = color.g;
		g_Pointers->m_TextInfo->color.b = color.b;
		g_Pointers->m_TextInfo->color.a = color.a;
		g_Pointers->m_TextInfo->font = static_cast<std::int64_t>(font);
		g_Pointers->m_TextInfo->wrap = wrap;
		g_Pointers->m_TextInfo->scale = size;
		g_Pointers->m_TextInfo->shadow = shadow;
		g_Pointers->m_TextInfo->outline = outline;

		switch (align)
		{
		case TextAlign::Left:
			g_Pointers->m_TextInfo->align = 1;
			break;
		case TextAlign::Center:
			g_Pointers->m_TextInfo->align = 0;
			break;
		case TextAlign::Right:
			g_Pointers->m_TextInfo->align = 2;
		default:
			break;
		}

		g_Pointers->call(g_Pointers->m_BeginTextDisplay, XorString("STRING"));
		g_Pointers->call(g_Pointers->m_AddTextComponent, text);
		g_Pointers->call(g_Pointers->m_EndTextDisplay, pos.x, pos.y);
	}

	float DrawHelper::getTextHeight(const TextFont font, const float size)
	{
		return Natives::HUD::_GET_TEXT_SCALE_HEIGHT(size, static_cast<std::int32_t>(font));
	}
}
