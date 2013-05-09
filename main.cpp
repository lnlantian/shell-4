#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include <unistd.h> 
#include <sys/wait.h>

#define TEST

/*
 * features:
 *  machine name
 *  relative and absolute path
 *  STDOUT and STDIN rediretion
 *  placing commands in background
 *  history of commands
 *  support for jobs, cd, history, exit, 
 *  ...kill(pid, SIGKILL)
 *  assume that items are separeted by space
 */



void clear_line(std::string& line)
{
  while (line[0] == ' ') {
    line.erase(0, 1);
  }
  while (line[line.size() - 1] == ' ') {
    line.erase(line.size() - 1, 1);
  }
}
void test_clear_line();

void strip_line(std::string  line,
                std::string& command,
                char** parameters,
                int& param_number)
{
  int split_index = line.find(' ');
  param_number = 0;
  command = bin_directories[0];
  if (split_index == int(std::string::npos)) {
    command += line;
  } else {
    command += line.substr(0, split_index);  
    int i = 0;
    // number of elements in array parameters
    int i_max = sizeof (parameters) / sizeof (*parameters);
    line = line.substr(split_index + 1);
    while (!line.empty() && i < i_max) {
      split_index = line.find(' ');
      // setting parameters
      strcpy(parameters[i], line.substr(0, split_index).c_str());
      ++i;
    }
    param_number = i;
  }
}
void test_strip_line();

std::string get_username()
{
  char* cusername = getlogin();
  if (cusername != NULL) {
    return std::string(cusername); 
  } else {
    return std::string("none");
  }
}

std::string get_cwd() // current working directory
{
  char* cwd = get_current_dir_name();
  if (cwd != NULL) {
    return std::string(cwd); 
  } else {
    return std::string("none");
  }
}

/*
 * main and tests
 */
void test();

std::string working_directory = get_cwd();

int main(int argc, char *argv[])
{
#ifdef TEST
  test();
#endif
  while (true) {
    std::string prompt = get_username() + '@' + get_cwd() + " $ ";
    std::cout << prompt;
    std::string line;
    getline(std::cin, line); // read command and parameters
    clear_line(line);
    if (line == "exit")
      break;
    pid_t pid = fork();          // pid of child process from fork
    if (pid != 0) { /* code of parent process */
      int status = 0;
      waitpid(pid, &status, 0);
      std::cout << "end of child process (" << pid 
        << ") with status " << status << '\n';
    } else { /* code of child process */
      std::string command;
      const int kparam_size = command.size(); // size of parameters array
      int param_number = 0; // real parameters number
      char** parameters = new char*[kparam_size];
      for (int i = 0; i < kparam_size; ++i) {
        parameters[i] = new char[kparam_size];
      }
      strip_line(line, command, parameters, param_number);
      /* parameters list should be ended by NULL */
      parameters[param_number + 1] = NULL;
      /* command execution */
      execv(command.c_str(), parameters);
      for (int i = 0; i < kparam_size; ++i) {
        delete[] parameters[i];
      } 
      delete[] parameters;
    }
  }
  return 0;
}

void test_clear_line()
{
  std::string a = "clear";
  clear_line(a);
  assert(a == "clear");
  a = "ls -a 2";
  clear_line(a);
  assert(a == "ls -a 2");
  a = " cd -Rs";
  clear_line(a);
  assert(a == "cd -Rs");
  a = " cd     ";
  clear_line(a);
  assert(a == "cd");
  a = "clear";
  clear_line(a);
  a = "     ";
  clear_line(a);
  assert(a == "");
}

void test_strip_line()
{
  std::string command;
  const int kparam_size = 50;
  char** parameters = new char*[kparam_size];
  int param_number = 0;
  for (int i = 0; i < kparam_size; ++i) {
    parameters[i] = new char[kparam_size];
  }
  strip_line("ls -a -t -z", command, parameters, param_number);
  assert(command == "/bin/ls");
  strip_line("du -sh", command, parameters, param_number);
  assert(command == "/bin/du");
  for (int i = 0; i < kparam_size; ++i) {
    delete[] parameters[i];
  }
  delete[] parameters;
}

void test()
{
  test_clear_line();
  test_strip_line();
}
