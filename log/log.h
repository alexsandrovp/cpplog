#pragma once

#include <iostream>

std::wostream &log(wchar_t type = L'D');
std::wostream &debug();
std::wostream &info();
std::wostream &warning();
std::wostream &error();
std::wostream &fatal();