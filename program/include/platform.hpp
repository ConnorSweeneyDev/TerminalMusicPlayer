#pragma once

#include <string>

namespace tmp::platform
{
  void init();
  void cleanup();

  extern std::string working_directory;
}
