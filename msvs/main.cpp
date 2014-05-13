// log.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>

#include <log.h>

using namespace std;

void func1(const wstring &s)
{
	Sleep(1011);
	LOGWARNING("function 1: string: " << s << " length: " << s.length());
}

void func2(const wstring &s)
{
	Sleep(1000);
	LOGFATAL("function 2: string: " << s << " length: " << s.length());
}

int _tmain(int argc, _TCHAR* argv[])
{
	LOGINFO("oi");
	
	thread t1(func1, L"aiaiaiai");
	thread t2(func2, L"ioioio");

	LOGDEBUG("before joining");

	t1.join();
	t2.join();

	LOGINFO("the end");

	return 0;
}

