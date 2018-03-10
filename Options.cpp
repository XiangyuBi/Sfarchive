#include "Options.hpp"




Options::Options(std::string prog) 
	: program(prog)
{
}

Options::Options(std::string prog, std::vector<std::string> opts, std::vector<std::string> desc)
	: options(opts)
	, program(prog)
	, descriptions(desc)
{
}

Options::Options(std::string prog, std::vector<std::string> opts, std::vector<std::string> desc,
		std::vector<bool> exts)
	: options(opts)
	, program(prog)
	, descriptions(desc)
	, extensions(exts)
{
}

inline Options& Options::addOption(std::string option, std::string desc, bool extendable)
{
	options.push_back(option);
	descriptions.push_back(desc);
	extensions.push_back(extendable);
	return *this;
}


std::string Options::str(size_t indent) const
  {
    std::ostringstream oss;
    std::string i(indent, ' ');
    oss << program << "  " << "[Options]:" << std::endl;;

    for (size_t j = 0; j < options.size(); j++)
    {
    	oss << i << options[j] << i << descriptions[j] << std::endl << std::endl;
    }
    return oss.str();
  }



  Parse::Parse(Options &opts)
  	:opts(opts)
  {}

  Parse& Parse::parse(std::vector<std::string> inArgv, int inArgc)  
  {
      size_t i;
      for(i = 0; i < size_t(inArgc)-1; i++)
    {

      if (std::find(opts.options.begin(), opts.options.end(), inArgv[i]) != opts.options.end())
      {
        if(inArgv[i] == "-v") std::cout << "SFarchiver Version " << opts.version << std::endl;

        else if(inArgv[i] == "-h") std::cout << opts.str(4) << std::endl;

        else if (inArgv[i] == "add")
        {
            if(inArgv.size() == i + 2)
            {
                std::cerr << "[Error]    Wrong add input" << std::endl;
                break;
            }
            //  ADD CLASS HERE
            std::vector<std::string> files(inArgv.begin() + i + 1, inArgv.end());
          //  for(auto & x : files) std::cout << x <<std::endl;
            AddFile Add(files);
            Add.addFiles();
            break;

        }

        else if (inArgv[i] == "del")
        {
            if(inArgv.size() == i + 2)
            {
                std::cerr << "[Error]    Wrong delete input"<< std::endl;
                break;
            }
            //  DELETE CLASS HERE
            std::vector<std::string> files(inArgv.begin() + i + 1, inArgv.end());
            //for(auto & x : files) std::cout << x <<std::endl;

            DelFile Del(files);
            Del.delFiles();
            break;
        }

            else if (inArgv[i] == "-e")
        {
            if(inArgv.size() == i + 2 || inArgv.size() >= i + 4)
            {
                std::cerr << "[Error]    Wrong extract input, only one file accepted one time"<< std::endl;
                break;
            }

            std::string arch = inArgv[i+1];
            std::string file = inArgv[i+2];

            ExtractFile ex(arch, file);
            ex.extract();
            break;
        }

        else if (inArgv[i] == "-l")

        {

            if(inArgv.size() == i + 1)
            {
                std::cerr << "[Error]    Specify the archive name: [-l <archive name> (\"string\")]"<< std::endl;
                break;
            }

            else if(inArgv.size() == i + 2)
            {
                std::string arch = inArgv[1];
                FileOperations fileop(arch);
                fileop.showAllFiles();
                break;

            }
            else if( inArgv.size() > i + 2)
            {
                std::string arch = inArgv[1];
                std::vector<std::string> strings(inArgv.begin() + i + 2, inArgv.end());
                std::cout << arch << std::endl;
                FileOperations fileop(arch);
                fileop.find(strings);

                // for(auto& x : strings) std::cout << x << std::endl;
                break;

            }
        }

        else if (inArgv[i] == "find")
        {
            if(inArgv.size() == i + 2 || inArgv.size() >= i + 4)
            {
                std::cerr << "[Error]    Wrong extract input, only one file accepted one time"<< std::endl;
                break;
            }

            std::string arch = inArgv[i+1];
            std::string str = inArgv[i+2];
            FileOperations fileop(arch);
            fileop.search(str);

            break;
        }


      }
      else
      {
        std::cerr << "[Error]    Unrecognized option: " << inArgv[i] << std::endl;
        std::cerr << "           Try -h to get help of usage" << std::endl;
        break;
      }
      

    }
  	return *this;
  }


