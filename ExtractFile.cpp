//
// Created by Birkee on 2018/3/7.
//

#include "ExtractFile.hpp"

ExtractFile::ExtractFile(std::string &arch, std::string &file)
    : archive(arch)
    , filename(file)
{
}

ExtractFile &ExtractFile::extract() {
    std::fstream arcFile;
    if(!AddFile::exists_test(archive + ".bin"))
    {
        std::cerr << "No such archive: " << archive  << std::endl;
        return *this;
    }
    arcFile.open(archive + ".bin", std::ios::in | std::ios::binary);

    std::string pointer;
    std::cout << pointer << std::endl;
    pointer.resize(pointerSize);
    arcFile.read(&pointer[0], pointerSize);
    std::string::size_type sz;
    long headerLength = std::stol(pointer, &sz);
    arcFile.seekg(0, std::ios::end);
    long oldEnd = arcFile.tellg();
    long position = oldEnd - headerLength;
    arcFile.seekg(position);
    std::string header;
    header.resize(headerLength);
    arcFile.read(&header[0], headerLength);

    auto f = header.rfind(filename);

    if(f != std::string::npos && header[f-1] == '$' && header[f+filename.size()] == '}')
    {
        auto tag = header.find("$Tag$", f+1) + 6;
        std::cout << "TAG" << header[tag] << std::endl;
        if( header[tag] != '0' ) {
            header[tag] = '0';
            auto findBlockStart = header.find('{', f+1)+1;
            auto findBlockEnd = header.find('}', findBlockStart+1);
            std::string BlocksToExtract = header.substr(findBlockStart, findBlockEnd - findBlockStart);
            std::stringstream ss(BlocksToExtract);
            size_t sizeOfFile;
            ss >> sizeOfFile;

            std::vector<int> blocks;
            for(int i = 0; ss >> i; )
                blocks.push_back(i);

            std::string contents;
            std::string buffer;
            buffer.resize(blockSize);

            for(auto& x : blocks)
            {
                arcFile.seekg(x * blockSize + pointerSize);
                arcFile.read(&buffer[0], blockSize);
                contents += buffer;
            }

            contents = contents.substr(0, sizeOfFile);
            std::cout << "Filename: " << filename << std::endl;
            std::cout << "File content: [" << std::endl;
            std::cout << contents << std::endl;
            std::cout << "]" << std::endl;


            arcFile.close();
            return *this;

        }

        else
        {
            std::cerr << "No file to extract" << std::endl;
            arcFile.close();
            return *this;
        }
    }
    else
    {
        std::cerr << "No file to extract" << std::endl;
        arcFile.close();
        return *this;
    }
}


