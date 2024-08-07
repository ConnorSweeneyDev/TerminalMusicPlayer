#pragma once

#include <string>
#include <vector>
#include <windows.h>

namespace tmp
{
  class App
  {
  public:
    void verify_arguments(int argc, char *argv[]);

    void playable_check();
    void play_song(int argc, char *argv[]);
    void read_input();
    void display_info();
    void close_song();

    void init();
    void cleanup();

    bool running = true;
    int current_song = 0;
    std::string current_song_path;

  private:
    bool remember_previous_session();
    void write_previous_session();
    void refresh_file_chache();
    void choose_random_song();
    void choose_song(const char *arg);
    void display_song();
    void resume_or_pause();
    void increase_volume();
    void decrease_volume();
    void seek_to(char key);
    void quit_app();

    std::string init_songs_directory();
    void init_files();
    void init_volume();
    void init_sdl();
    void cleanup_sdl();

    std::string songs_directory;
    std::string volume_path;
    std::vector<std::string> files;
    std::vector<std::string> unused_files;
    std::string previous_session_song_name;
    int previous_session_song_progress;
    std::string previous_session_song_display_length;
    int volume;

    int current_song_index;
    std::string current_song_name;
    std::string current_song_display_length;
    bool current_song_paused;
    int current_song_progress;

    CONSOLE_SCREEN_BUFFER_INFO current_song_buffer_info;
    int current_song_bar_width;
    int current_song_bar_height;

    DWORD wait_time = 100;
  };
  extern App app;
}
