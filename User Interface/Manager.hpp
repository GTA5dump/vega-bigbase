#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "HeaderType.hpp"
#include "Submenu.hpp"

namespace Vega
{
	class InstructionalButtons;
}

namespace Vega::UserInterface
{
	class Manager
	{
	public:
		Vector2 m_Pos = { 0.8f, 0.4f };
		Vector2 m_Size = { 0.25f, 0.6f };
		Color m_BackgroundColor = { 0, 0, 0, 130 };

		std::size_t m_OptionsPerPage = 13;
		bool m_Opened = true;

		std::vector<std::unique_ptr<Submenu>> m_AllSubmenus = {};
		std::stack<Submenu*> m_SubmenuStack = {};

		bool m_BlockInput = false;
		bool m_InputNumpad = false;
		bool m_InputArrows = true;
		bool m_InputController = false;
		std::int32_t m_ScrollInputDelay = 120;
		std::int32_t m_HorizonalInputDelay = 120;
		std::int32_t m_OpenInputDelay = 200;
		std::int32_t m_EnterInputDelay = 200;
		std::int32_t m_BackInputDelay = 200;

		HeaderType m_HeaderType = HeaderType::Gradient;
		bool m_HeaderGradientTransparent = false;
		bool m_HeaderGradientFlip = false;
		float m_HeaderGradientStretch = 0.f;
		float m_HeaderGradientFiller = 0.f;
		Color m_HeaderGradientColorLeft = { 0, 186, 255, 255 };
		Color m_HeaderGradientColorRight = { 252, 0, 255, 255 };
		bool m_DrawHeaderGlare = true;
		bool m_DrawHeaderTitle = true;
		bool m_HeaderTitleShadow = true;
		bool m_HeaderTitleOutline = false;
		Color m_HeaderBackgroundColor = MENU_DEFAULT_HEADER_COLOR;
		Color m_HeaderTextColor = { 255, 255, 255, 255 };
		std::make_signed_t<std::size_t> m_HeaderGlareIterations = 1;

		float m_TopbarTextSize = 0.4f;
		float m_TopbarLeftPadding = 2.1f;
		float m_TopbarRightPadding = 2.1f;
		Color m_TopbarBackgroundColor = { 0, 0, 0, 200 };
		Color m_TopbarSubmenuNameColor = { 255, 255, 255, 255 };
		Color m_TopbarOptionCountColor = { 255, 255, 255, 255 };

		float m_OptionRectSize = 0.04f;
		float m_OptionTextSize = 0.35f;
		float m_OptionLeftPadding = 2.1f;
		float m_OptionRightPadding = 2.1f;
		float m_OptionSelectedHorizontalPadding = 2.4f;
		float m_OptionCompoundPadding = 2.4f;
		Color m_NormalTextSelectedColor = { 0, 0, 0, 255 };
		Color m_NormalTextUnselectedColor = { 255, 255, 255, 255 };
		Color m_TextBreakSelectedColor = { 0, 0, 0, 255 };
		Color m_TextBreakUnselectedColor = { 200, 200, 200, 255 };
		Color m_HorizontalIndicatorColor = { 0, 0, 0, 255 };
		Color m_SubmenuSelectedIndicatorColor = { 0, 0, 0, 255 };
		Color m_SubmenuUnselectedIndicatorColor = { 255, 255, 255, 255 };
		Color m_TextBackgroundSelectedColor = { 255, 255, 255, 255 };
		Color m_TextBackgroundUnselectedColor = { 0, 0, 0, 160 };
		Color m_OptionCompoundToggledColor = { 0, 255, 0, 255 };
		Color m_OptionCompoundNotToggledColor = { 255, 0, 0, 255 };

		bool m_ResetSelectedOptionOnClose = false;
		bool m_ResetSelectedOptionOnSwitch = false;
		bool m_ResetSelectedOptionOnBack = false;

		bool m_DrawFooter = true;
		bool m_FooterDynamicIcon = true;
		bool m_DrawFooterBackground = true;
		bool m_DrawFooterIcon = true;
		float m_FooterSize = 0.04f;
		Color m_FooterBackground = { 0, 0, 0, 200 };
		Color m_FooterIcon = { 255, 255, 255, 255 };

		bool m_SoundLeft = true;
		bool m_SoundRight = true;
		bool m_SoundUp = true;
		bool m_SoundDown = true;
		bool m_SoundBack = true;
		bool m_SoundEnter = true;
		bool m_SoundOpen = true;

		bool m_DrawDescriptions = true;
		float m_DescriptionSize = 0.035f;
		Color m_DescriptionBackgroundColor = { 0, 0, 0, 120 };
		Color m_DescriptionTextColor = { 255, 255, 255, 255 };
		Color m_DescriptionSpriteColor = { 255, 255, 255, 255 };

		bool m_DrawInstructionButtons = true;
		bool m_HideInstructionalButtonsThisFrame = false;
		std::unique_ptr<InstructionalButtons> m_InstructionalButtons;

	public:
		explicit Manager();
		~Manager();

		void onTick();

		template <typename SubmenuType>
		Manager& addSubmenu(SubmenuType& submenu)
		{
			auto newSub = std::make_unique<std::remove_reference_t<SubmenuType>>(std::move(submenu));

			if (m_SubmenuStack.empty())
			{
				m_SubmenuStack.push(newSub.get());
			}

			m_AllSubmenus.push_back(std::move(newSub));

			return *this;
		}
		Manager& switchToSub(std::uint32_t id);
	};
}

namespace Vega
{
	extern std::unique_ptr<UserInterface::Manager> g_UIManager;
}
