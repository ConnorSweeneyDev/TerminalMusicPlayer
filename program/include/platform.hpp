#pragma once

#include <string>

namespace tmp::platform
{
  void cursor_visible(bool is_visible);

  void init();
  void cleanup();

  extern std::string working_directory;
}
