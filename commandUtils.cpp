#include "commandUtils.h"

//System includes
#include <exception>

template <class N> //Template with a structure N (N would be passed around)
std::map<std::string, CommandPack<N>> CommandUtilities::commandStore = {};

template <class N> //Template with a structure N (N would be passed around)
const bool CommandUtilities::processArgv(const char** argv, const int argc, N *pointerToN)
{
  /* Variable declaration block*/
  std::string arguments{}; //creates a std::string out of the arguments
  unsigned int n_pos = 0; //the position (used in flag aquiring block)

  /* Variable assignment block */
  for (auto iii = 0; iii < argc; iii++) //transfers all of argv into the string
    arguments += argv[iii];

  /* Flag execution block */
  for (auto&& it : commandStore<N>) //loops through all iterations of commandStore
  {
    if ((n_pos = arguments.find(it.first)) != std::string::npos) //if a flag is found within the string
    {
      auto flagArguments = gainArguments(n_pos + it.first.size(), arguments); //aquires flag arguments
      if (flagArguments != "") //if the argument is not "" (error/not)
      {
        it.second.command(flagArguments, *pointerToN); //executes the command with the flag arguments and the pointer
      }
    }
  }

  /* Post-execution execution block */
  try
  {
    commandStore<N>.at("").command("", *pointerToN); //atempts to execute the command at ""
  }
  catch (std::out_of_range&) //catches all out of range
  {} //do absolutely nothing (its ok if the end-user does not define this)

  return true; //Fuction sueccessfully completed

}

const std::string CommandUtilities::gainArguments(const unsigned int pos_of_flag, const std::string& fullArgument)
{
  //Error checking
  {
    for (unsigned int iii = pos_of_flag; iii < fullArgument.size(); iii++)
    {
      if (fullArgument[iii] != ' ')
        break; //error check complete

      if (iii + 1 == fullArgument.size())
        return ""; //nothing after the flag!
    }
  }

  /* Variable declaration */
  unsigned int currentPos = pos_of_flag; //sets the current position to the position of the flag
  enum whatExists : uint8_t
  {
    NOTHING = 0x00, //this is only possible if it is actually only 0
    DOUBLEQUOTES = 0x01, //toggle if double quotes exists
    SINGLEQUOTES = 0x02, //toggle if single quote exists
    FLAG = 0x04 //toggle if flag exists
  };

  uint8_t what_available = NOTHING; //for now, nothing exists

  /* Next double quote detection */
  if (~what_available & DOUBLEQUOTES && (currentPos = fullArgument.find("\"", pos_of_flag + 1)) != std::string::npos) //if a quote " is detected
  {
    if (fullArgument.find("\"", currentPos + 1) != std::string::npos) //and yet another after the first
      what_available |= DOUBLEQUOTES;
  }

  /* Next single quote detection */
  if (~what_available & SINGLEQUOTES && (currentPos = fullArgument.find("\'", pos_of_flag + 1)) != std::string::npos) //if a quote ' is detected
  {
    if (fullArgument.find("\'", currentPos + 1) != std::string::npos) //and yet another after the first
      what_available |= SINGLEQUOTES;
  }

  /* Next flag detection */
  if (~what_available & DOUBLEQUOTES && ~what_available & SINGLEQUOTES && ~what_available & SINGLEQUOTES) //if quotes don't already exist
  {
    if (fullArgument.find("--", pos_of_flag + 1) != std::string::npos || fullArgument.find("-", pos_of_flag + 1) != std::string::npos) //only one of these will result to anything other than std::string::npos, so we're safe
      what_available |= FLAG; //flag exists
  }

  /* Based on which flags are enabled, skip the function */
  if (what_available & FLAG)
    return ""; //No arguments here

  //Nothing (a.k.a quoteless)
  if (what_available & NOTHING)
  {
    currentPos = fullArgument.find_first_not_of(' ', pos_of_flag + 1); //finds the first character that is not a quote (this needs to be done anyway because currentPos will be set to std::string::npos all over the place)
    unsigned int n_count = fullArgument.find(' ', currentPos + 1) - currentPos; //the length from currentPos to the index before ' '
    return {fullArgument.substr(currentPos, n_count)}; //returns the arguments
  }

  //Double Quotes
  if (what_available & DOUBLEQUOTES)
  {
    //currentPos is already at double quotes
    unsigned int n_count = fullArgument.find('\"', currentPos + 1) - currentPos; //the length from currentPos to the index before "\""
    return {fullArgument.substr(currentPos, n_count)}; //returns the arguments
  }

  //Single Quotes
  if (what_available & SINGLEQUOTES)
  {
    //currentPos is already at single quotes
    unsigned int n_count = fullArgument.find("\'", currentPos + 1) - currentPos; //the length from currentPos to the index before "\'"
    return {fullArgument.substr(currentPos, n_count)}; //returns the arguments
  }

  //If control reaches here, something went wrong.
  throw std::runtime_error("const std::string CommandUtilities::gainArguments(const uint, const string&) managed a flag that is neither nothing or anything.");

  return ""; //and if it reaches here, just return
}
