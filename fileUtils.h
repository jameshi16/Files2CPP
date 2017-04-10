#ifndef FILEUTILS_H
#define FILEUTILS_H

//System includes
#include <iostream>

namespace FileUtilities
{
  /**
   * Gets the file as hex, and then creates a new array which the array pointer points to.
   * This function will do all it can to read absolutely everything. If the file size is more than std::size_t, **THIS FUNCTION WILL FAIL**.
   * If there is not enough memory to store ALL of the data, **THIS FUNCTION WILL FAIL**.
   * For a function that reads ONLY 100MB per call, please use getLargeFileData() instead.
   * @param  array        The pointer that is expected to be assigned to an array. It is recommended that this points to nullptr.
   * @param  size         The integer that will contain the size of the result array.
   * @param  fileLocation The file to read off from into the array.
   * @return              True or false, depending on whether the operation succeeded.
   */
  const bool getFileData(unsigned char*& array, unsigned int& size, const std::string fileLocation);
  /**
   * Does its best to find the size of the file. (Does not use Boost.)
   * @param  fileLocation The location of the file
   * @return              The size of the file.
   */
  const std::size_t getFileSize(const std::string& fileLocation);
  /**
   * Does its best to find the size of the file. (Does not use Boost.)
   * This function will restore the correct position of g.
   * @param  fileReader The ifstream object that is opened to the file.
   * @return            The size of the file.
   */
  const std::size_t getFileSize(std::ifstream& fileReader);
};

#endif
