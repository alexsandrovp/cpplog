#pragma once

#include <windows.h>

class lock
{
	CRITICAL_SECTION h;
	lock(lock const &);
	lock const &operator=(lock const &);
public:
	lock() { InitializeCriticalSection(&h); }
	~lock() { DeleteCriticalSection(&h); }
	bool try_enter() { return 0 != TryEnterCriticalSection(&h); } 
	void enter() { EnterCriticalSection(&h); }
	void exit() { LeaveCriticalSection(&h); }
};