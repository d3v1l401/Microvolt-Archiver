#pragma once
#include <iostream>
#include <string>
using namespace std;

class ATaker
{
private:
	
	static std::string TargetName;
	static std::string ActionType;
	static short int Argc;

	static std::string HelpMessage;

protected:


public:

	static void AssumeParameters(short int argc, const char* argv[]);
	static void Initialize();

};