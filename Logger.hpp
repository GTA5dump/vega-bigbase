#pragma once
#include "PrecompiledHeader.hpp"
namespace fs = std::filesystem;
namespace Vega
{
	class Logger {
	public:
		Logger();
		~Logger();
	public:
		void Raw(const char* format, ...);
		void Info(const char* format, ...);
		void Error(const char* format, ...);
		void Log(const char* type, const char* format, std::va_list args);
	public:
		std::pair<std::unique_ptr<char[]>*, std::size_t> GetMessages();
	public:
		std::istream& GetInput();
	private:
		std::vector<std::unique_ptr<char[]>> m_Messages;
	private:
		fs::path m_FilePath;
	private:
		std::ofstream m_File;
		std::ofstream m_Console;
	private:
		std::ifstream m_Input;
	};

	extern std::unique_ptr<Logger> g_Logger;
}
