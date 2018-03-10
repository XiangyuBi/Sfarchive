#include "Options.cpp"
#include <vector>
#include <string>
#include <iostream>

int main(int argc, const char* argv[])
{
  std::string first_arge;
  std::vector<std::string> all_args;
  if (argc > 1)
  {
    first_arge = argv[1];
    all_args.assign(argv + 1, argv + argc);
  }
  std::vector<std::string> opts{"-v", "-h", "-l", "add", "del", "-e", "find" };

  std::vector<bool> exts{false, false, true, true, true, true};

  std::vector<std::string> decs{"Show current version of project",
                                "Get help for options", 
                                "Show current files in the archive",
                                "Add a file into archive",
                                "Delete a file from archive",
                                "Extract file",
                                "Find files in archive containing a string"
                                };

  Options Option(std::string(argv[0]), opts, decs, exts);
  Parse parser(Option);
  parser.parse(all_args, argc);

  return 0;
  }
