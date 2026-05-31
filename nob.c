#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char** argv) {
  GO_REBUILD_URSELF(argc, argv);

  Cmd cmd = {0};
  cmd_append(&cmd, "g++");
  cmd_append(&cmd, "src/main.cpp");
  cmd_append(&cmd, "src/log.cpp");
  cmd_append(&cmd, "-o", "main");
  cmd_append(&cmd, "-I./include/");

  if (!cmd_run(&cmd)) {
    return 1;
  }
  return 0;
}