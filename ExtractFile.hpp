//
// Created by Birkee on 2018/3/7.
//

#ifndef ECE180_EXTRACTFILE_HPP
#define ECE180_EXTRACTFILE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "DelFile.cpp"

class ExtractFile {
public:
    explicit ExtractFile(std::string& arch, std::string& file);

    size_t pointerSize = 10;
    size_t blockSize = 1 * 20;

    ExtractFile& extract();

protected:
    std::string archive;
    std::string filename;

};


#endif //ECE180_EXTRACTFILE_HPP
