#ifndef COMMANDUTILS_H
#define COMMANDUTILS_H

//System includes
#include <iostream>
#include <string>
#include <map>

template <class N> //Template with a structure N (N would be passed around)
struct CommandPack
{
  typedef int* Command(std::string, N&); //Typedef a fuction accepting std::string and arbitary struct N
  CommandPack()=delete; //deletes default constructor
  CommandPack(Command p_command) {command = p_command;} //Creates a CommandPack based on the passed function

  CommandPack<N>& operator=(const CommandPack<N>&)=default;
  Command command = 0; //creates a command pointer pointing to nothing
};

namespace CommandUtilities
{
  /**
   * Processes the string and calls the respective commands
   * @param  arguments  The entire argv
   * @param  argc       The number of elements
   * @param  pointerToN The pointer to N that is shared among all of the commands (common flags)
   * @return            True or false depending if all or none of the commands worked
   */
  template <class N>
  const bool processArgv(const char** argv, const int argc, N *pointerToN);

  /**
   * Attempts to aquire the arguments after the flag
   * Priority: Double quotes, Single quotes, and then empty
   * @param  pos_of_flag  The position of where the last character representing the flag is
   * @param  fullArgument The string of all of the arguments passed
   * @return              The arguments for said flag.
   * @throw               A runtime error if the flag manages to exceed expected values
   */
  const std::string gainArguments(const unsigned int pos_of_flag, const std::string& fullArgument);

  //Global store of commands
  template <class N> //Template with a structure N (N would be passed around)
  extern std::map<std::string, CommandPack<N>> commandStore; //a map containing the flag name in string, and the command pack
                                                             //if the flag is "", it will be executed after all of the flag names has been processed.
};

#endif
