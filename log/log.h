#pragma once

#include <mutex>
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
				locker.lock();
				std::wclog << std::setfill(L'0')
					<< get_current_date_time().c_str() << L' ' << type << L" P"
					<< std::setw(5) << get_current_process_id() << L" T"
					<< std::setw(5) << get_current_thread_id() << L' ';
				return std::wclog;
			}

			void release()
			{
				std::wclog << std::endl;
				if (locker.owns_lock()) locker.unlock();
			}

			static logger &Instance()
			{
				static logger instance;
				return instance;
			}

		private:

			std::mutex logmutex;
			std::unique_lock<std::mutex> locker;

			logger(logger const&){};
			logger& operator=(logger const&){};
			logger() : logmutex(), locker(logmutex, std::defer_lock) {};

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
}

#define LOGDEBUG(x)		logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'D') << x; logdetails::logger::Instance().release();
#define LOGINFO(x)		logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'I') << x; logdetails::logger::Instance().release();
#define LOGWARNING(x)	logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'W') << x; logdetails::logger::Instance().release();
#define LOGERROR(x)		logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'E') << x; logdetails::logger::Instance().release();
#define LOGFATAL(x)		logdetails::logger::Instance().w5EAFA343EC974AA7AF201953459463DD(L'F') << x; logdetails::logger::Instance().release();