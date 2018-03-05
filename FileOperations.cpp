//
// Created by Birkee
//

#include "FileOperations.hpp"

FileOperations::FileOperations(std::string &arch)
    : archive(arch)
{
}

FileOperations &FileOperations::showAllFiles() {


    std::string filename = archive + ".bin";
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if(!in)
    {
        std::cerr << "No archive: " << archive << std::endl;
        return *this;
    }

    std::string pointer;
    pointer.resize(pointerSize);
    in.read(&pointer[0], pointer.size());
    std::string::size_type sz;
    long pt = std::stoi(pointer, &sz);
    in.seekg(0, std::ios::end);
    long oldEnd = in.tellg();
    long position = oldEnd - pt;
    in.seekg(position);
    std::string header;
    header.resize(pt);
    in.read(&header[0], pt);
    in.close();
   // std::cout << header << std::endl;

    size_t start=0;
    size_t end;
    std::string currentFile;
    std::string currentSize;
    std::string currentTime;
  //  std::cout << header.find("{{", start+1) << std::endl;
    while(header.find("{{", start+1) != std::string::npos) {
        start = header.find("{{", start+1) + 2;

        if( header[header.find("$Tag$", start+1) + 6] != '1' )
            continue;
        end = header.find('}', start + 1) - 1;


        currentFile = header.substr(start, end - start + 1);

        start = header.find('{', end + 1) + 1;
        end = header.find(' ', start + 1) - 1;
        currentSize = header.substr(start, end - start + 1);

        start = header.find('{', end + 1) + 1;
        end = header.find('}', start + 1) - 1;
        currentTime = header.substr(start, end - start + 1);

        std::cout << "Filename: " << currentFile << std::endl;
        std::cout << "    Size: " << currentSize << std::endl;
        std::cout << "    Time: " << currentTime << std::endl;
    }

    return *this;
}
