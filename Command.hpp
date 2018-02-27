
// Xiangyu : This class is of no use, just for further extension

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <vector>
#include <string> 
#include <unordered_map>
#include <sstream>


class Command
{
public:
	explicit Command(std::string cmd, std::string desc);

	explicit Command(std::vector<std::string>& cmd, std::string desc);


	std::string command;

	std::string description;

	std::vector<std::string> suffixes;

	Command& execute();

};

