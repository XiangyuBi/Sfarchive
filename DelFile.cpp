//
// Created by Birkee on 2018/2/28.
//


#include "DelFile.hpp"

DelFile::DelFile(std::vector <std::string> suffixes)
            : files(std::vector<std::string>(suffixes.begin()+1, suffixes.end()) )
    {
      archive = suffixes[0];
      // No elegant here
    }




void DelFile::del(std::string &filein) {
      std::fstream arcFile;

     if(!AddFile::exists_test(archive + ".bin"))
    {
        std::cerr << "No such archive: " << archive  << std::endl;
        return;
    }
      arcFile.open(archive + ".bin", std::ios::in | std::ios::out | std::ios::binary);



      // Get the pointer

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



      auto f = header.rfind(filein);

      if(f != std::string::npos && header[f-1] == '{' && header[f+filein.size()] == '}')
      {
        auto tag = header.find("$Tag$", f+1) + 6;
        std::cout << "TAG" << header[tag] << std::endl;
        if( header[tag] != '0' ) {
          header[tag] = '0';
          auto findBlockStart = header.find('{', f+1);
          auto findBlockEnd = header.find('}', findBlockStart+1);
          findBlockStart = header.find(' ', findBlockStart);

          std::string BlocksDeleted = header.substr(findBlockStart, findBlockEnd - findBlockStart);

          // get the current empty block

          auto emptyBlockEnd = header.find('}', 0);

          std::string emptyBlock = header.substr(1, emptyBlockEnd - 1);


          if (emptyBlock == "-1") {
            emptyBlock = BlocksDeleted;
            header.erase(0, 3);
            //header = "{" + emptyBlock + header;
          }

          else {
            header.erase(0, emptyBlock.size()+1);
            emptyBlock += BlocksDeleted;
            //header = "{" + emptyBlock + header;
          }
          header = "{" + emptyBlock + header;

          arcFile.seekp(position);
          arcFile.write(header.c_str(), header.size());
          arcFile.seekp(std::ios::beg);
          std::string newPointer = std::to_string(header.size());
          std::string comp(pointerSize-newPointer.size(), '0');
          arcFile.write((comp + newPointer).c_str(), pointerSize);
          arcFile.close();
          return;

        }

        else
        {
          std::cerr << "No file to delete" << std::endl;
          return;
        }
      }
      else
      {
        std::cerr << "No file to delete" << std::endl;
        return;
      }







    }

    DelFile &DelFile::delFiles() {
      for(auto &x : files) del(x);
      return *this;
    }
