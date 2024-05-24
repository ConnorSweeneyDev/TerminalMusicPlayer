#pragma once

#include <string>
#include <vector>
#include <windows.h>

namespace tmp
{
    class App
    {
        friend class Player;

        public:
            void play_song();
            void read_input();
            void close_song();
            void display_info();

            void init();
            void cleanup();

            bool running = true;
            int current_song = 0;

        private:
            void load_files();
            void init_volume();

            void choose_random_song();
            void display_song();
            void resume_or_pause();
            void increase_volume();
            void decrease_volume();
            void quit_app();

            std::string songs_directory;
            std::string volume_path;
            std::vector<std::string> files;
            int volume;

            int current_song_index;
            std::string current_song_name;
            std::string current_song_path;
            std::string current_song_display_length;
            bool current_song_paused;
            int current_song_progress;

            CONSOLE_SCREEN_BUFFER_INFO current_song_buffer_info;
            int bar_width;
            int bar_height;

            DWORD wait_time = 100;
    };
    extern App app;
}
