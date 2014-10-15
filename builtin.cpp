#include "builtin.h"

extern "C" {
#include <unistd.h>
}

#include <iostream>
#include <vector>

/**
 * changing directory part
 */

extern const char* history_file;

int change_directory(std::string path)
{
  return chdir(path.c_str());
}

/**
 * checking builtins and help
 */
void builtin_help(void)
{
  std::cout << "built-ins list:\n\n";
  //std::cout << " - jobs\n";
  std::cout << " - cd\n";
  std::cout << " - help\n";
  //std::cout << " - history\n";
  std::cout << " - exit\n";
}

void usage(void)
{
  std::cout << "Simple shell made by lukequaint\n\n";
  std::cout << "Based on bash, allows to:\n";
  std::cout << " - execute commands (with parameters)\n";
  std::cout << " - use pipes\n";
  std::cout << " - walking through directories\n";
  std::cout << " - searching in history using arrows\n\n";
  builtin_help();
}


int check_builtin(std::string line)
{
  size_t space = 0;
  std::string cmd = line;
  std::vector<std::string> parameters; /* built-in params */
  /* extract parameter and erase it from line */
  if ((space = line.find(' ')) != std::string::npos) { 
    cmd = line.substr(0, space);
    line.erase(0, space + 1);
    while ((space = line.find(' ')) != std::string::npos) {
      std::string param = line.substr(0, space);
      parameters.push_back(param);
      line.erase(0, space + 1);
    }
    parameters.push_back(line);
  }
  if (cmd == "exit") { /* here history saving? */
    return 0;
  } else if (cmd == "cd") {
  
    if (parameters.size() != 1) {
      std::cerr << "Wrong parameters. Should be one containing path.\n";
    } else {
      if (change_directory(parameters[0])) {
        std::cerr << "Wrong directory name.\n";
      }
    }
    return 1;
  
  //} else if (cmd == "history") {
    //std::ifstream hist_in(history_file); 
    //if (!hist_in) /* error while opening file, but it's not very bad */
      //return 1;
    //while (hist_in && !hist_in.eof()) {
        //std::string command;
        //getline(hist_in, command);
        //std::cout << command << '\n';
    //}
    //return 1;
  //} else if (cmd == "jobs") {
    ///* background processes */
    //return 1;
  } else if (cmd == "builtin") {
    builtin_help();
    return 1;
  } else if (cmd == "help") {
    usage();
    return 1;
  }
  return -1;
}
