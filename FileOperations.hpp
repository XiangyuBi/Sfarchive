//
// Created by Birkee
//

#ifndef ECE180_FILEOPERATIONS_HPP
#define ECE180_FILEOPERATIONS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class FileOperations {
public:

    explicit FileOperations(std::string &arch);
    std::string archive;


    FileOperations& showAllFiles();
    FileOperations& find(std::vector<std::string>& strings);
    FileOperations& search(std::string& str);

protected:
    size_t pointerSize = 10;
    size_t blockSize = 20 * 1;





};


#endif //ECE180_FILEOPERATIONS_HPP
