// log.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <log.h>

using namespace std;

logger& LOG = logger::Instance();

int _tmain(int argc, _TCHAR* argv[])
{
	LOG.info() << "oi";
	return 0;
}

