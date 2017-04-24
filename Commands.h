#ifndef COMMANDS_H
#define COMMANDS_H

//System includes
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <limits>

//Boost includes
#include <boost/filesystem.hpp>

//Self includes
#include "fileUtils.h"

struct CommandsShare //The sturcture containing the variables to share among the Commands class
{
  std::string str_directory{}; //the string storing the directory
  std::string str_saveAsDirectory{}; //the directory to save as
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
   * Flag: -upd, -userDir
   * @param args Tbe arguments.
   * @param cs   The shared structure that all functions will get access to.
   */
  void userProvidedDirectory(std::string args, CommandsShare& cs)
  {
    if (!boost::filesystem::exists(boost::filesystem::path(args)) && !boost::filesystem::is_directory(boost::filesystem::path(args)))
      return; //returns, directory invalid

    cs.str_directory = args;
  }

  /**
   * Saves the input as the parent directory for within the .cpp code.
   * Flag: -sad, -saveAsDir
   * @param args The arguments.
   * @param cs   The shared structure that all functions will get access to.
   */
  void saveAsDirectory(std::string args, CommandsShare& cs)
  {
    if (!boost::filesystem::exists(boost::filesystem::path(args)) && !boost::filesystem::is_directory(boost::filesystem::path(args)))
      return; //returns, directory invalid

    cs.str_saveAsDirectory = args;
  }

  void finalProcess(std::string args, CommandsShare& cs)
  {
    /* Print the directories to the user */
    std::cout << "Directory to scan files from: " << cs.str_directory << std::endl;
    std::cout << "Directory to save files as: " << cs.str_saveAsDirectory << std::endl;

    /* CommandsShare check */
    if (cs.str_directory == "") //if directory selected is empty
      cs.str_directory = boost::filesystem::current_path().string(); //gets the current path

    if (cs.str_saveAsDirectory == "") //if directory to save is empty
      cs.str_saveAsDirectory = "."; //dot path is best path

    /* Convert all of the strings to boost's format */
    cs.str_directory        = boost::filesystem::path(cs.str_directory).generic_string(); //makes str_directory a well-formed directory string
    cs.str_saveAsDirectory  = boost::filesystem::path(cs.str_saveAsDirectory).generic_string(); //makes str_saveAsDirectory well-formed directory string

    /* Modify str_directory to be a relative path according to str_saveAsDirectory. */
    {
      std::string currentDirectory = boost::filesystem::current_path().generic_string();
      unsigned int n_pos = 0;

      while ((n_pos = cs.str_directory.find(currentDirectory)) != std::string::npos)
        cs.str_directory.replace(n_pos, currentDirectory.size(), cs.str_saveAsDirectory); //replaces the current directory with ./
    }

    /*** Main sequence ***/
    /** Open/Create data.cpp/.h files **/
    std::ofstream cppFile{"data.cpp", std::ios::trunc}; //opens (creates) a cpp file called data.cpp (TODO: Implement way to change this)
    std::ofstream hFile  {"data.h", std::ios::trunc}; //opens (creates) a h file called data.h (TODO: Implement way to change this)
    /** Check if opening was successful **/
    if (!cppFile || !hFile)
      return; //opening the files failed, will not proceed.

    /** Standard code for CPP file **/
    cppFile << "#include \"data.h\"" << std::endl << "#include <memory>" << std::endl << std::endl; //2 end lines are intentional. //Headers
    cppFile << "/**" << std::endl << "* The function to call in order to get data stored through the tool. Only supports ISO directory seperators, i.e. \"/\" not \"\\\"" << std::endl //Body
    << "* @param path   The path of the file (if you didn't specify, the parent directory is ./)" << std::endl
    << "* @param array  The char* pointer to contain a newly formed array. It should not be pointing to any array. (nullptr)" << std::endl
    << "* @param size   The int to contain the size of the array." << std::endl
    << "* @return       True if the path was found within the function and the array and size was overwritten, false if nothing was found within the function." << std::endl
    << "**/"
    << "bool acquireData(std::string path, unsigned char*& array, std::size_t& size)" << std::endl
    << "{" << std::endl;

    /** Insertion of all of the files in the directory **/
    for (auto&& it : boost::filesystem::recursive_directory_iterator{boost::filesystem::path(cs.str_directory)}) //loops through the entire filesystem (recursive)
    {
      /* Ignore files2cpp.exe, data.h, and data.cpp. (Data.h and Data.cpp customizable soon) */
      if (it.path().generic_string().find("files2cpp.exe") != std::string::npos)
        continue; //skips this loop

      if (it.path().generic_string().find("data.cpp") != std::string::npos)
        continue; //skips this loop

      if (it.path().generic_string().find("data.h") != std::string::npos)
        continue; //skips this loop

      std::cout << "Writing file: " << it.path().generic_string() << std::endl;

      cppFile << "  if (std::string(\"" << it.path().generic_string() << "\").find(path) != std::string::npos)" << std::endl
      << "    {" << std::endl
      << "      size = " << static_cast<unsigned int>(FileUtilities::getFileSize(it.path().generic_string())) << ";" << std::endl
      << "      array = new unsigned char[size] {";

      /* The variables */
      size_t theSize = 0; //The size
      unsigned char* theArray = nullptr; //An array pointing to nothing

      /* Check to ensure that function returns true */
      if (FileUtilities::getFileData(theArray, theSize, it.path().generic_string()))
      {
        for (unsigned int iii = 0; iii < theSize; iii++) //runs a loop to write every single hexadecimal into the file
        {
          /* Data writing block */
          cppFile << "0x" << std::hex << static_cast<unsigned int>(theArray[iii]); //Writes the number

          if (iii + 1 != theSize)
          {
            cppFile << ","; //writes a comma
          }
        }
        delete[] theArray; //deletes the array
      }

      /* Closes the curly braces */
      cppFile << "};" << std::endl; //make a single empty line after the braces
      cppFile << "      return true;" << std::endl; //allows the function to return straight away
      cppFile << "    }" << std::endl; //closes the if braces
    }

    cppFile << "  return false;" << std::endl; //returns false
    cppFile << "}" << std::endl; //closes the function

    /** Standard code for the header file **/
    hFile << "#ifndef DATA_H" << std::endl
    << "#define DATA_H" << std::endl << std::endl
    << "#include <iostream>" << std::endl
    << "#include <string>" << std::endl
    << "bool aquireData(std::string path, unsigned char*& array, std::size_t& size);" << std::endl << std::endl
    << "#endif" << std::endl;

    /** Completed Generation. Close all file streams **/
    cppFile.close();
    hFile.close();
    return;
  }

};

#endif //Header guard
