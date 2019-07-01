#include "ATaker.h"
#include "CColors/advconsole.h"
#include "Archives/Archive.h"
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;
#pragma warning (disable: 4996)

#define ARG_ACTION		1
#define ARG_KOMDIRNAME	2
#define ARG_FILTER		3
#define ARG_FILTERVALUE 4

std::string ATaker::ActionType;
std::string ATaker::TargetName;
short int	ATaker::Argc		= NULL;

std::string ATaker::HelpMessage = "[ HELP ]\n\n"
								  "- COMMANDS -\n\n"
								  "-h: Will print this help message.\n"
								  "-u: Will unpack the selected archive.\n"
								  "-p: Will pack the selected folder.\n";

void ATaker::AssumeParameters(short int argc, const char* argv[]) {
	if (!argv)
		cout << Color(AC_RED) << "No parameters parsed.\n" << Color(AC_YELLOW) << "Please use -h if you don't know how to use MicroVolt Archiver.\n";
	if (!argc)
		cout << Color(AC_RED) << "No parameters parsed.\n" << Color(AC_YELLOW) << "Please use -h if you don't know how to use MicroVolt Archiver.\n";
	
	switch (argc) {
	case 2:
		cout << Color(AC_YELLOW) << ATaker::HelpMessage;
		break;
	case 3:
		ATaker::Argc = 3;
		if (argv[ARG_ACTION] != nullptr && argv[ARG_KOMDIRNAME] != nullptr) {
			ATaker::ActionType = (char*)(argv[ARG_ACTION]);
			ATaker::TargetName = (char*)(argv[ARG_KOMDIRNAME]);
			cout << Color(AC_WHITE) << "Selected Target: " << Color(AC_GREEN) << ATaker::TargetName << "\n";
		} else 
			cout << Color(AC_RED) << "No parameters parsed, despite argc says you did?" << Color(AC_WHITE) << "\n";
		
		break;
	default:
		cout << Color(AC_RED) << "No parameters parsed.\n" << Color(AC_YELLOW) << "Please use -h if you don't know how to use MicroVolt Archiver.\n";
		break;
	}
}

void ATaker::Initialize()
{
	switch (ATaker::Argc) {
	case 3:
		if (!ATaker::ActionType.compare("-u")) {
			Archive::Init(ACTION_UNPACK, ATaker::TargetName);
			Archive* _handler = new Archive();

			if (_handler->Open())
				_handler->ExtractAll();

			_handler->~Archive();

			std::cout << Color(AC_WHITE) << "\n\tProcess " << Color(AC_GREEN) << "completed" << Color(AC_WHITE) << "!\n";
			break;
		} else if (!ATaker::ActionType.compare("-p")) {
			Archive::Init(ACTION_PACK, ATaker::TargetName);
			ArchWriter* _handler = new ArchWriter();

			_handler->PackAll(ATaker::TargetName, ATaker::TargetName + "_pack");

			_handler->~ArchWriter();
			break;
		} else {
			cout << Color(AC_RED) << "Unrecognized parameter parsed in <" << Color(AC_MAGENTA) << " COMMAND " << Color(AC_RED) << "> position.\n";
			break;
		}
		break;
	default:
		//cout << Color(AC_RED) << "No commands to initialize.\n";
		break;
	}
}