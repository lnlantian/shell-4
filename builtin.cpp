#include "builtin.h"

void show_history(void)
{
  return;
}

int check_builtin(std::string line)
{
  int space_index = 0;
  if ((space_index = line.find(' ')) != int(std::string::npos)) { /* !!! */
    std::string cmd = line.substr(0, space_index);
  }
  if (line == "exit") {
    return 0;
  } else if (line == "cd") {

  } else if (line == "history") {
    show_history();
  } else if (line == "jobs") {

  } else if (line == "help") {
    // builtin_help();
  }
  return -1;
}
