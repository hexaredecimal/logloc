#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef LOG_NO_NAMESPACE
namespace logloc {
#endif
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

#ifdef LOG_IMPLEMENTATION

void logloc::Log::set_message(std::string& message) {
  this->m_message = message;
}

void logloc::Log::set_kind(std::string& kind) { this->m_kind = kind; }

void logloc::Log::set_filename(std::string& filename) {
  this->m_filename = filename;
}

void logloc::Log::set_message(const char* message) {
  this->m_message = std::string(message);
}

void logloc::Log::set_kind(const char* kind) {
  this->m_kind = std::string(kind);
}

void logloc::Log::set_filename(const char* filename) {
  this->m_filename = std::string(filename);
}

void logloc::Log::set_input_lines(std::vector<std::string> input_lines) {
  this->m_input_lines = input_lines;
}

void logloc::Log::set_start_info(int line, int col) {
  this->m_start.line = line;
  this->m_start.col = col;
}

void logloc::Log::set_end_info(int line, int col) {
  this->m_end.line = line;
  this->m_end.col = col;
}

static bool is_line_valid(std::vector<std::string>& lines, int line) {
  int index = line - 1;
  return !(index < 0 || index >= lines.size());
}

static std::string get_line(std::vector<std::string>& lines, int line) {
  int index = line - 1;
  if (index < 0) index = 0;
  if (index >= lines.size()) index = lines.size() - 1;

  return lines[index];
}

static std::string get_line_gutter(int line) {
  return std::to_string(line) + std::string(" | ");
}

std::string logloc::Log::get_column_pointer(int line, int column) {
  std::string gutter = get_line_gutter(line);

  std::stringstream ss;
  for (int i = 0; i < gutter.size(); ++i) ss << " ";

  for (int i = 1; i < column; ++i) ss << " ";

  if (m_end.col == 0 || m_end.col <= column)
    ss << "^";
  else {
    int carets = m_end.col - column + 1;
    for (int i = 0; i < carets; ++i) ss << "^";
  }
  return ss.str();
}

static bool has_lines_above_line(std::vector<std::string>& source, int line,
                                 int lines = 1) {
  if (!is_line_valid(source, line)) return false;
  for (int target = line - lines; target < line; target++) {
    if (!is_line_valid(source, target)) return false;
  }
  return true;
}

static bool has_lines_below_line(std::vector<std::string>& source, int line,
                                 int lines = 1) {
  if (!is_line_valid(source, line)) return false;
  if (!is_line_valid(source, line + lines)) return false;
  return true;
}

void logloc::Log::report() {
  int lines_count = m_input_lines.size();
  bool has_lines = lines_count > 0;
  bool has_file_name = m_filename.length() > 0;

  int line = m_start.line;
  int col = m_start.col;

  int far_back = m_reveal_lines_before;
  int far_forward = m_reveal_lines_after;

  if (!has_lines && !has_file_name) {
    std::cerr << line << ":" << col << ": " << m_kind << ": " << m_message
              << std::endl;
    return;
  } else if (!has_lines && has_file_name) {
    std::cerr << m_filename << ":" << line << ":" << col << ": " << m_kind
              << ": " << m_message << std::endl;
    return;
  }

  if (has_lines && !has_file_name) {
    std::cerr << line << ":" << col << ": " << m_kind << ": " << m_message
              << std::endl;
  }

  if (has_lines && has_file_name) {
    std::cerr << m_filename << ":" << line << ":" << col << ": " << m_kind
              << ": " << m_message << std::endl;
  }

  if (!is_line_valid(m_input_lines, line)) return;

  while (line - far_back < line) {
    int prev = line - far_back;
    if (is_line_valid(m_input_lines, prev)) {
      std::cerr << get_line_gutter(prev) << get_line(m_input_lines, prev)
                << std::endl;
    }
    far_back--;
  }
  std::cerr << get_line_gutter(line) << get_line(m_input_lines, line)
            << std::endl;
  std::cerr << get_column_pointer(line, col) << std::endl;

  int count = 1;
  while (far_forward > 0) {
    int next = line + count;
    if (is_line_valid(m_input_lines, next)) {
      std::cerr << get_line_gutter(next) << get_line(m_input_lines, next)
                << std::endl;
    } else
      break;
    count++;
    far_forward--;
  }
}

void logloc::Log::reveal_lines_before(int before) {
  this->m_reveal_lines_before = before;
}

void logloc::Log::reveal_lines_after(int after) {
  this->m_reveal_lines_after = after;
}
#endif

#ifndef LOG_NO_NAMESPACE
}  // namespace logloc
#endif