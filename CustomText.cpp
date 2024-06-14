#include "CustomText.hpp"

CustomText g_CustomText;

void CustomText::initialize()
{
}

void CustomText::reg(const char* label, const char* text)
{
	auto labelSize = std::strlen(label) + 1;
	auto textSize = std::strlen(text) + 1;

	auto labelBuf = std::make_unique<char[]>(labelSize);
	auto textBuf = std::make_unique<char[]>(textSize);

	std::strcpy(labelBuf.get(), label);
	std::strcpy(textBuf.get(), text);

	m_Texts.emplace(std::move(labelBuf), std::move(textBuf));
}

void CustomText::unreg(const char* label)
{
	for (decltype(auto) i : m_Texts)
	{
		auto raw = i.first.get();
		if (std::strcmp(raw, label) == 0)
		{
			m_Texts.erase(i.first);
			break;
		}
	}
}

const char* CustomText::get(const char* label)
{
	for (decltype(auto) i : m_Texts)
	{
		auto raw = i.first.get();
		if (std::strcmp(raw, label) == 0)
		{
			return i.second.get();
		}
	}

	return nullptr;
}
