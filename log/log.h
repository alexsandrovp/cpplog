#pragma once

#include <iostream>
#include <iomanip>

#if _WIN32 || _WIN64
#include <sstream>
#include <windows.h>
#define COMPILE_OS_WINDOWS
#else if __GNUC__
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#endif

class logger
{
    public:

		template<typename T> std::wostream& operator <<(const T& thing) { return std::wclog << thing; }

		logger &debug() { return write(); }
		logger &info() { return write(L'I'); }
		logger &warning() { return write(L'W'); }
		logger &error() { return write(L'E'); }
		logger &fatal() { return write(L'F'); }

		static logger &Instance()
		{
			static logger instance;
			return instance;
		}

    private:
		logger(){};
		logger(logger const&){};
		logger& operator=(logger const&){};

		logger &write(wchar_t type = L'D')
		{
			this->Instance() << L"\n" << std::setfill(L'0')
				<< get_current_date_time().c_str() << L' ' << type << L" P"
				<< std::setw(5) << get_current_process_id() << L" T"
				<< std::setw(5) << get_current_thread_id() << L' ';
			return this->Instance();
		}

#ifdef COMPILE_OS_WINDOWS

		static unsigned long get_current_process_id() { return GetCurrentProcessId(); }
		static unsigned long get_current_thread_id() { return GetCurrentThreadId(); }

		static std::wstring get_current_date_time()
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			std::wostringstream ss;
			ss << std::setfill(L'0')
				<< std::setw(4) << st.wYear << L'-'
				<< std::setw(2) << st.wMonth << L'-'
				<< std::setw(2) << st.wDay << L' '
				<< std::setw(2) << st.wHour << L':'
				<< std::setw(2) << st.wMinute << L':'
				<< std::setw(2) << st.wSecond << L'.'
				<< std::setw(3) << st.wMilliseconds;
			return ss.str();
		}

#else if __GNUC__

		static unsigned long get_current_process_id() { return getppid(); }
		static unsigned long get_current_thread_id() { return (unsigned long)syscall(224); }

		static wstring get_current_date_time()
		{
			wchar_t buf[80];
			time_t now = time(0);
			struct tm tstruct = *localtime(&now);
			wcsftime(buf, sizeof(buf), L"%Y-%m-%d %X", &tstruct);
			return buf;
		}

#endif
};