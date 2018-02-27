

// Xiangyu : This class is of no use, just for further extension


#include "Command.hpp"

Command::Command(std::string cmd, std::string desc)
	: command(cmd)
	, description(desc)
{}

Command::Command(std::vector<std::string> &cmd, std::string desc)
	: description(desc)
	, suffixes(std::vector<std::string>(cmd.begin()+1, cmd.end())
)
{
	command = cmd[0];
	// Xiangyu : NOT ELEGANT HERE
}


Command& Command::execute()
{
	if(command == "-d")
	{
		std::cout << "Show files" << std::endl;
	} 
	else if(command == "add")
	{
		std::cout << "Add file" << std::endl;;
	}

	else if(command == "delete")
	{
		std::cout << "Delete file" << std::endl;
	}


	return *this;

}


