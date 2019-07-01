#include <iostream>
#include "ATaker.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "CColors/advconsole.h"
using namespace AdvancedConsole;

#define ENTRY_BANNER  "[ uKOM Archiver ]\nMade by d3vil401 (http://d3vsite.org)\nESEmu Project (http://esemuproject.com)\nCompiled at "

int main(int argc, const char* argv[]) {
	SetConsoleTitle("Microvolt Archiver");

	cout << Color(AC_BLACK, AC_FOREGROUND) << Color(AC_WHITE, AC_BACKGROUND) << "[ Microvolt Archiver ]" << Color(AC_WHITE, AC_FOREGROUND) << Color(AC_BLACK, AC_BACKGROUND) << "\n"
		 << Color(AC_WHITE) << "Made by " << Color(AC_RED) << "d3v" << Color(AC_WHITE) << "il401 (" << Color(AC_CYAN) << "https://d3vsite.org/" << Color(AC_WHITE) << ")\n"
		 << Color(AC_WHITE) << "Compiled at " << Color(AC_GREEN) << __DATE__ << " " << __TIME__ << Color(AC_WHITE) << "\n"
		 << Color(AC_WHITE) << "\nAn unpacker/packer software for " << Color(AC_GREEN) << "Microvolt Surge" << Color(AC_WHITE) << " MMOFPS's '" << Color(AC_MAGENTA) << "*.dip" << Color(AC_WHITE) << "'.\n"
		 << Color(AC_CYAN) << "Microvolt Archiver is a free software under GPLv3 license.\nGive credits to the original author whenever you use part of this product.\n\n";

	ATaker::AssumeParameters(argc, argv);
	ATaker::Initialize();

	getchar();
	return 0;
}