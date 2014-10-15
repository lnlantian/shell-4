#include "Cmd.hpp"

Cmd::Cmd()
{

}

Cmd::~Cmd()
{

}

std::string Cmd::clear_line(std::string line)
{
  while ((line[0] == ' ') || (line[0] == '\n')) {
    line.erase(0, 1);
  }
  while ((line[line.size() - 1] == ' ') || (line[line.size() - 1] == '\n')) {
    line.erase(line.size() - 1, 1);
  }
  return line;
}
