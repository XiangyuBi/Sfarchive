//
// Created by Birkee o
//


#ifndef ADDFILE_HPP
#define ADDFILE_HPP

#include <string>
#include <vector>
//#include "Huffman.cpp"
//#include "huffman.h"


class AddFile
{
public:
    explicit AddFile(std::vector<std::string> suffixes);
    AddFile& addFiles();


protected:
    std::string archive;
    std::vector<std::string> files;
    std::string root = "./";

};





#endif //ECE180_ADDFILE_HPP
