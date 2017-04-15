#include <iostream>

//Self-includes
#include "Commands.h"
#include "commandUtils.h"
#include "fileUtils.h"

/**
 * The main method of the program.
 * @param  argc Argument count
 * @param  argv Arguments
 * @return      An integer, 0 or otherwise representing successful execution or otherwise
 */
int main(int argc, char** argv)
{
  /* Variable declaration */
  typedef std::map<std::string, CommandPack<CommandsShare>> StoreOfCommands;
  CommandsShare cs; //"N"
  StoreOfCommands commandStore = {};

  /* Emplace functions into map */
  commandStore.emplace(std::make_pair("-t", CommandPack<CommandsShare>{Commands::test}));

  /* Processes the argv array */
  CommandUtilities::processArgv(argv, argc, commandStore, &cs);

  /* Returns */
  return 0;
}
