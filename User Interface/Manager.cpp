#include "../PrecompiledHeader.hpp"
#include "../ScriptCallback.hpp"
#include "Manager.hpp"
#include "DrawHelper.hpp"
#include "OptionFlags.hpp"
#include "Toggleable.hpp"
#include "NumberBoolOption.hpp"
#include "SimpleOption.hpp"
#include "InstructionalButtons.hpp"

#include "../Reconstructed Classes/CResolution.hpp"

#include "../Pointers.hpp"
#include "../Util.hpp"
#include "../Main.hpp"

namespace Vega
{
	std::unique_ptr<UserInterface::Manager> g_UIManager;
}

namespace Vega::UserInterface
{
	float conv360(float base, float min, float max)
	{
		float fVar0;
		if (min == max) return min;
		fVar0 = max - min;
		base -= std::round(base - min / fVar0) * fVar0;
		if (base < min) base += fVar0;
		return base;
	}

	Vector2 resKek(float kek)
	{
		return { (static_cast<float>(g_Pointers->m_Resolution->y) / static_cast<float>(g_Pointers->m_Resolution->x)) * kek, kek };
	}

	Manager::Manager():
		m_InstructionalButtons(std::make_unique<InstructionalButtons>())
	{
	}

	Manager::~Manager()
	{
	}

	void Manager::onTick()
	{
		if (m_SubmenuStack.top()->getOptions().empty())
		{
			m_SubmenuStack.top()->execute();
		}

		if (m_SubmenuStack.top()->getOptions().empty())
		{
			m_SubmenuStack.top()->addOption(SimpleOption().setLeftText(XorString("No Options")));
		}

		if (Util::isGameFocused() && !g_ScriptCallbackManager->isFlagSet(ScriptCallbackDisplayFlags::Keyboard))
		{
			bool openPressed = false;
			if (m_InputArrows || m_InputNumpad)
			{
				openPressed |= Util::isKeyPressed(VK_INSERT);
			}

			if (m_InputController)
			{
				openPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlScriptRB) && Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlPhoneRight);
			}

			static std::uint32_t openTimer = GetTickCount();
			if (openPressed && (GetTickCount() - openTimer) >= static_cast<std::uint32_t>(m_OpenInputDelay))
			{
				openTimer = GetTickCount();
				m_Opened ^= true;
				if (m_ResetSelectedOptionOnClose)
				{
					m_SubmenuStack.top()->setCurrentOption(0);
				}

				if (m_SoundOpen)
				{
					Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, m_Opened ? XorString("SELECT") : XorString("BACK"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
				}			
			}

			if (m_SubmenuStack.top()->getOptions().empty())
			{
				return;
			}

			if (m_Opened)
			{
				Natives::PAD::DISABLE_CONTROL_ACTION(0, Enums::ControlPhone, true);

				bool downPressed = false;
				bool upPressed = false;
				bool enterPressed = false;
				bool backPressed = false;
				bool leftPressed = false;
				bool rightPressed = false;

				if (m_InputArrows)
				{
					downPressed = Util::isKeyPressed(VK_DOWN);
					upPressed = Util::isKeyPressed(VK_UP);
					enterPressed = Util::isKeyPressed(VK_RETURN);
					backPressed = Util::isKeyPressed(VK_BACK);
					leftPressed = Util::isKeyPressed(VK_LEFT);
					rightPressed = Util::isKeyPressed(VK_RIGHT);
				}
				
				if (m_InputNumpad)
				{
					downPressed |= Util::isKeyPressed(VK_NUMPAD2);
					upPressed |= Util::isKeyPressed(VK_NUMPAD8);
					enterPressed |= Util::isKeyPressed(VK_NUMPAD5);
					backPressed |= Util::isKeyPressed(VK_NUMPAD0);
					leftPressed |= Util::isKeyPressed(VK_NUMPAD4);
					rightPressed |= Util::isKeyPressed(VK_NUMPAD6);
				}

				if (m_InputController)
				{
					downPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendDown);
					upPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendUp);
					enterPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendAccept);
					backPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendCancel);
					leftPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendLeft);
					rightPressed |= Natives::PAD::IS_DISABLED_CONTROL_PRESSED(0, Enums::ControlFrontendRight);
				}

				static std::uint32_t scrollTimer = GetTickCount();
				static std::uint32_t enterTimer = GetTickCount();
				static std::uint32_t backTimer = GetTickCount();
				static std::uint32_t horizontalTimer = GetTickCount();

				if (downPressed)
				{
					if ((GetTickCount() - scrollTimer) >= static_cast<std::uint32_t>(m_ScrollInputDelay))
					{
						scrollTimer = GetTickCount();
						if (m_SoundDown)
						{
							Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("NAV_UP_DOWN"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
						}

						if (m_SubmenuStack.top())
						{
							auto currOpt = m_SubmenuStack.top()->getCurrentOption();
							if (currOpt < m_SubmenuStack.top()->getOptions().size() - 1)
							{
								m_SubmenuStack.top()->setCurrentOption(currOpt + 1);
							}
							else
							{
								m_SubmenuStack.top()->setCurrentOption(0);
							}

							currOpt = m_SubmenuStack.top()->getCurrentOption();
							if (m_SubmenuStack.top()->getOptions()[currOpt]->isFlagSet(OptionFlags::Break))
							{
								if (currOpt < m_SubmenuStack.top()->getOptions().size() - 1)
								{
									m_SubmenuStack.top()->setCurrentOption(currOpt + 1);
								}
							}
						}
					}
				}
				if (upPressed)
				{
					if ((GetTickCount() - scrollTimer) >= static_cast<std::uint32_t>(m_ScrollInputDelay))
					{
						scrollTimer = GetTickCount();
						if (m_SubmenuStack.top())
						{
							if (m_SoundUp)
							{
								Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("NAV_UP_DOWN"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
							}

							auto currOpt = m_SubmenuStack.top()->getCurrentOption();
							if (currOpt > 0)
							{
								m_SubmenuStack.top()->setCurrentOption(currOpt - 1);
							}
							else
							{
								m_SubmenuStack.top()->setCurrentOption(m_SubmenuStack.top()->getOptions().size() - 1);
							}

							currOpt = m_SubmenuStack.top()->getCurrentOption();
							if (m_SubmenuStack.top()->getOptions()[currOpt]->isFlagSet(OptionFlags::Break))
							{
								if (currOpt > 0)
								{
									m_SubmenuStack.top()->setCurrentOption(currOpt - 1);
								}
							}
						}
					}
				}
				if (enterPressed)
				{
					if ((GetTickCount() - enterTimer) >= static_cast<std::uint32_t>(m_EnterInputDelay))
					{
						enterTimer = GetTickCount();
						if (m_SubmenuStack.top() && !m_SubmenuStack.top()->getOptions().empty())
						{
							if (m_SoundEnter)
							{
								Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("SELECT"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
							}
							m_SubmenuStack.top()->getOptions().at(m_SubmenuStack.top()->getCurrentOption())->onKeyPress(KeyPressType::Enter);
						}
					}
				}
				if (backPressed)
				{
					if ((GetTickCount() - backTimer) >= static_cast<std::uint32_t>(m_BackInputDelay))
					{
						backTimer = GetTickCount();
						if (m_SoundBack)
						{
							Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("BACK"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
						}

						if (m_SubmenuStack.size() < 2)
						{
							m_Opened = false;
						}
						else
						{
							if (m_ResetSelectedOptionOnBack)
							{
								m_SubmenuStack.top()->setCurrentOption(0);
							}

							m_SubmenuStack.pop();
						}
					}
				}
				if (leftPressed)
				{
					if ((GetTickCount() - horizontalTimer) >= static_cast<std::uint32_t>(m_HorizonalInputDelay))
					{
						horizontalTimer = GetTickCount();
						if (m_SubmenuStack.top() && !m_SubmenuStack.top()->getOptions().empty())
						{
							decltype(auto) option = m_SubmenuStack.top()->getOptions().at(m_SubmenuStack.top()->getCurrentOption());

							if (option->isFlagSet(OptionFlags::Horizontal))
							{
								if (m_SoundLeft)
								{
									Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("NAV_LEFT_RIGHT"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
								}

								option->onKeyPress(KeyPressType::Left);
							}
						}
					}
				}
				if (rightPressed)
				{
					if ((GetTickCount() - horizontalTimer) >= static_cast<std::uint32_t>(m_HorizonalInputDelay))
					{
						horizontalTimer = GetTickCount();
						if (m_SubmenuStack.top() && !m_SubmenuStack.top()->getOptions().empty())
						{
							decltype(auto) option = m_SubmenuStack.top()->getOptions().at(m_SubmenuStack.top()->getCurrentOption());

							if (option->isFlagSet(OptionFlags::Horizontal))
							{
								if (m_SoundRight)
								{
									Natives::AUDIO::PLAY_SOUND_FRONTEND(-1, XorString("NAV_LEFT_RIGHT"), XorString("HUD_FRONTEND_DEFAULT_SOUNDSET"), false);
								}

								option->onKeyPress(KeyPressType::Right);
							}
						}
					}
				}
			}
		}

		if (!m_Opened)
		{
			m_SubmenuStack.top()->clear();
			return;
		}

		// Header
		switch (m_HeaderType)
		{
		case HeaderType::Static:
			g_DrawHelper.rect(
				{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f / 2.f) },
				{ m_Size.x, 0.1f },
				m_HeaderBackgroundColor);
			break;
		case HeaderType::Gradient:
			for (std::size_t i = 0; i < (m_HeaderGradientTransparent ? 1 : 20); ++i)
			{
				g_DrawHelper.sprite(
					XorString("aircraft_dials"), XorString("aircraft_dials_g0"),
					{ m_Pos.x - (m_HeaderGradientStretch / 2.f), m_Pos.y - m_Size.y / 2.f + (0.1f / 2.f) },
					{ m_Size.x + m_HeaderGradientStretch + m_HeaderGradientFiller, 0.1f },
					m_HeaderGradientColorRight,
					m_HeaderGradientFlip ? 0.f : 180.f);
				g_DrawHelper.sprite(
					XorString("aircraft_dials"), XorString("aircraft_dials_g0"),
					{ m_Pos.x + (m_HeaderGradientStretch / 2.f), m_Pos.y - m_Size.y / 2.f + (0.1f / 2.f) },
					{ m_Size.x + m_HeaderGradientStretch + m_HeaderGradientFiller, 0.1f },
					m_HeaderGradientColorLeft,
					m_HeaderGradientFlip ? 180.f : 0.f);
			}

		default:
			break;
		}
		if (m_DrawHeaderTitle)
		{
			g_DrawHelper.text(MENUNAME_NORMAL,
				{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f / 4.f) },
				{ 1.f, 1.f },
				m_HeaderTextColor,
				DrawHelper::TextAlign::Center,
				DrawHelper::TextFont::HouseScript,
				{ m_Pos.x - m_Size.x / 2.1f, m_Pos.x + m_Size.x / 2.1f },
				m_HeaderTitleShadow, m_HeaderTitleOutline);
		}

		// Glare
		if (m_DrawHeaderGlare)
		{
			constexpr float glareX = 0.3648f;
			constexpr float glareY = 0.194802f;
			constexpr float glareScaleX = 1.066f;
			constexpr float glareScaleY = 1.098f;

			auto handle = Natives::GRAPHICS::REQUEST_SCALEFORM_MOVIE(XorString("mp_menu_glare"));
			static float gGlareDir = 0.5f;
			auto rot = Natives::CAM::GET_GAMEPLAY_CAM_ROT(2);
			float dir = conv360(rot.z, 0.f, 360.f);
			if ((gGlareDir == 0.f || gGlareDir - dir > 0.5f) || gGlareDir - dir < -0.5f)
			{
				gGlareDir = dir;
				Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(handle, XorString("SET_DATA_SLOT"));
				Natives::GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_FLOAT(dir);
				Natives::GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
			}

			for (std::make_signed_t<std::size_t> i = 0; i < m_HeaderGlareIterations; ++i)
			{
				Natives::GRAPHICS::DRAW_SCALEFORM_MOVIE(handle, m_Pos.x + glareX, m_Pos.y + glareY, glareScaleX, glareScaleY, 255, 255, 255, 255, NULL);
			}
		}

		if (m_DrawInstructionButtons && !m_HideInstructionalButtonsThisFrame)
		{
			if (m_InputNumpad)
			{
				m_InstructionalButtons->addById(XorString("Up"), 144);
				m_InstructionalButtons->addById(XorString("Down"), 138);
				m_InstructionalButtons->addById(XorString("Left"), 140);
				m_InstructionalButtons->addById(XorString("Right"), 142);
				m_InstructionalButtons->addById(XorString("Select"), 141);
				m_InstructionalButtons->addById(XorString("Back"), 136);
				m_InstructionalButtons->addById(XorString("Close"), 133);
			}
			else
			{
				m_InstructionalButtons->addById(XorString("Up"), 194);
				m_InstructionalButtons->addById(XorString("Down"), 195);
				m_InstructionalButtons->addById(XorString("Left"), 196);
				m_InstructionalButtons->addById(XorString("Right"), 197);
				m_InstructionalButtons->addByInput(XorString("Select"), Enums::ControlFrontendAccept);
				m_InstructionalButtons->addByInput(XorString("Back"), Enums::ControlPhoneCancel);
			}

			m_InstructionalButtons->draw();
		}
		else if (m_HideInstructionalButtonsThisFrame)
		{
			m_HideInstructionalButtonsThisFrame = false;
		}

		decltype(auto) options = m_SubmenuStack.top()->getOptions();

		// Top bar
		char optionCounter[32] = {};
		std::snprintf(optionCounter, sizeof(optionCounter), XorString("%llu / %llu"), m_SubmenuStack.top()->getSelectedWithoutBreaks() + 1, m_SubmenuStack.top()->getOptionCountWithoutBreaks());
		char newSubName[64] = {};
		std::strncpy(newSubName, m_SubmenuStack.top()->getName(), sizeof(newSubName));
		Util::uppercase(newSubName);
		g_DrawHelper.rect(
			{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f) + (0.035f / 2.f) },
			{ m_Size.x, 0.035f },
			m_TopbarBackgroundColor);
		g_DrawHelper.text(newSubName,
			{ m_Pos.x - m_Size.x / m_TopbarLeftPadding, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.145f) - (g_DrawHelper.getTextHeight(DrawHelper::TextFont::ChaletLondon, m_TopbarTextSize) / 2.f) },
			{ m_TopbarTextSize, m_TopbarTextSize },
			m_TopbarSubmenuNameColor,
			DrawHelper::TextAlign::Left,
			DrawHelper::TextFont::ChaletLondon,
			{ m_Pos.x - m_Size.x / m_TopbarLeftPadding, m_Pos.x + m_Size.x / 2.1f },
			false, false);
		g_DrawHelper.text(optionCounter,
			{ m_Pos.x + m_Size.x / m_TopbarRightPadding, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.145f) - (g_DrawHelper.getTextHeight(DrawHelper::TextFont::ChaletLondon, m_TopbarTextSize) / 2.f) },
			{ m_TopbarTextSize, m_TopbarTextSize },
			m_TopbarOptionCountColor,
			DrawHelper::TextAlign::Right,
			DrawHelper::TextFont::ChaletLondon,
			{ m_Pos.x - m_Size.x / m_TopbarLeftPadding, m_Pos.x + m_Size.x / m_TopbarRightPadding },
			false, false);

		auto currOpt = m_SubmenuStack.top()->getCurrentOption();

		std::size_t startPoint = 0;
		std::size_t endPoint = options.size() > m_OptionsPerPage ? m_OptionsPerPage : options.size();

		if (options.size() > m_OptionsPerPage && currOpt >= m_OptionsPerPage)
		{
			startPoint = currOpt - m_OptionsPerPage + 1;
			endPoint = currOpt + 1;
		}

		for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
		{
			auto selected = (i == m_SubmenuStack.top()->getCurrentOption());
			decltype(auto) option = options[i].get();
			auto horizontal = option->isFlagSet(OptionFlags::Horizontal) && !option->isFlagSet(OptionFlags::Compound);

			auto rightPadding = m_OptionLeftPadding;
			if (horizontal && selected)
			{
				rightPadding = m_OptionSelectedHorizontalPadding;
			}
			else if (option->isFlagSet(OptionFlags::Compound))
			{
				rightPadding = m_OptionCompoundPadding;
			}

			g_DrawHelper.rect(
				{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.35f) + j * m_OptionRectSize + (m_OptionRectSize / 2.f) },
				{ m_Size.x, m_OptionRectSize },
				selected ? m_TextBackgroundSelectedColor : m_TextBackgroundUnselectedColor);
			if (!option->isFlagSet(OptionFlags::Break)) g_DrawHelper.text(option->getLeftText(),
				{ m_Pos.x - m_Size.x / m_OptionLeftPadding, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.32f) + j * m_OptionRectSize - (g_DrawHelper.getTextHeight(DrawHelper::TextFont::ChaletLondon, m_OptionTextSize) / 2.f) + (m_OptionRectSize / 2.f) },
				{ m_OptionTextSize, m_OptionTextSize },
				selected ? m_NormalTextSelectedColor : m_NormalTextUnselectedColor,
				DrawHelper::TextAlign::Left,
				DrawHelper::TextFont::ChaletLondon,
				{ m_Pos.x - m_Size.x / 2.1f, m_Pos.x + m_Size.x / 2.1f },
				false, false);
			g_DrawHelper.text(option->getRightText(),
				{ m_Pos.x + m_Size.x / (horizontal && selected ? 2.38f : m_OptionLeftPadding), m_Pos.y - m_Size.y / 2.f + (0.1f * 1.32f) + j * m_OptionRectSize - (g_DrawHelper.getTextHeight(DrawHelper::TextFont::ChaletLondon, m_OptionTextSize) / 2.f) + (m_OptionRectSize / 2.f) },
				{ m_OptionTextSize, m_OptionTextSize },
				selected ? m_NormalTextSelectedColor : m_NormalTextUnselectedColor,
				DrawHelper::TextAlign::Right,
				DrawHelper::TextFont::ChaletLondon,
				{ m_Pos.x - m_Size.x / 2.1f, m_Pos.x + m_Size.x / rightPadding },
				false, false);

			if (option->isFlagSet(OptionFlags::Break))
			{
				g_DrawHelper.text(option->getLeftText(),
					{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.425f) + j * m_OptionRectSize },
					{ 0.42f, 0.42f },
					selected ? m_TextBreakSelectedColor : m_TextBreakUnselectedColor,
					DrawHelper::TextAlign::Center,
					DrawHelper::TextFont::HouseScript,
					{ m_Pos.x - m_Size.x / 2.1f, m_Pos.x + m_Size.x / 2.1f },
					false, false);
			}

			if (horizontal && selected)
			{
				g_DrawHelper.sprite(
					XorString("commonmenu"), XorString("shop_arrows_upanddown"),
					{ m_Pos.x + m_Size.x / 2.225f, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.355f) + j * m_OptionRectSize + (m_OptionRectSize / 2.f) },
					resKek(0.035f),
					m_HorizontalIndicatorColor,
					90.f
				);
			}

			if (option->isFlagSet(OptionFlags::Compound))
			{
				auto toggled = option->isFlagSet(OptionFlags::IsToggled);
				auto size = resKek(0.03f);
				g_DrawHelper.sprite(
					XorString("commonmenu"), XorString("common_medal"),
					{ m_Pos.x + m_Size.x / m_OptionLeftPadding - (size.x / 2.f), m_Pos.y - m_Size.y / 2.f + (0.1f * 1.35f) + j * m_OptionRectSize + (m_OptionRectSize / 2.f) },
					size,
					toggled ? m_OptionCompoundToggledColor : m_OptionCompoundNotToggledColor
				);
			}

			if (option->isFlagSet(OptionFlags::Submenu))
			{
				auto size = resKek(0.01f);
				g_DrawHelper.sprite(
					XorString("mparrow"), XorString("mp_arrowxlarge"),
					{ m_Pos.x + m_Size.x / m_OptionLeftPadding - (size.x / 1.f), m_Pos.y - m_Size.y / 2.f + (0.1f * 1.35f) + j * m_OptionRectSize + (m_OptionRectSize / 2.f) },
					{ size.x, size.y * 2.f },
					selected ? m_SubmenuSelectedIndicatorColor : m_SubmenuUnselectedIndicatorColor
				);
			}
		}

		// Footer
		if (m_DrawFooter)
		{
			auto size = 0.03f;
			auto heading = 0.f;
			auto texx = XorString("shop_arrows_upanddown");

			auto top = (m_SubmenuStack.top()->getCurrentOption() == 0);
			auto bottom = (m_SubmenuStack.top()->getCurrentOption() == m_SubmenuStack.top()->getOptions().size() - 1);
			if (m_FooterDynamicIcon && (top || bottom))
			{
				texx = XorString("arrowright");
				heading = bottom ? 270.f : 90.f;
				size = 0.02f;
			}

			if (m_DrawFooterBackground)
			{
				g_DrawHelper.rect(
					{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.35f) + (endPoint - startPoint) * m_OptionRectSize + (m_FooterSize / 2.f) },
					{ m_Size.x, m_FooterSize },
					m_FooterBackground
				);
			}

			if (m_DrawFooterIcon)
			{
				g_DrawHelper.sprite(
					XorString("commonmenu"), texx,
					{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.35f) + (endPoint - startPoint) * m_OptionRectSize + (m_FooterSize / 2.f) },
					resKek(size),
					m_FooterIcon,
					heading
				);
			}

			if (m_DrawDescriptions)
			{
				decltype(auto) currentOpt = m_SubmenuStack.top()->getOptions()[m_SubmenuStack.top()->getCurrentOption()];
				if (currentOpt && currentOpt->isFlagSet(OptionFlags::Description))
				{
					auto spriteScale = resKek(0.025f);

					g_DrawHelper.rect(
						{ m_Pos.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.4f) + (endPoint - startPoint) * m_OptionRectSize + m_FooterSize + (m_DescriptionSize / 2.f) },
						{ m_Size.x, m_DescriptionSize },
						m_DescriptionBackgroundColor
					);
					g_DrawHelper.sprite(
						"shared", "info_icon_32",
						{ m_Pos.x - m_Size.x / 2.05f + (spriteScale.x / 2.f), m_Pos.y - m_Size.y / 2.f + (0.1f * 1.4f) + (endPoint - startPoint) * m_OptionRectSize + m_FooterSize + (m_DescriptionSize / 2.f) },
						spriteScale,
						m_DescriptionSpriteColor
					);
					g_DrawHelper.text(
						currentOpt->getDescription(),
						{ m_Pos.x - m_Size.x / 2.05f + spriteScale.x, m_Pos.y - m_Size.y / 2.f + (0.1f * 1.365f) + (endPoint - startPoint) * m_OptionRectSize + m_FooterSize + (m_DescriptionSize / 2.f) - (g_DrawHelper.getTextHeight(DrawHelper::TextFont::ChaletLondon, 0.3f) / 2.f) },
						{ 0.24f, 0.3f },
						m_DescriptionTextColor,
						DrawHelper::TextAlign::Left,
						DrawHelper::TextFont::ChaletLondon,
						{ m_Pos.x - m_Size.x / 2.1f, m_Pos.x + m_Size.x / 2.1f }
					);
				}
			}
		}

		m_SubmenuStack.top()->clear();
	}

	Manager& Manager::switchToSub(std::uint32_t id)
	{
		for (decltype(auto) i : m_AllSubmenus)
		{
			if (i->getId() == id)
			{
				if (m_ResetSelectedOptionOnSwitch)
				{
					m_SubmenuStack.top()->setCurrentOption(0);
				}

				m_SubmenuStack.push(i.get());
				i->clear();
				i->execute();

				break;
			}
		}

		return *this;
	}
}