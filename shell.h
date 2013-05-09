#ifndef SHELL_H
#define SHELL_H

const std::string bin_directories[] = { "/bin/", // for exec() functions
                                        "/usr/bin/",
                                        "/usr/local/bin/" };

class Shell
{
 public:
  Shell();
  ~Shell();
  std::string Cwd();
  std::string Username();
  bool Builtin(Command cmd);
 private:
  std::string cwd_;  
  std::string username_;
};

#endif
