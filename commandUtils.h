#ifndef COMMANDUTILS_H
#define COMMANDUTILS_H

//System includes
#include <iostream>
#include <string>
#include <map>

template <class N> //Template with a structure N (N would be passed around)
struct CommandPack
{
  typedef void (*Command) (std::string, N&); //Typedef a fuction accepting std::string and arbitary struct N
  CommandPack()=delete; //deletes default constructor
  CommandPack(Command p_command) {command = reinterpret_cast<void*>(p_command);} //Creates a CommandPack based on the passed function
  /**
   * The operator().
   */
  void operator()(std::string str_args, N &n)
  {
    reinterpret_cast<Command>(command)(str_args, n); //launches the command after the cast
  }
  CommandPack<N>& operator=(const CommandPack<N>&)=default;
  void *command = nullptr; //creates a command pointer pointing to nothing
};

namespace CommandUtilities
{
  /**
   * Attempts to aquire the arguments after the flag
   * Priority: Double quotes, Single quotes, and then empty
   * @param  pos_of_flag  The position of where the last character representing the flag is
   * @param  fullArgument The string of all of the arguments passed
   * @return              The arguments for said flag.
   * @throw               A runtime error if the flag manages to exceed expected values
   */
  const std::string gainArguments(const unsigned int pos_of_flag, const std::string& fullArgument);

  /**
   * Attempts to aquire the arguments after the flag
   * NOTE: Deprecated, do not use.
   * Priority: Double quotes, Single quotes, and then empty
   * @param  pos_of_flag  The position of where the last character representing the flag is
   * @param  fullArgument The string of all of the arguments passed
   * @return              The arguments for said flag.
   * @throw               A runtime error if the flag manages to exceed expected values
   */
  const std::string DEPRECATEDgainArguments(const unsigned int pos_of_flag, const std::string& fullArgument);

  /**
   * Processes the string and calls the respective commands
   * @param  arguments      The entire argv
   * @param  argc           The number of elements
   * @param  commandsStore  The store of commands. The key, std::string, is the visual representation of the flag, the value, CommandPack<N>, is the CommandPack itself
   * @param  pointerToN     The pointer to N that is shared among all of the commands (common flags)
   * @return                True or false depending if all or none of the commands worked
   */
  template <class N>
  const bool processArgv(char** argv, const int argc, std::map<std::string, CommandPack<N>>& commandStore, N *pointerToN)
  {
    /* Variable declaration block*/
    std::string arguments{}; //creates a std::string out of the arguments
    unsigned int n_pos = 0; //the position (used in flag aquiring block)

    /* Variable assignment block */
    for (auto iii = 1; iii < argc; iii++) //transfers all of argv into the string (iii starts from 1 to skip the current directory thing)
    {
      arguments += argv[iii]; //adds the argument into the string

      if (iii + 1 != argc) //if iii + 1 is not max size
        arguments += " "; //adds an empty space
    }

    /* Flag execution block */
    for (auto&& it : commandStore) //loops through all iterations of commandStore
    {
      while (it.first != "" && (n_pos = arguments.find(it.first, n_pos)) != std::string::npos) //if a flag is found within the string
      {
        auto flagArguments = gainArguments(n_pos + it.first.size() - 1, arguments); //aquires flag arguments
        if (flagArguments != "") //if the argument is not "" (error/not)
          it.second(flagArguments, *pointerToN); //executes the command with the flag arguments and the pointer

        ++n_pos; //adds to n_pos, so that std::string::find can find the next flag.
      }
      n_pos = 0; //resets n_pos
    }

    /* Post-execution execution block */
    try
    {
      commandStore.at("")("", *pointerToN); //atempts to execute the command at ""
    }
    catch (std::out_of_range&) //catches all out of range
    {} //do absolutely nothing (its ok if the end-user does not define this)

    return true; //Function successfully completed
  }
};

#endif
