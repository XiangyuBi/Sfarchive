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
    while(header.find("$FILENAME", start+1) != std::string::npos) {
        start = header.find("$FILENAME$", start+1) + std::string("$FILENAME$").length();

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

FileOperations &FileOperations::find(std::vector<std::string> &strings) {
   // for(size_t i = 0; i < strings.size(); i++) strings[i] = strings[i].substr(1, strings[i].size()-2);

    std::string filein = archive + ".bin";
    std::ifstream in(filein, std::ios::in | std::ios::binary);
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

    size_t start=0;
    size_t end;
    std::string currentFile;
    std::string currentSize;
    std::string currentTime;

    while(header.find("$FILENAME", start+1) != std::string::npos) {
        start = header.find("$FILENAME$", start+1) + std::string("$FILENAME$").length();
        if( header[header.find("$Tag$", start+1) + 6] != '1' )
            continue;
        end = header.find('}', start + 1) - 1;
        currentFile = header.substr(start, end - start + 1);

        bool flag = false;
        for( auto& str : strings)
            if(currentFile.find(str) != std::string::npos)
            {
                flag = true;
                break;
            }

        if (!flag)
            continue;

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

FileOperations &FileOperations::search(std::string &str) {

    std::string filein = archive + ".bin";
    std::ifstream in(filein, std::ios::in | std::ios::binary);
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


    size_t start=0;
    size_t end;
    std::string currentFile;
    std::string currentSize;
    std::string currentTime;
    //  std::cout << header.find("{{", start+1) << std::endl;
    while(header.find("$FILENAME", start+1) != std::string::npos) {
        start = header.find("$FILENAME$", start+1) + std::string("$FILENAME$").length();

        if( header[header.find("$Tag$", start+1) + 6] != '1' )
            continue;
        end = header.find('}', start + 1) - 1;

        currentFile = header.substr(start, end - start + 1);

        auto findBlockStart = header.find('{', start+1)+1;
        auto findBlockEnd = header.find('}', findBlockStart+1);
        std::string Blocks = header.substr(findBlockStart, findBlockEnd - findBlockStart);
        findBlockStart = header.find(' ', findBlockStart);
        std::vector<int> theBlock;
        std::stringstream ff(Blocks);

        size_t sizeOfFile;
        ff >> sizeOfFile;

        for(int i = 0; ff >> i; )
            theBlock.push_back(i);

        std::string contents;
        std::string buffer;
        buffer.resize(blockSize);

        for(auto& x : theBlock)
        {
            in.seekg(x * blockSize + pointerSize);
            in.read(&buffer[0], blockSize);
            contents += buffer;
        }
        contents = contents.substr(0, sizeOfFile);


        size_t pos = contents.find(str);

        start = header.find('{', end + 1) + 1;
        end = header.find(' ', start + 1) - 1;
        currentSize = header.substr(start, end - start + 1);

        start = header.find('{', end + 1) + 1;
        end = header.find('}', start + 1) - 1;
        currentTime = header.substr(start, end - start + 1);



        if(pos != std::string::npos) {
            std::cout << "Filename: " << currentFile << std::endl;
            std::cout << "    Position found: " << pos << std::endl;
            std::cout << "    Size: " << currentSize << std::endl;
            std::cout << "    Time: " << currentTime << std::endl;
        }
        else
            continue;


    }
    in.close();
    return *this;


}






