//Options class
//Author: Birkee
//22/2/2018

#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>
#include <vector>
#include <string> 
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <queue>

//#include "Command.cpp"
//#include "DelFile.cpp"
#include "FileOperations.cpp"
#include "ExtractFile.cpp"

class Options
{
public:
	explicit Options(std::string prog);

	explicit Options(std::string prog, std::vector<std::string> opts, std::vector<std::string> desc);

	explicit Options(std::string prog, std::vector<std::string> opts, std::vector<std::string> desc, 
		std::vector<bool> exts);

	const std::string version = "1.0";

	std::vector<std::string> options;
	
	std::string program;

	std::vector<std::string> descriptions;

	std::vector<bool> extensions;

	//std::queue<Command> commmands;


	Options& addOption(std::string option, std::string desc, bool extendable);

	std::string str(size_t indent) const;

  

};

class Parse 
{
public:

	explicit Parse(Options &opts);
	Parse& parse(std::vector<std::string> inArgv, int inArgc);
	//Parse& execute();

private:
	Options opts;

};





#endif