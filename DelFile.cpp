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
      auto e = header.rfind('{')+1;
      int totalBlock  = std::stoi(header.substr(e, header.size() - 1 - e));
       // bool resizable = false;
        int blockUsed = 0;

    std::cout << totalBlock << std::endl;


      if(f != std::string::npos && header[f-1] == '$' && header[f+filein.size()] == '}')
      {
        auto tag = header.find("$Tag$", f+1) + 6;
        if( header[tag] != '0' ) {
          header[tag] = '0';
          auto findBlockStart = header.find('{', f+1);
          auto findBlockEnd = header.find('}', findBlockStart+1);
          findBlockStart = header.find(' ', findBlockStart);

          std::string BlocksDeleted = header.substr(findBlockStart, findBlockEnd - findBlockStart);

          // get the current empty block

          auto emptyBlockEnd = header.find('}', 0);

          std::string emptyBlock = header.substr(1, emptyBlockEnd - 1);

           // std::cout << emptyBlock << std::endl;


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

            std::stringstream ff(emptyBlock);
            for(int i = 0; ff >> i; )
                blockUsed ++;



          header = "{" + emptyBlock + header;

          arcFile.seekp(position);
          arcFile.write(header.c_str(), header.size());
          arcFile.seekp(std::ios::beg);
          std::string newPointer = std::to_string(header.size());
          std::string comp(pointerSize-newPointer.size(), '0');
          arcFile.write((comp + newPointer).c_str(), pointerSize);
          arcFile.close();

            if (double(blockUsed) / totalBlock > 0.5) {
                std::cout << "resize" << std::endl;
                resize(header);
            }

          return;

        }

        else
        {
          std::cerr << "No file to delete" << std::endl;
            arcFile.close();
          return;
        }
      }
      else
      {
        std::cerr << "No file to delete" << std::endl;
          arcFile.close();
        return;
      }

    }





    DelFile &DelFile::delFiles() {
      for(auto &x : files) del(x);
      return *this;
    }




void DelFile::resize(std::string &header)
{
    std::string newArchive = archive + "temp" + ".bin";
    std::string newName = archive + "temp";
    std::ifstream originFile;
    std::string originName = archive + ".bin";
    originFile.open(originName, std::ios::in | std::ios::binary);

    size_t start=0;
    size_t end;
    std::string currentFile;
    std::string currentSize;
    std::string currentTime;

    size_t fileNum = 0;
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

        long sizeOfFile;
        ff >> sizeOfFile;

        for(int i = 0; ff >> i; )
            theBlock.push_back(i);

        std::string contents;
        std::string buffer;
        buffer.resize(blockSize);

        for(auto& x : theBlock)
        {
            originFile.seekg(x * blockSize + pointerSize);
            originFile.read(&buffer[0], blockSize);
            contents += buffer;
        }
        contents = contents.substr(0, sizeOfFile);


     //   size_t pos = contents.find(str);

        start = header.find('{', end + 1) + 1;
        end = header.find(' ', start + 1) - 1;
        currentSize = header.substr(start, end - start + 1);

        start = header.find('{', end + 1) + 1;
        end = header.find('}', start + 1) - 1;
        currentTime = header.substr(start, end - start + 1);
        AddFile newFile(newName);

        if(fileNum == 0) {
            newFile.generateNewArchive(newArchive, contents, sizeOfFile, currentFile, currentTime);
            fileNum ++;
        }
        else {
            newFile.addFileToArchive(newArchive, contents, sizeOfFile, currentFile, currentTime);
        }


    }
    originFile.close();
    int a = std::remove(originName.c_str());
    int b = std::rename(newArchive.c_str(), originName.c_str());
    //std::cout << a << std::endl;
    //std::cout << b << std::endl;

}


