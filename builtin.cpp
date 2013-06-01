#include "builtin.h"

/**  
 * history part
 */

/* number in history */
unsigned long long command_number = 1;
std::vector<std::pair<unsigned long long, std::string> > history;

unsigned long long new_command()
{
  return command_number++;
}

void clear_history(void)
{
  command_number = 1;
  history.clear();
}

void cut_history(void)
{
  int size = history.size();
  if (size <= kSavedCommands) return; /* no need to cut */
  std::vector<std::pair<unsigned long long, std::string> > tmp; /* cut vector */
  for (int i = size - kSavedCommands; i < size; ++i) {
    tmp.push_back(history[i]);
  }
  history = tmp;
}

void show_history(std::ostream& out)
{  
  int size = history.size();
  for (int i = std::max(size - kSavedCommands, 0); i < size; ++i) {
    out << std::setw(3) << history[i].first << ' ' << history[i].second << '\n';
  }
}

void load_history(void)
{
  std::ifstream in(kHistoryFilename.c_str());
  unsigned long long number = 0;
  std::string command;
  std::string tmp;
  std::string numstr;
  while (getline(in, tmp)) {
    unsigned i = 0;
    /* get number string */
    while ((i < tmp.size()) && (tmp[i] == ' ' || isdigit(tmp[i]))) {
      ++i;
    }
    /* cut number of command */
    numstr = tmp.substr(0, i);
    command = tmp.substr(i);
    std::istringstream instr(numstr); /* convert string to number */
    instr >> number;
    command_number = std::max(command_number, number);
    history.push_back(make_pair(number, command));
  }
  command_number++; /* it will be number of next command */
}

void save_history(void)
{
  std::ofstream out(kHistoryFilename.c_str());
  show_history(out);
}

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
  std::cout << " - history\n";
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
  
  } else if (cmd == "history") {
    bool to_print = true;
    /* checking parameters */
    for (std::vector<std::string>::iterator i = parameters.begin();
         i != parameters.end(); ++i) {
      if (*i == "-c") {
        clear_history();
#ifdef DEBUG
        std::cerr << "History cleared!\n";
#endif
      } else {
        std::cerr << "Invalid parameter: " << *i << " .\n";
      }
    }
    if (to_print) {
      show_history(std::cout);
    }
    return 1;
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
