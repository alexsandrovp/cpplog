// log.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <log.h>

int _tmain(int argc, _TCHAR* argv[])
{
	logger::fatal() << "oi";
	return 0;
}

