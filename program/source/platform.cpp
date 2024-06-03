#include <direct.h>
#include <iostream>
#include <windows.h>

#include "platform.hpp"

namespace tmp::platform
{
  void screen_setup();
  void screen_shutdown();
  void cursor_visible(bool is_visible);
  void input_setup();
  std::string get_working_directory();

  std::string working_directory;

  void init()
  {
    screen_setup();
    cursor_visible(false);
    input_setup();

    working_directory = get_working_directory();
  }

  void cleanup()
  {
    screen_shutdown();
    cursor_visible(true);
  }

  void screen_setup()
  {
    system("cls");
    system("color 09");
  }

  void screen_shutdown()
  {
    system("cls");
    system("color 07");
  }

  void cursor_visible(bool is_visible)
  {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(out, &cursor_info);
    cursor_info.bVisible = is_visible;
    SetConsoleCursorInfo(out, &cursor_info);
  }

  void input_setup()
  {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode &= (DWORD)~ENABLE_LINE_INPUT;
    SetConsoleMode(hStdin, mode);
  }

  std::string get_working_directory()
  {
    char cwd[FILENAME_MAX];
    if (_getcwd(cwd, sizeof(cwd)) == NULL)
    {
      std::cout << "Error getting current working directory!" << std::endl;
      exit(1);
    }

    return std::string(cwd);
  }
}
