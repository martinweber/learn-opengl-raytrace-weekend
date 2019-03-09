#ifndef STANDARD_OUT_SINK_H_
#define STANDARD_OUT_SINK_H_

#include <string>
#include <iostream>
#include <iomanip>

#include <windows.h>

#include <g3log\logmessage.hpp>

namespace tachyon
{
	struct StandardOutSink 
	{
		static int minLogLevel;

		enum FG_COLOR { YELLOW = 33, RED = 31, GREEN = 32, WHITE = 97 };
		
		FG_COLOR getColor(const LEVELS level) const
		{
			if (level.value == WARNING.value)
				return FG_COLOR::YELLOW;

			if (level.value == g3::kDebugValue)
				return FG_COLOR::GREEN;

			if (g3::internal::wasFatal(level))
				return FG_COLOR::RED;

			return FG_COLOR::WHITE;
		}

		static void setLogLevel(const std::string& loglevel)
		{
			if (loglevel == "debug") minLogLevel = g3::kDebugValue;
			else if (loglevel == "info") minLogLevel = g3::kInfoValue;
			else if (loglevel == "warn") minLogLevel = g3::kWarningValue;
			else if (loglevel == "error") minLogLevel = g3::kFatalValue;
			else if (loglevel == "fatal") minLogLevel = g3::kInternalFatalValue;
			else LOG(FATAL) << "Unknown LogLevel requested: " << loglevel;
		}

		void ReceiveLogMessage(g3::LogMessageMover logEntry)
		{
			auto level = logEntry.get()._level;

			// skip messages that are below the minimum log-level
			if (level.value < minLogLevel)
				return;

			auto color = getColor(level);

			g3::LogMessage entry = logEntry.get();
#ifdef WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (entry._level.value)
			{
			case g3::kWarningValue: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
			case g3::kDebugValue: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
			default: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
			}
#endif

			std::cout << std::setw(8) << std::left << entry.level() << " ";


#ifdef WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE  );
#endif

			std::string function = entry.function();

			size_t pos = function.rfind("::");
			if (pos == std::string::npos) pos = 0;
			else
			{
				pos = function.rfind("::", pos-2);
				if (pos == std::string::npos) pos = 0;
				else pos += 2;
			}
			std::string func = function.substr(pos);

			std::cout << func  << ":" << std::setw(4) << entry.line() << " ";

#ifdef WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#endif
			std::cout << entry.message() << std::endl;
		}
	};

	int StandardOutSink::minLogLevel = g3::kDebugValue; // log everything by default
}

#endif 
