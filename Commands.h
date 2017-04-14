#ifndef COMMANDS_H
#define COMMANDS_H

//System includes
#include <string>
#include <iostream>

struct CommandsShare //The sturcture containing the variables to share among the Commands class
{
  std::string str_directory{}; //the string storing the directory
};

/*
The namespace Commands. All fuctions within this namespace is CommandUtilities compliant; and can be used on it.
 */
namespace Commands
{
  /**
   * Outputs to the console stream to prove that the command processing is working. All arguments within will be printed to the stream as well
   * Flag: -t, --t, -test, --test
   * @param args The arguments.
   * @param cs   The shared structure that all functions will get access to.
   */
  void test(std::string args, CommandsShare& cs)
  {
    std::cout << "Files2CPP's output stream is working loud and clear!" << std::endl;
    std::cout << "Argument passed to the testing stream: " << args << std::endl;
    return;
  }

  /**
   * Takes the user provided directory and places it into the CommandsShare.
   * Flag: -upd, --upd, -userDir
   * @param args Tbe arguments.
   * @param cs   The shared structure that all functions will get access to.
   */
  void userProvidedDirectory(std::string args, CommandsShare& cs)
  {
    //TODO: Place Boost filesystem library code to check if current directory is valid here
    cs.str_directory = args;
  }

};

#endif //Header guard
