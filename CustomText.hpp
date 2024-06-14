#pragma once
#include "PrecompiledHeader.hpp"

class CustomText
{
private:
	std::unordered_map<std::unique_ptr<char[]>, std::unique_ptr<char[]>> m_Texts;
public:
	void initialize();

	void reg(const char* label, const char* text);
	void unreg(const char* label);

	const char* get(const char* label);
};

extern CustomText g_CustomText;