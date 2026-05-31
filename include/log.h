#pragma once

#include <string>
#include <vector>

namespace logloc {

class Log {
 public:
  Log()
      : m_message{},
        m_kind{},
        m_filename{},
        m_input_lines{},
        m_start{1, 1},
        m_end{} {}

  void set_message(std::string& message);
  void set_kind(std::string& kind);
  void set_filename(std::string& filename);
  void set_input_lines(std::vector<std::string> inputlines);

  void set_message(const char* message);
  void set_kind(const char* kind);
  void set_filename(const char* filename);

  void set_start_info(int, int);
  void set_end_info(int, int);

  void report();

  void reveal_lines_before(int lines);
  void reveal_lines_after(int after);

 private:
  struct Loc {
    int line;
    int col;
  };

  std::string m_message;
  std::string m_kind;
  std::string m_filename;
  std::vector<std::string> m_input_lines;
  Loc m_start;
  Loc m_end;

  int m_reveal_lines_before = 1;
  int m_reveal_lines_after = 1;

  std::string get_column_pointer(int line, int column);
};
}  // namespace logloc
