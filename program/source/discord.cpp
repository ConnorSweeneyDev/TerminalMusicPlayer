#include <fstream>
#include <iostream>
#include <regex>
#include <windows.h>

#include "app.hpp"
#include "discord.hpp"
#include "platform.hpp"

namespace tmp::discord
{
  std::string config_path;
  std::string id_path;
  std::string presence;

  void check_for_config();
  void start_connection();

  void update_presence(std::string song_name)
  {
    std::ofstream config_file(config_path);
    if (!config_file.is_open()) return;

    std::string optional_s = "";
    if (app.current_song != 1) optional_s = "s";

    std::string client_id = "";
    std::ifstream id_file(id_path);
    if (!id_file.is_open()) return;

    std::string line;
    std::getline(id_file, line);
    client_id = line;

    id_file.close();

    presence = "[Identifiers]\nClientID=" + client_id + "\n\n[State]\nState=" + std::to_string(app.current_song) +
               " Song" + optional_s + " This Session | Playing | 0%" + "\nDetails=" + song_name +
               "\nStartTimestamp=\nEndTimestamp=\n\n[Images]\nLargeImage=\"\"\nLargeImageTooltip="
               "\nSmallImage=\"\"\nSmallImageTooltip=";

    config_file << presence;
    config_file.close();
  }

  void update_progress(int progress)
  {
    std::ofstream config_file(config_path);
    if (!config_file.is_open()) return;

    size_t percent_pos = presence.find("%");
    size_t space_pos = presence.rfind(" ", percent_pos);

    std::string old_progress = presence.substr(space_pos + 1, percent_pos - space_pos - 1);
    presence = std::regex_replace(presence, std::regex(old_progress + "%"), std::to_string(progress) + "%");

    config_file << presence;
    config_file.close();
  }

  void update_pause(bool current_song_paused)
  {
    std::ofstream config_file(config_path);
    if (!config_file.is_open()) return;

    if (current_song_paused)
      presence = std::regex_replace(presence, std::regex("Playing"), "Paused");
    else
      presence = std::regex_replace(presence, std::regex("Paused"), "Playing");

    config_file << presence;
    config_file.close();
  }

  void init()
  {
    config_path = tmp::platform::working_directory + "\\config.ini";
    id_path = tmp::platform::working_directory + "\\user\\client_id.txt";

    check_for_config();
    start_connection();
  }

  void cleanup()
  {
    system("pwsh -NoProfile -Command \"if ((Get-Process -Name 'easyrp' -ErrorAction "
           "SilentlyContinue) -eq $null) { } else { Stop-Process -Name 'easyrp' -Force }");
  }

  void check_for_config()
  {
    std::ifstream file_check(config_path);
    bool file_exists = file_check.good();
    if (!file_exists)
    {
      std::ofstream config_file(config_path);
      config_file << "";
      config_file.close();
    }
  }

  void start_connection()
  {
    cleanup();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string cmd_line = "pwsh -NoProfile -ExecutionPolicy Bypass -Command \"Start-Process -d \"" +
                           tmp::platform::working_directory + "\" -FilePath \"" + tmp::platform::working_directory +
                           "\\binary\\easyrp.exe\" -Confirm:$false -WindowStyle Hidden\"";

    TCHAR cmd_line_tchar[MAX_PATH];
#ifdef UNICODE
    _tcscpy_s(cmd_line_tchar, cmd_line.c_str());
#else
    strcpy_s(cmd_line_tchar, cmd_line.c_str());
#endif

    if (!CreateProcess(NULL, cmd_line_tchar, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
      std::cout << "CreateProcess failed: " << GetLastError() << ".\n";
      tmp::platform::cleanup();
      exit(1);
    }
  }
}
