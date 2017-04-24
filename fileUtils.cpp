#include "fileUtils.h"

//System includes
#include <fstream>

const bool FileUtilities::getFileData(unsigned char*& array, unsigned int& size, const std::string fileLocation)
{
  /* Variable declaration block */
  std::ifstream fileReader{fileLocation, std::ios::in | std::ios::binary}; //opens the file in binary mode

  /* Error checking block */
  if (!fileReader) //If there is an error in trying to read the file
    return false;

  size = getFileSize(fileReader); //gets the file size
  array = new unsigned char[size + 1]; //assigns the array to a block of memory size of the file + 1

 /* File reading block */
  fileReader.seekg(0); //Probably not needed, but seeks back all the way to the start.
  fileReader.read(reinterpret_cast<char*>(array), size); //reads the whole file and dumps it into array.
  if (fileReader.bad())
  {
    fileReader.close(); //properly closes the file
    delete[] array; //properly deletes the array
    return false; //reading failed
  }

  array[size] = '\0'; //ends the entire array with '\0' (a.k.a e.o.f, though it is platform-dependent)
  fileReader.close(); //properly closes the file
  return true; //reading successful!
}

const std::size_t FileUtilities::getFileSize(const std::string& fileLocation)
{
  std::ifstream fileReader{fileLocation, std::ios::in | std::ios::binary}; //opens the file in binary mode
  return getFileSize(fileReader); //uses the other overload of this function
}

const std::size_t FileUtilities::getFileSize(std::ifstream& fileReader)
{
  /* Error handling block */
  if (!fileReader)
    return 0; //Error occured, return no size.

  /* Variable declaration block */
  auto preMutation_position = fileReader.tellg(); //gets the current position
  std::size_t returnSize = 0;

  /* Finding out the size block */
  fileReader.seekg(0, fileReader.end); //sets the position to 0 relative to the end of the file
  returnSize = fileReader.tellg(); //tells the size
  fileReader.seekg(preMutation_position); //sets g back to its original position
  return returnSize; //returns the current return size -1.
}
