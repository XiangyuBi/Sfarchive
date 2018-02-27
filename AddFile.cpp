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

AddFile &AddFile::addFiles() {
    std::string path = root + archive + "/";
    //std::FILE* readFile;
    std::string filename = "test.txt";
    std::string exptname = "test.sfa";
    //const char *from = filename.c_str();
    //const char *to = exptname.c_str();
    //huffman::encode("test.sfa", "test.txt");
    //huffman::decode("test1.txt","test.sfa");

    //for(auto& x : huffman1.leaf) std::cout << int(x) << std::endl;
    //huffman1.encode("test.sfa", "test.txt");
    //huffman1.decode("test1.txt", "test.sfa");

    return *this;
}


