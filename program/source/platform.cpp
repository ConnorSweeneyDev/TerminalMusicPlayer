#include <direct.h>
#include <iostream>
#include <windows.h>

#include "platform.hpp"

namespace tmp::platform
{
  std::string working_directory;

  void screen_setup();
  void input_setup();
  std::string get_working_directory();

  void cursor_visible(bool is_visible)
  {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(out, &cursor_info);
    cursor_info.bVisible = is_visible;
    SetConsoleCursorInfo(out, &cursor_info);
  }

  void init()
  {
    screen_setup();
    input_setup();

    working_directory = get_working_directory();
  }

  void cleanup()
  {
    system("cls");
    system("color 07");
    cursor_visible(true);
  }

  void screen_setup()
  {
    system("cls");
    system("color 09");
    cursor_visible(false);
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
