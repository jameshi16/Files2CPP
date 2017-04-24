#include "commandUtils.h"

//System includes
#include <exception>
#include <memory>

const std::string CommandUtilities::gainArguments(const unsigned int pos_of_flag, const std::string& fullArgument)
{
  /* fullArgument check */
  //Ensure that accessing pos_of_flag + 1 is < size().
  if (pos_of_flag + 1 >= fullArgument.size())
    return ""; //There is no arguments to gain here.

  /* ENUM to represent what exists within the fullArgument string */
  enum whatExists
  {
    START_DOUBLEQUOTES,
    END_DOUBLEQUOTES,
    START_SINGLEQUOTES,
    END_SINGLEQUOTES,
    PURE, //pure means no quotes, no flags, but just plain arguments.
    FLAG,
    NOTHING //The "null" of this enum (in an array of whatExists, use this as the max size in the array.)
  };

  /* Variable declaration */
  unsigned int pos_var1               = 0; //The position variable used within expressions
  uint8_t pos_closest                 = 7; //The position of <insert whatever here> closest to the pos_of_flag
  std::unique_ptr<unsigned int[]> pos(new unsigned int[NOTHING]); //creates an unsigned int array with NOTHING as the size. This array represents the positions of the whatExists enum in fullArgument

  for (unsigned int iii = 0; iii < NOTHING; iii++) //Fills with std::string::npos
    pos[iii] = std::string::npos;

  /* Finding the next positions double quotes, single quotes, flag, or nothing. */
  pos[START_SINGLEQUOTES] = fullArgument.find("\'", pos_of_flag);
  pos[END_SINGLEQUOTES]   = fullArgument.find("\'", pos[START_SINGLEQUOTES] + 1);
  pos[START_DOUBLEQUOTES] = fullArgument.find("\"", pos_of_flag);
  pos[END_SINGLEQUOTES]   = fullArgument.find("\"", pos[END_SINGLEQUOTES] + 1);
  pos[FLAG]               = ((pos_var1 = fullArgument.find(" -", pos_of_flag)) != std::string::npos) ? (++pos_var1) : (pos_var1); //The space before '-' is intentional
  pos[PURE]               = ((pos_var1 = fullArgument.find(" ", pos_of_flag)) != std::string::npos) ? (++pos_var1) : (pos_var1);

  pos_var1 = 0; //resets temporary positions

  /* Finds which one (only comparing START_ if available) comes first */
  for (auto iii = 0; iii < NOTHING; iii++)
  {
    if (pos_closest == 7 || (pos[iii] < pos[pos_closest] && pos[iii] != std::string::npos))
      pos_closest = iii; //sets the closest position to iii
  }

  /* Processing block */
  //Single quotes
  if (pos_closest == START_SINGLEQUOTES && pos[START_SINGLEQUOTES] != std::string::npos)
    return fullArgument.substr(pos[pos_closest] + 1, pos[END_SINGLEQUOTES] - pos[START_SINGLEQUOTES] + 1);

  //Double quotes
  if (pos_closest == START_DOUBLEQUOTES && pos[START_DOUBLEQUOTES] != std::string::npos)
    return fullArgument.substr(pos[pos_closest] + 1, pos[END_DOUBLEQUOTES] - pos[START_DOUBLEQUOTES] + 1);

  //Pure
  if (pos_closest == PURE && pos[FLAG] > pos[PURE])
    return fullArgument.substr(pos[pos_closest], pos[FLAG] - pos[PURE] - 1);

  /* Fall through */
  return ""; //no arguments found
}

const std::string CommandUtilities::DEPRECATEDgainArguments(const unsigned int pos_of_flag, const std::string& fullArgument)
{
  //Error checking
  {
    if (fullArgument.find_first_not_of(" ", pos_of_flag + 1) == std::string::npos)
      return "";
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
  if (what_available == NOTHING) //NOTHING is, in binary, 0000. Hence, it is required to use == (arithmetic) instead of & (bitwise).
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
