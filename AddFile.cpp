//
// Created by Birkee
//

#include "AddFile.hpp"


AddFile::AddFile(std::vector <std::string> suffixes)
        : files(std::vector<std::string>(suffixes.begin()+1, suffixes.end()) )
{
    archive = suffixes[0];
    // No elegant here
}

void AddFile::add(std::string& filein) {
    std::string filename = archive + ".bin";

    std::ifstream in(filein, std::ios::in | std::ios::binary);
    long length;
    std::string contents;

    if (in)
    {
        in.seekg(0, std::ios::end);
        length = in.tellg();
        contents.resize(length);
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    }

    else
        throw(errno);


    if (exists_test(filename))
        addFileToArchive(filename, contents, length, filein);

    else
        generateNewArchive(filename, contents, length, filein);


}

AddFile &AddFile::addFiles() {

    for(auto &x : files) add(x);
    return *this;

}



std::string AddFile::generateHelper(const std::string &line) {
    std::string rline = "{" + line + "}";
    return rline;
}

std::string AddFile::generateLog(const std::string &filename, long &size, const std::string& block) {

    auto end = std::chrono::system_clock::now();

    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string time = std::ctime(&end_time);
    std::string log;
    log += generateHelper(filename);
    log += generateHelper(std::to_string(size) + " " + block);
    log += generateHelper(time);

    return log;

}

void AddFile::generateNewArchive(std::string &filein, std::string& content,  long& length, std::string& filename) {

    std::cout << "    ...Creating new archive: [ " << archive << " ]" <<  std::endl;

    std::ofstream outfile;
    outfile.open(filein, std::ios::out | std::ios::binary);
    outfile.seekp(pointerSize);
    outfile.write(content.c_str(), length);

    long block = ceil(double(length) / blockSize);

    auto headerStart = block * blockSize + 1;

    std::string blockHeader;
    for(int i = 0; i < block; i++)

    {
        blockHeader += std::to_string(i);
        blockHeader += " ";
    }

    // generate initial header for the file
    std::string header = generateLog(filename, length, blockHeader);
    header = "{ -1 }" + header;
    outfile.seekp(headerStart + pointerSize - 1);

    outfile.write(header.c_str(), header.size());

    outfile.seekp(std::ios::beg);
    std::string pointer = std::to_string(header.size());
    std::string comp(pointerSize-pointer.size(), '0');

    outfile.write((comp + pointer).c_str(), pointerSize);
    outfile.close();

}



/*
 *  1. {  ...  } first bracket: free blocks
 *  2. { { filename } {size , <blocks>} { time } }
 * */

std::vector<int> AddFile::getBlockForFile(std::string &filename, std::string &header)
{
    std::size_t found = 0;
    found = header.find(filename, found);
    std::vector<int> blocks;

    if (found != std::string::npos )
    {
        std::size_t start = header.find('{', found + 1) + 1;
        std::size_t end = header.find('}', start) - 1;
        std::string info = header.substr(start, end-start+1);     // "size  block1 block2 block3"
        std::stringstream ss(info);
        for(int i = 0; ss >> i; )
            blocks.push_back(i);

        return blocks;
    }

    else
        throw std::invalid_argument( "Cannot find such file" );
}


std::vector<int> AddFile::getAvailableBlock(std::string &header)
{

    std::size_t found = header.find('}');
    std::string info = header.substr(1, found-1);
    std::stringstream ss(info);
    std::vector<int> blocks;

    for(int i = 0; ss >> i; )
        blocks.push_back(i);
    return blocks;


}

std::string AddFile::getInformationForFile(std::string &filename, std::string &header) {

    std::size_t curPos;
    std::size_t endPos;
    curPos = header.find(filename);
    if (curPos != std::string::npos) {
        curPos = header.find('{', curPos);
        endPos = header.find(' ', curPos);

        std::string size = header.substr(curPos + 1, endPos - 1);

        curPos = header.find('{', endPos);
        endPos = header.find('}', curPos);

        std::string time = header.substr(curPos + 1, endPos - 1);

        std::string info = "Filename: " + filename + "\n"
                           + "Size: " + size + "\n"
                           + "Time" + time;

        return info;
    }
    else
        throw std::invalid_argument( "Cannot find such file" );
}




void AddFile::addFileToArchive(std::string &filein, std::string &content, long &length, std::string &filename) {
    std::cout << "    ...Existed archive: [ " << archive << " ]" <<  std::endl;
    std::fstream arcFile;
    arcFile.open(filein, std::ios::in | std::ios::out | std::ios::binary);

    // Get the pointer

    std::string pointer;
    pointer.resize(pointerSize);
    arcFile.read(&pointer[0], pointerSize);

    std::string::size_type sz;
    long headerLength = std::stol(pointer, &sz);

    arcFile.seekg(0, std::ios::end);
    long position = arcFile.tellg() - headerLength;

    // This position is important
    arcFile.seekg(position);

    std::string header;
    header.resize(headerLength);
    arcFile.read(&header[0], headerLength);


    auto f = header.find(filename);

    if(f != std::string::npos && header[f-1] == '{' && header[f+filename.size()] == '}')
    {
        std::cerr<< "File " << filename << " " << "existed!" << std::endl;
        return;
    }

    std::vector<int> avalBlock = getAvailableBlock(header);
    //std::vector<int> FileBlock = getBlockForFile(filename, header);


    if (avalBlock[0] == -1)
    {
        // append new
        //std::cout << "Äppend New";
        arcFile.seekp(position);
        arcFile.write(content.c_str(), length);
        int currentBlockNum = (position - pointerSize) / blockSize;
        int newBlockNum = ceil(double(length) / blockSize);
        long newPosition = (currentBlockNum + newBlockNum ) * blockSize + 1;

        std::string newBlock;

        for(auto i = currentBlockNum; i < currentBlockNum+newBlockNum; i++)
        {
            newBlock += std::to_string(i);
            newBlock += " ";
        }
        std::string newHeader = generateLog(filename, length, newBlock);
        header+= newHeader;
        arcFile.seekp(newPosition);
        arcFile.write(header.c_str(), header.size());

        std::string newPointerLength = std::to_string(header.size());
        std::string comp(pointerSize-newPointerLength.size(), '0');
        newPointerLength = comp + newPointerLength;

        arcFile.seekp(std::ios::beg);
        arcFile.write(newPointerLength.c_str(), pointerSize);
        arcFile.close();


    }
    else
    {
        // use the current first
        //std::cout << "Fill the first";
    }

}








