#include "stdafx.h"

#include "log.h"

#include <sstream>
#include <iomanip>

#if _WIN32 || _WIN64
#include <windows.h>
#define COMPILE_OS_WINDOWS
#endif

using namespace std;

#ifdef COMPILE_OS_WINDOWS

unsigned long get_current_process_id() { return GetCurrentProcessId(); }
unsigned long get_current_thread_id() { return GetCurrentThreadId(); }

wstring get_current_date_time()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	wostringstream ss;
	ss << setfill(L'0')
		<< setw(4) << st.wYear << L'-'
		<< setw(2) << st.wMonth << L'-'
		<< setw(2) << st.wDay << L' '
		<< setw(2) << st.wHour << L':'
		<< setw(2) << st.wMinute << L':'
		<< setw(2) << st.wSecond << L'.'
		<< setw(3) << st.wMilliseconds;
    return ss.str();
}

#else if __GNUC__

#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>

unsigned long get_current_process_id() { return getppid(); }
unsigned long get_current_thread_id() { return (unsigned long)syscall(224); }

wstring get_current_date_time()
{
	wchar_t buf[80];
    time_t now = time(0);
	struct tm tstruct = *localtime(&now);
    wcsftime(buf, sizeof(buf), L"%Y-%m-%d %X", &tstruct);
    return buf;
}

#endif

wostream &log(wchar_t type)
{
	wclog << endl << setfill(L'0')
		<< get_current_date_time().c_str() << L' ' << type << L" P"
		<< setw(5) << get_current_process_id() << L" T"
		<< setw(5) << get_current_thread_id() << L' ';

	return wclog;
}

wostream &debug() { return log(); }
wostream &info() { return log(L'I'); }
wostream &warning() { return log(L'W'); }
wostream &error() { return log(L'E'); }
wostream &fatal() { return log(L'F'); }