/*
 * author: Lukasz Hryniuk
 * e-mail: lukequaint@gmail.com
 * */

#include "builtin.h"
#include <unistd.h>
#include <iostream>
#include <vector>

/**
 * changing directory part
 */

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
  std::cout << " - jobs\n";
  std::cout << " - help\n";
  std::cout << " - cd\n";
  //std::cout << " - history\n";
  std::cout << " - exit\n";
}

int check_builtin(std::string line)
{
  int space = 0;
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
    //bool to_print = true;
    ///* checking parameters */
    //for (std::vector<std::string>::iterator i = parameters.begin();
         //i != parameters.end(); ++i) {
      //if (*i == "-c") {
        //clear_history();
//#ifdef DEBUG
        //std::cerr << "History cleared!\n";
//#endif
      //} else {
        //std::cerr << "Invalid parameter: " << *i << " .\n";
      //}
    //}
    //if (to_print) {
      //show_history();
    //}
    //return 1;
  } else if (cmd == "jobs") {
    /* background processes */
    return 1;
  } else if (cmd == "builtin") {
    builtin_help();
    return 1;
  } else if (cmd == "help") {
    /* shell usage / builtins! */
    return 1;
  }
  return -1;
}
