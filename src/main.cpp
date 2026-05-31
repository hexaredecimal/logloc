#include <log.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> get_lines(const std::string& path) {
  std::ifstream file(path);
  std::vector<std::string> lines{};
  if (!file) {
    int line = __builtin_LINE();
    int col = 1;
    logloc::Log msg;
    msg.set_start_info(line, col);
    msg.set_kind("[Error]");
    msg.set_message("Failed to read file");
    msg.report();
    return lines;
  }

  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}

int main() {
  auto lines = get_lines("src/main.cpp");

  logloc::Log msg;
  msg.set_start_info(3, 1);
  msg.set_end_info(3, 2);
  msg.set_message("Hello, I am a message");
  msg.set_kind("Error");
  msg.set_filename("src/main.cpp");
  msg.set_input_lines(lines);
  msg.reveal_lines_after(5);
  msg.report();
}