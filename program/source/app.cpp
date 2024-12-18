#include <algorithm>
#include <conio.h>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>
#include <windows.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <fileref.h>

#include "app.hpp"
#include "discord.hpp"
#include "platform.hpp"
#include "player.hpp"

namespace tmp
{
  App app;

  std::map<std::string, std::string> replace_chars = {
    {"“", "\""}, {"”", "\""}, {"’", "'"},  {"–", "-"},  {"á", "a"}, {"å", "a"}, {"é", "e"}, {"í", "i"},
    {"ó", "o"},  {"Ö", "O"},  {"ú", "u"},  {"ñ", "n"},  {"ç", "c"}, {"ä", "a"}, {"ë", "e"}, {"ï", "i"},
    {"ö", "o"},  {"ü", "u"},  {"Á", "A"},  {"É", "E"},  {"Í", "I"}, {"Ó", "O"}, {"Ú", "U"}, {"Ñ", "N"},
    {"Ç", "C"},  {"Ä", "A"},  {"Ë", "E"},  {"Ï", "I"},  {"Ü", "U"}, {"ß", "B"}, {"ø", "o"}, {"Ø", "O"},
    {"æ", "ae"}, {"Æ", "AE"}, {"œ", "oe"}, {"Œ", "CE"}, {"Å", "A"}, {"Þ", "D"}, {"þ", "b"}, {"ð", "d"},
    {"Ý", "Y"},  {"ý", "y"},  {"ÿ", "y"},  {"Ÿ", "Y"},  {"Š", "S"}, {"š", "s"}, {"Ž", "Z"}, {"ž", "z"}};

  void App::verify_arguments(int argc, char *argv[])
  {
    if (argc == 2 && argv[1] == std::string("-c"))
      return;
    else if (argc == 2 && argv[1] == std::string("-r"))
      return;

    std::set<std::string> arguments;
    for (int index = 1; index < argc; index++)
    {
      if (arguments.contains(argv[index]))
      {
        std::cout << "Duplicate file: " << argv[index] << std::endl;

        system("color 07");
        tmp::platform::cursor_visible(true);
        tmp::discord::cleanup();
        exit(1);
      }
      arguments.insert(argv[index]);

      if (!(std::find(files.begin(), files.end(), argv[index]) != files.end()))
      {
        std::cout << "File not found: " << argv[index] << std::endl;

        system("color 07");
        tmp::platform::cursor_visible(true);
        tmp::discord::cleanup();
        exit(1);
      }
    }
  }

  void App::playable_check()
  {
    int songs_checked = 0;
    for (auto &file : files)
    {
      choose_song(file.c_str());
      Mix_Music *music = Mix_LoadMUS(app.current_song_path.c_str());
      if (music == nullptr)
      {
        cleanup();
        std::cout << "Error loading file: " << app.current_song_path << ": " << Mix_GetError() << std::endl;
        std::cout << "Songs checked: " << songs_checked << std::endl;
        refresh_file_chache();
      }
      std::cout << "File loaded: " << app.current_song_path << std::endl;
      songs_checked++;

      Mix_FreeMusic(music);
      music = nullptr;
    }
    std::cout << "Success - Songs checked: " << songs_checked << std::endl;
    refresh_file_chache();
  }

  void App::play_song(int argc, char *argv[])
  {
    current_song++;

    bool previous_session_exists = false;
    if (current_song == 1) previous_session_exists = remember_previous_session();

    if ((argc == 1 || current_song >= argc) && !previous_session_exists)
      choose_random_song();
    else if (argc > 1)
    {
      previous_session_exists = false;
      if (argv[current_song] == std::string("-c"))
        playable_check();
      else if (argv[current_song] == std::string("-r"))
        refresh_file_chache();
      else
        choose_song(argv[current_song]);
    }

    if (previous_session_exists) choose_song(previous_session_song_name.c_str());
    tmp::player::open();
    tmp::player::set_volume(volume);
    tmp::player::play();
    if (previous_session_exists)
    {
      size_t colon_pos = previous_session_song_display_length.find(':');
      int minutes = std::stoi(previous_session_song_display_length.substr(0, colon_pos));
      int seconds = std::stoi(previous_session_song_display_length.substr(colon_pos + 1));
      int time = minutes * 60 + seconds;
      double previous_session_song_progress_seconds = (previous_session_song_progress * time) / 100.0;
      tmp::player::seek(previous_session_song_progress_seconds);
      current_song_progress = previous_session_song_progress;
      current_song_display_length = previous_session_song_display_length;
    }
    display_song();
    tmp::player::song_playing = true;
    current_song_paused = false;
  }

  void App::read_input()
  {
    if (GetAsyncKeyState(VK_MEDIA_PLAY_PAUSE))
      resume_or_pause();
    else if (GetAsyncKeyState(VK_MEDIA_NEXT_TRACK))
      close_song();
    else if (GetAsyncKeyState(VK_MEDIA_STOP))
      quit_app();

    if (_kbhit())
    {
      char key = (char)_getch();

      if (key == 'p' || key == 'P')
        resume_or_pause();
      else if (key == 'u' || key == 'U')
        increase_volume();
      else if (key == 'd' || key == 'D')
        decrease_volume();
      else if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' ||
               key == '7' || key == '8' || key == '9')
        seek_to(key);
      else if (key == 'n' || key == 'N')
        close_song();
      else if (key == 'q' || key == 'Q')
        quit_app();
    }

    Sleep(wait_time);
  }

  void App::display_info()
  {
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_song_buffer_info))
    {
      current_song_bar_width = current_song_buffer_info.srWindow.Right - current_song_buffer_info.srWindow.Left - 17;
      current_song_bar_height = current_song_buffer_info.srWindow.Bottom - current_song_buffer_info.srWindow.Top - 1;
    }

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, static_cast<short>(current_song)});

    size_t colon_pos = current_song_display_length.find(':');
    int minutes = std::stoi(current_song_display_length.substr(0, colon_pos));
    int seconds = std::stoi(current_song_display_length.substr(colon_pos + 1));
    int time = minutes * 60 + seconds;

    double progress = tmp::player::get_progress();
    int progress_percent = int(progress / time * 100.0);
    std::string progress_percent_blank_space;
    if (progress_percent < 10)
      progress_percent_blank_space = "   ";
    else if (progress_percent < 100 && progress_percent >= 10)
      progress_percent_blank_space = "  ";
    else
      progress_percent_blank_space = " ";

    std::string volume_percent_blank_space;
    if (volume < 10)
      volume_percent_blank_space = "  ";
    else if (volume < 100 && volume >= 10)
      volume_percent_blank_space = " ";
    else
      volume_percent_blank_space = "";

    std::string new_lines_blank_space;
    for (int i = 0; i < current_song_bar_width; ++i) new_lines_blank_space += " ";
    std::string new_lines;
    for (int i = 0; i < current_song_bar_height - current_song; ++i) new_lines += "\n" + new_lines_blank_space;
    if (current_song_bar_height - current_song + 1 > 0) new_lines += "\n";

    int pos = (int)(progress / time * current_song_bar_width);
    std::cout << "\r" + new_lines + " [";
    for (int i = 0; i < current_song_bar_width; ++i)
    {
      if (i < pos)
        std::cout << "=";
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << ".";
    }
    if (progress_percent > 100) progress_percent = 100;
    if (progress_percent < 0) progress_percent = 0;
    std::cout << "] " << progress_percent << "%" << progress_percent_blank_space << "Vol: " << volume << "%"
              << volume_percent_blank_space << "\r";
    std::cout.flush();

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, static_cast<short>(current_song)});

    current_song_progress = progress_percent;
    tmp::discord::update_progress(current_song_progress);
    write_previous_session();
  }

  void App::close_song()
  {
    if (current_song_paused) resume_or_pause();

    tmp::player::close();
    tmp::player::song_playing = false;
  }

  void App::init()
  {
    tmp::platform::init();
    tmp::discord::init();
    init_sdl();

    songs_directory = init_songs_directory();
    init_files();
    if (files.empty())
    {
      cleanup();
      exit(1);
    }

    init_volume();
  }

  void App::cleanup()
  {
    tmp::player::close();
    tmp::platform::cleanup();
    tmp::discord::cleanup();
    cleanup_sdl();
  }

  bool App::remember_previous_session()
  {
    std::string previous_session_path = tmp::platform::working_directory + "\\user\\previous_session.txt";
    std::ifstream file_check(previous_session_path);
    bool file_exists = file_check.good();
    if (!file_exists)
    {
      std::ofstream previous_session_file(previous_session_path);
      previous_session_file << "";
      previous_session_file.close();
      return false;
    }

    std::ifstream previous_session_file(previous_session_path);
    if (!previous_session_file.is_open())
    {
      cleanup();
      exit(1);
    }

    std::string line;
    std::getline(previous_session_file, line);
    previous_session_song_name = line;
    std::getline(previous_session_file, line);
    previous_session_song_progress = std::stoi(line);
    std::getline(previous_session_file, line);
    previous_session_song_display_length = line;
    previous_session_file.close();
    return true;
  }

  void App::write_previous_session()
  {
    std::string previous_session_path = tmp::platform::working_directory + "\\user\\previous_session.txt";
    std::ofstream previous_session_file(previous_session_path);
    if (!previous_session_file.is_open())
    {
      cleanup();
      exit(1);
    }

    previous_session_file << current_song_path.substr(current_song_path.find_last_of("/") + 1) << std::endl;
    previous_session_file << current_song_progress << std::endl;
    std::string length = current_song_display_length;
    if (length.find_first_of(" ") != std::string::npos) length.replace(length.find_first_of(" "), 1, "");
    previous_session_file << length << std::endl;
    previous_session_file.close();
  }

  void App::refresh_file_chache()
  {
    std::cout << "Refreshing file cache..." << std::endl;
    Sleep(3000);

    std::string unused_files_path = tmp::platform::working_directory + "\\user\\unused_files.txt";
    if (std::filesystem::exists(unused_files_path)) std::remove(unused_files_path.c_str());

    std::string previous_session_path = tmp::platform::working_directory + "\\user\\previous_session.txt";
    if (std::filesystem::exists(previous_session_path)) std::remove(previous_session_path.c_str());

    cleanup();
    std::cout << "File cache refreshed." << std::endl;
    exit(0);
  }

  void App::choose_random_song()
  {
    if (unused_files.empty()) unused_files = files;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (int)unused_files.size() - 1);
    current_song_index = dis(gen);

    current_song_name = unused_files[(size_t)current_song_index];
    current_song_path = songs_directory + "/" + current_song_name;
    unused_files.erase(unused_files.begin() + current_song_index);

    std::ofstream unused_files_file(tmp::platform::working_directory + "\\user\\unused_files.txt");
    unused_files_file.clear();
    for (std::string file : unused_files) unused_files_file << file << std::endl;
    unused_files_file.close();
  }

  void App::choose_song(const char *arg)
  {
    if (unused_files.empty()) unused_files = files;

    current_song_index = 0;
    for (std::string file : unused_files)
    {
      if (arg == file)
        break;
      else
        current_song_index++;
    }

    current_song_name = arg;
    current_song_path = songs_directory + "/" + current_song_name;
    if (current_song_index == (int)unused_files.size() && current_song_name != unused_files.back()) return;
    unused_files.erase(unused_files.begin() + current_song_index);

    std::ofstream unused_files_file(tmp::platform::working_directory + "\\user\\unused_files.txt");
    unused_files_file.clear();
    for (std::string file : unused_files) unused_files_file << file << std::endl;
    unused_files_file.close();
  }

  void App::display_song()
  {
    TagLib::FileRef file(current_song_path.c_str());
    TagLib::String title_tag = file.tag()->title();
    std::string title_str = title_tag.to8Bit(true);
    for (auto &pair : replace_chars)
    {
      size_t pos = 0;
      while ((pos = title_str.find(pair.first)) != std::string::npos)
        title_str.replace(pos, pair.first.length(), pair.second);
    }
    TagLib::String artist_tag = file.tag()->artist();
    std::string artist_str = artist_tag.to8Bit(true);
    for (auto &pair : replace_chars)
    {
      size_t pos = 0;
      while ((pos = artist_str.find(pair.first)) != std::string::npos)
        artist_str.replace(pos, pair.first.length(), pair.second);
    }

    double duration = player::get_duration();
    std::string seconds_str;
    if (((int)duration % 60) < 10)
      seconds_str = "0" + std::to_string((int)duration % 60);
    else
      seconds_str = std::to_string((int)duration % 60);
    std::string duration_str = std::to_string((int)duration / 60) + ":" + seconds_str;

    current_song_name = title_str + " | " + artist_str + " | " + duration_str;
    std::string info = " " + std::to_string(current_song) + " | " + current_song_name;

    int screen_width = 0;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_song_buffer_info))
      screen_width = current_song_buffer_info.srWindow.Right - current_song_buffer_info.srWindow.Left + 1;
    std::string blank_space;
    for (int i = 0; i < screen_width - (int)info.size(); ++i) blank_space += " ";
    std::cout << info << blank_space << std::endl;
    tmp::discord::update_presence(current_song_name);

    std::string length;
    auto it = std::find(current_song_name.rbegin(), current_song_name.rend(), ' ');
    if (it != current_song_name.rend())
    {
      ++it;
      std::string result(it.base(), current_song_name.end());
      length = result;
    }
    current_song_display_length = length;
  }

  void App::resume_or_pause()
  {
    if (current_song_paused)
    {
      tmp::player::resume();
      system("color 09");
      current_song_paused = false;
    }
    else
    {
      tmp::player::pause();
      system("color 04");
      current_song_paused = true;
    }
    tmp::discord::update_pause(current_song_paused);
  }

  void App::increase_volume()
  {
    volume += 2;
    if (volume > 100)
      volume = 100;
    else if (volume < 0)
      volume = 0;

    tmp::player::set_volume(volume);

    std::ofstream volume_file(volume_path);
    if (!volume_file.is_open())
    {
      cleanup();
      exit(1);
    }

    volume_file << volume;
    volume_file.close();
  }

  void App::decrease_volume()
  {
    volume -= 2;
    if (volume > 100)
      volume = 100;
    else if (volume < 0)
      volume = 0;

    tmp::player::set_volume(volume);

    std::ofstream volume_file(volume_path);
    if (!volume_file.is_open())
    {
      cleanup();
      exit(1);
    }

    volume_file << volume;
    volume_file.close();
  }

  void App::seek_to(char key)
  {
    size_t colon_pos = current_song_display_length.find(':');
    int minutes = std::stoi(current_song_display_length.substr(0, colon_pos));
    int seconds = std::stoi(current_song_display_length.substr(colon_pos + 1));
    int time = minutes * 60 + seconds;
    int number = key - '0';
    tmp::player::seek((((number * 10.0) * time) / 100.0));
    current_song_progress = number * 10;
  }

  void App::quit_app()
  {
    write_previous_session();

    tmp::player::song_playing = false;
    running = false;
  }

  std::string App::init_songs_directory()
  {
    std::string songs_directory_path = tmp::platform::working_directory + "\\user\\songs_directory.txt";
    std::ifstream file_check(songs_directory_path);
    bool file_exists = file_check.good();
    if (!file_exists)
    {
      std::cout << "Must have a user\\songs.txt containing the path to your songs!" << std::endl;
      return "";
    }

    std::ifstream songs_directory_file(songs_directory_path);
    if (!songs_directory_file.is_open())
    {
      cleanup();
      exit(1);
    }

    std::string line;
    std::getline(songs_directory_file, line);
    songs_directory_file.close();
    return line;
  }

  void App::init_files()
  {
    std::string search_term = songs_directory + "\\*.mp3";

    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFile(search_term.c_str(), &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) files = std::vector<std::string>();

    do {
      if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
      files.push_back(find_data.cFileName);
    } while (FindNextFile(find_handle, &find_data) != 0);
    FindClose(find_handle);

    std::string unused_files_path = tmp::platform::working_directory + "\\user\\unused_files.txt";
    std::ifstream file_check(unused_files_path);
    bool file_exists = file_check.good();
    if (!file_exists)
    {
      std::ofstream unused_files_file(unused_files_path);
      for (std::string file : files) unused_files_file << file << std::endl;
      unused_files_file.close();
    }

    std::ifstream unused_files_file(unused_files_path);
    if (!unused_files_file.is_open())
    {
      cleanup();
      exit(1);
    }

    std::string line;
    while (std::getline(unused_files_file, line)) unused_files.push_back(line);
    unused_files_file.close();
  }

  void App::init_volume()
  {
    volume_path = tmp::platform::working_directory + "\\user\\volume.txt";
    std::ifstream file_check(volume_path);
    bool file_exists = file_check.good();
    if (!file_exists)
    {
      std::ofstream volume_file(volume_path);
      volume_file << 10;
      volume_file.close();
    }

    std::ifstream volume_file(volume_path);
    if (!volume_file.is_open())
    {
      volume = 10;
      return;
    }

    std::string line;
    std::getline(volume_file, line);
    volume_file.close();
    volume = std::stoi(line);
  }

  void App::init_sdl()
  {
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
      std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
      exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
      std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
      exit(1);
    }
  }

  void App::cleanup_sdl()
  {
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
  }
}
