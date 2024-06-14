#include "../PrecompiledHeader.hpp"
#include "InstructionalButtons.hpp"
#include "../Natives.hpp"
#include "../Pointers.hpp"

namespace Vega
{
	InstructionalButtons::InstructionalButtons()
	{
	}

	InstructionalButtons::~InstructionalButtons()
	{
	}

	void InstructionalButtons::addCustom(const char* const name, const char* const custom)
	{
		InstructionalButton button{};
		std::strncpy(button.m_Name, name, sizeof(button.m_Name));
		std::snprintf(button.m_StringId, sizeof(button.m_StringId), "t_%s", custom);
		button.m_Type = InstructionalButton::Type::String;
		m_Buttons.push_back(std::move(button));
	}

	void InstructionalButtons::addByInput(const char* const name, std::uint32_t const id)
	{
		InstructionalButton button{};
		std::strncpy(button.m_Name, name, sizeof(button.m_Name));
		std::strncpy(button.m_StringId, Natives::PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(0, id, true), sizeof(button.m_StringId));
		button.m_Type = InstructionalButton::Type::String;
		m_Buttons.push_back(std::move(button));
	}

	void InstructionalButtons::addById(const char* const name, std::uint32_t id)
	{
		InstructionalButton button{};
		std::strncpy(button.m_Name, name, sizeof(button.m_Name));
		button.m_NumericId = id;
		button.m_Type = InstructionalButton::Type::Numeric;
		m_Buttons.push_back(std::move(button));
	}

	void InstructionalButtons::draw()
	{
		auto handle = Natives::GRAPHICS::REQUEST_SCALEFORM_MOVIE(XorString("instructional_buttons"));
		Natives::GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(handle, 255, 255, 255, 0, 0);

		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("CLEAR_ALL"));
		Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("SET_CLEAR_SPACE"));
		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(200);
		Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		for (std::size_t i = 0, end = m_Buttons.size(); i < end; ++i)
		{
			auto const& button = m_Buttons[i];

			Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("SET_DATA_SLOT"));
			Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(static_cast<int>(i));

			if (button.m_Type == InstructionalButton::Type::String)
			{
				Natives::GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING(XorString("STRING"));
				g_Pointers->call(g_Pointers->m_AddTextComponent, button.m_StringId);
				Natives::GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
			}
			else if (button.m_Type == InstructionalButton::Type::Numeric)
			{
				Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(button.m_NumericId);
			}

			Natives::GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING(XorString("STRING"));
			g_Pointers->call(g_Pointers->m_AddTextComponent, button.m_Name);
			Natives::GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
			Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		}

		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("DRAW_INSTRUCTIONAL_BUTTONS"));
		Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("SET_BACKGROUND_COLOUR"));
		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);  // R
		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);  // G
		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);  // B
		Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(80); // A
		Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		m_Buttons.clear();
	}
}
