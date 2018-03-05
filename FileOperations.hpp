//
// Created by Birkee
//

#ifndef ECE180_FILEOPERATIONS_HPP
#define ECE180_FILEOPERATIONS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class FileOperations {
public:

    FileOperations(std::string &arch);
    std::string archive;


    FileOperations& showAllFiles();

protected:
    size_t pointerSize = 10;





};


#endif //ECE180_FILEOPERATIONS_HPP
