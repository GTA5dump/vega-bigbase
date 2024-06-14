#include "PrecompiledHeader.hpp"
#include "Main.hpp"

namespace Vega
{
	std::unique_ptr<Logger> g_Logger;
	Logger::Logger() {
		try {
			m_FilePath.append(std::getenv("appdata")); m_FilePath.append(MENUNAME_NORMAL);
			if (!std::filesystem::exists(m_FilePath))
				std::filesystem::create_directory(m_FilePath);
			m_FilePath.append(MENUNAME_NORMAL ".log");
		}
		catch (std::filesystem::filesystem_error const&) {
			g_Logger->Error("std::filesystem error!");
		}
		if (!AttachConsole(GetCurrentProcessId())) {
			AllocConsole();
		}
		SetConsoleTitleA(XorString(MENUNAME_NORMAL));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		m_Console.open("CONOUT$");
		m_Input.open("CONIN$");
		m_File.open(m_FilePath, std::ios_base::out | std::ios_base::app);
	}
	Logger::~Logger() {
		fclose(stdout);
		FreeConsole();
	}
	void Logger::Raw(const char* format, ...) {
		std::va_list args{};
		va_start(args, format);
		auto time = std::time(nullptr);
		auto tm = std::localtime(&time);
		char prefix[64] = {};
		std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
		auto messageBuffer = std::make_unique<char[]>(messageLength);
		std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
		std::vsnprintf(messageBuffer.get(), messageLength, format, args);
		m_Console << prefix << messageBuffer.get() << std::endl;
		m_Messages.push_back(std::move(messageBuffer));
		va_end(args);
	}
	void Logger::Info(const char* format, ...) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::va_list args{};
		va_start(args, format);
		Log("Info", format, args);
		va_end(args);
	}
	void Logger::Error(const char* format, ...) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::va_list args{};
		va_start(args, format);
		Log("Error", format, args);
		va_end(args);
		Vega::g_ShouldExit = true;
	}
	void Logger::Log(const char* type, const char* format, std::va_list args) {
		auto time = std::time(nullptr);
		auto tm = std::localtime(&time);
		char prefix[64] = {};
		std::snprintf(prefix, sizeof(prefix) - 1, "%02d:%02d:%02d | %s | ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);
		std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
		auto messageBuffer = std::make_unique<char[]>(messageLength);
		std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
		std::vsnprintf(messageBuffer.get(), messageLength, format, args);
		m_File << prefix << messageBuffer.get() << std::endl;
		m_Console << prefix << messageBuffer.get() << std::endl;
		m_Messages.push_back(std::move(messageBuffer));
	}
	std::pair<std::unique_ptr<char[]>*, std::size_t> Logger::GetMessages() {
		return std::make_pair(m_Messages.data(), m_Messages.size());
	}
	std::istream& Logger::GetInput() {
		return m_Input;
	}
}
