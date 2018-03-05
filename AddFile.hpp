//
// Created by Birkee o
//


#ifndef ADDFILE_HPP
#define ADDFILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <chrono>
#include <ctime>

#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <cmath>       /* ceil */


//#include "Huffman.cpp"
//#include "huffman.h"

class AddFile
{
public:
    explicit AddFile(std::vector<std::string> suffixes);

    void add(std::string& filein);

    AddFile& addFiles();

    long blockSize = 1 * 20;

    long pointerSize = 10;

    inline static bool exists_test (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
/*
    static inline std::string tail(std::string const& source, size_t const length) {
        if (length >= source.size()) { return source; }
        return source.substr(source.size() - length);
    } // tail
*/

protected:
    std::string archive;

    std::vector<std::string> files;
    //std::string root = "./";





    std::string generateLog(const std::string &filename, long &size, const std::string& block);

    inline std::string generateHelper(const std::string& line);

    void generateNewArchive(std::string& filein, std::string& content,  long& length, std::string& filename);

    void addFileToArchive(std::string& filein, std::string& content, long& length, std::string& filename);



};




class Header
{
public:
    explicit Header(std::string& header);

    std::vector<int> getBlockForFile(std::string &filename);

    std::vector<int> getAvailableBlock();

    std::string getInformationForFile(std::string& filename);

    Header& parseAll();

    std::vector<std::string> filenames;

    std::string header;






};






#endif //ECE180_ADDFILE_HPP
