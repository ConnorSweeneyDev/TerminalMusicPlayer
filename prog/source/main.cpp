#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#include "init.hpp"
#include "song.hpp"

int main()
{
    disable_visual_input();

    std::string directory = "C:\\Users\\conno\\Documents\\Programming\\C++\\TerminalMusicPlayer\\Songs";
    std::vector<std::string> files = initialize_song_list(directory);
    if (files.empty())
        return 1;

    int volume = initialize_volume();

    int current_song = 0;
    while (true)
    {
        current_song++;

        int random_index;
        std::string song = generate_song(files, random_index);
        std::string path = "\"" + directory + "\\" + song + "\"";

        open_song(path);
        set_volume(volume);
        play_song(path);
        display_song_info(song, current_song, random_index, files.size());

        std::string command;
        bool is_paused = false;
        while (!song_ended())
        {
            if (_kbhit())
            {
                char key = _getch();
                if (key == 'p')
                {
                    pause_or_play(is_paused);
                }
                else if (key == 'u')
                {
                    increase_volume(volume);
                }
                else if (key == 'd')
                {
                    decrease_volume(volume);
                }
                else if (key == 'n')
                    break;
                else if (key == 'q')
                {
                    close_song(path);
                    return 0;
                }
            }
            Sleep(100);
        }
        close_song(path);
    }
    return 0;
}
