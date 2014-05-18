#pragma once

#include "lock.h"

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

namespace logdetails {
	class logger
	{
		public:

			//this weird name is to avoid people calling this function directly
			//people should use the macro instead
			std::wostream &w5EAFA343EC974AA7AF201953459463DD(wchar_t type)
			{
				std::wstring stamp;
				get_current_date_time(stamp);
				unsigned long pid = get_current_process_id();
				unsigned long tid = get_current_thread_id();
				locker.enter();
				std::wclog << std::setfill(L'0')
					<< stamp << L' ' << type
					<< L" P" << std::setw(5) << pid
					<< L" T" << std::setw(5) << tid << L' ';
				return std::wclog;
			}

			void release()
			{
				std::wclog << std::endl;
				locker.exit();
			}

			static logger &Instance()
			{
				static logger instance;
				return instance;
			}

		private:

			lock locker;
			logger(logger const&) {};
			logger &operator=(logger const&) {};
			logger() {};

	#ifdef COMPILE_OS_WINDOWS

			static unsigned long get_current_process_id() { return GetCurrentProcessId(); }
			static unsigned long get_current_thread_id() { return GetCurrentThreadId(); }

			static void get_current_date_time(std::wstring &stamp)
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
				stamp = ss.str();
			}

	#else if __GNUC__

			static unsigned long get_current_process_id() { return getppid(); }
			static unsigned long get_current_thread_id() { return (unsigned long)syscall(224); }

			static void get_current_date_time(std::wstring &stamp)
			{
				wchar_t buf[80];
				time_t now = time(0);
				struct tm tstruct = *localtime(&now);
				wcsftime(buf, sizeof(buf), L"%Y-%m-%d %X", &tstruct);
				stamp = buf;
			}

	#endif
	};
}

#define LOGDEBUG(x)   do { logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'D') << x; logdetails::logger::Instance().release(); } while(false)
#define LOGINFO(x)    do { logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'I') << x; logdetails::logger::Instance().release(); } while(false)
#define LOGWARNING(x) do { logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'W') << x; logdetails::logger::Instance().release(); } while(false)
#define LOGERROR(x)   do { logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'E') << x; logdetails::logger::Instance().release(); } while(false)
#define LOGFATAL(x)   do { logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'F') << x; logdetails::logger::Instance().release(); } while(false)