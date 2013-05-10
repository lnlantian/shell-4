#include "builtin.h"

unsigned long long command_number = 1;
std::vector<std::pair<unsigned long long, std::string> > history;

unsigned long long new_command()
{
  return command_number++;
}

void clear_history(void)
{
  int size = history.size();
  if (size <= kSavedCommands) return; /* no need to cut */
  std::vector<std::pair<unsigned long long, std::string> > tmp; /* cut vector */
  for (int i = size - kSavedCommands; i < size; ++i) {
    tmp.push_back(history[i]);
  }
  history = tmp;
}

void show_history(std::ofstream& out)
{  
  int size = history.size();
  for (int i = std::max(size - kSavedCommands, 0); i < size; ++i) {
    out << std::setw(3) << history[i].first << ' ' << history[i].second << '\n';
  }
  return;
}

void load_history(void)
{

}

void save_history(void)
{
  std::ofstream out(history_filename);
  show_history(out); // !!! here 
}

void builtin_help(void)
{

}

int check_builtin(std::string line)
{
  int space_index = 0;
  std::string cmd = line;
  if ((space_index = line.find(' ')) != int(std::string::npos)) { /* !!! */
    cmd = line.substr(0, space_index);
  }
  if (cmd == "exit") {
    return 0;
  } else if (cmd == "cd") {

    return 1;
  } else if (cmd == "history") {
    show_history();
    return 1;
  } else if (cmd == "jobs") {

    return 1;
  } else if (cmd == "help") {
    // builtin_help();
    return 1;
  }
  return -1;
}
