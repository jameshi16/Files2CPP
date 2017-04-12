#include "commandUtils.h"
template <class N> //Template with a structure N (N would be passed around)
std::map<std::string, CommandPack<N>> CommandUtilities::commandStore = {};

bool CommandUtilities::processArgv(char** argv, int argc)
{
  std::string arguments{}; //creates a std::string out of the arguments

  for (auto iii = 0; iii < argc; iii++) //transfers all of argv into the string
    arguments += argv[iii];
}
