#include <string>
#include <vector>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#include "init.hpp"
#include "song.hpp"
#include "discord.hpp"

int main()
{
    std::string cwd = init::setup_console();
    init::setup_discord(cwd);

    std::string directory = cwd + "\\Songs";
    std::vector<std::string> files = init::initialize_song_list(directory);
    if (files.empty())
    {
        discord::close_discord();
        return 1;
    }

    int volume = init::initialize_volume(cwd);

    int current_song = 0;
    while (true)
    {
        current_song++;

        int random_index;
        std::string song = song::generate_song(files, random_index);
        std::string path = "\"" + directory + "\\" + song + "\"";

        song::open_song(path);
        song::set_volume(volume);
        song::play_song(path);
        song::display_song_info(song, current_song, random_index, files.size(), cwd);

        bool is_paused = false;
        std::string command;
        while (!song::song_ended())
        {
            if (_kbhit())
            {
                char key = _getch();
                if (key == 'p')
                    song::pause_or_play(is_paused, cwd);
                else if (key == 'u')
                    song::increase_volume(volume, cwd);
                else if (key == 'd')
                    song::decrease_volume(volume, cwd);
                else if (key == 'n')
                    break;
                else if (key == 'q')
                {
                    song::close_song(path);
                    discord::close_discord();
                    return 0;
                }
            }
            Sleep(100);
        }
        song::close_song(path);
    }
    discord::close_discord();
    return 0;
}
