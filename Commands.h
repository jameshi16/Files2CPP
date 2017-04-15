#ifndef COMMANDS_H
#define COMMANDS_H

//System includes
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <limits>

//Self includes
#include "fileUtils.h"

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
   * Gets the data of the file named testing.png. Then, it writes out to a file named
   * testing2.png. All arguments are also passed to the output stream.
   * Flag: -t, -test. (-- variants are also supported)
   * @param args The arguments.
   * @param cs   The shared structure that all functions will get access to.
   */
  void test(std::string args, CommandsShare& cs)
  {
    std::cout << "Argument passed to the testing stream: " << args << std::endl;
    std::cout << "Attempting to load file: test.png" << std::endl;
    //TODO: Insert Boost code to check if file exists

    std::cout << "Entering variable declaration block" << std::endl;

    /* Get maximum size of size_t */
    std::cout << "Aquiring the maximum size of std::size_t" << std::endl;
    auto max_size = std::numeric_limits<std::size_t>::max(); //A thing to contain the maximum size of anything
    std::cout << "Max size of std::size_t acquired: " << max_size << std::endl;

    /* Get amount of bytes in a char */
    std::cout << "Aquiring how many bytes an unsigned char contains" << std::endl;
    auto char_max = std::numeric_limits<unsigned char>::digits / 8;
    std::cout << "An unsigned char can contain " << char_max << " byte(s) of data" << std::endl;

    /* Variables */
    unsigned char *fileData = new unsigned char[50000000]; //creates an array with the size of std::size_t.
    std::cout << "Variable declaration block passed. Loading files up to " << 50000000 << " bytes are possible." << std::endl;
    delete[] fileData; //We don't really need this anymore

    /* Getting the file */
    std::cout << "Acquiring file data in test.png..." << std::endl;
    if (!FileUtilities::getFileData(fileData, max_size, "./test.png")) //opens the test.png file (max_size is modified here)
    {
      std::cout << "An error occured while trying to load test.png. (Does it exist?)" << std::endl;
      return;
    }
    std::cout << "File acquisition successful." << std::endl;

    /* Writing to another file */
    std::cout << "Attempting to create/overwrite: test2.png" << std::endl;
    std::ofstream out_file{"./test2.png", std::ios::trunc | std::ios::out | std::ios::binary};
    if (!out_file) //opens the test2 file
    {
      std::cout << "Creation/Overwrite of test2.png failed." << std::endl;
      out_file.close(); //closes the file
      delete[] fileData; //properly delete fileData
      return;
    }
    //Write
    std::cout << "Attempting to write into test2.png" << std::endl;
    out_file.write(reinterpret_cast<char*>(fileData), max_size); //writes unformatted input into the file
    std::cout << "Writing technically succeeded." << std::endl;
    out_file.close(); //closes the file

    delete[] fileData; //if everything is successful so far, delete the array.
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
