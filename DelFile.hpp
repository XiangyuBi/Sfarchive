//
// Created by Birkee on 2018/2/28.
//

#ifndef ECE180_DELFILE_HPP
#define ECE180_DELFILE_HPP

#include "AddFile.cpp"


class DelFile
{
public:
    explicit DelFile(std::vector<std::string> suffixes);

    void del(std::string& filein);


    DelFile& delFiles();

    long blockSize = 1 * 20;

    long pointerSize = 10;


protected:
    std::string archive;
    std::vector<std::string> files;

};

#endif //ECE180_DELFILE_HPP
