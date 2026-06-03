# LogLoc
- A simple header only error logging library for compiler development in c++.


## Features
- File/Line/Colum infomation
- Message type (Error/Warning/Etc)
- Error location (line and colum)
- Error line printing
- Error line context printing

## Example

```cpp
#define LOG_IMPLEMENTATION
// #define LOG_NO_NAMESPACE // Disable the namespace usage if you want. 
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
```

The example above has the following output:
```sh
src/main.cpp:3:1: Error: Hello, I am a message
2 | #include <log.h>
3 | 
    ^^
4 | #include <fstream>
5 | #include <iostream>
6 | #include <string>
7 | #include <vector>
8 | 
```

## Contributing
All contributions are welcome, but mostly will be rejected as I am trying to keep 
this library extremely small. 

