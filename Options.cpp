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
            if(inArgv.size() == i + 1)
            {
                std::cerr << "[Error]    Wrong delete input" << std::endl;
                break;
            }
            //  ADD CLASS HERE
            std::vector<std::string> files(inArgv.begin() + i + 1, inArgv.end());
            for(auto & x : files) std::cout << x <<std::endl;
            AddFile Add(files);
            Add.addFiles();
            break;

        }

        else if (inArgv[i] == "delete")
        {
            if(inArgv.size() == i + 1)
            {
                std::cerr << "[Error]    Wrong delete input"<< std::endl;
                break;
            }
            //  DELETE CLASS HERE
            std::vector<std::string> files(inArgv.begin() + i + 1, inArgv.end());
            //for(auto & x : files) std::cout << x <<std::endl;
            break;
        }

        else if (inArgv[i] == "display" || "-d")
        {
            if(inArgv.size() == i + 1)
            {
                std::cerr << "[Error]    Specify the archive name: [-d <archive name>]"<< std::endl;
                break;
            }
            std::cout << "display class here" << std::endl;
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


