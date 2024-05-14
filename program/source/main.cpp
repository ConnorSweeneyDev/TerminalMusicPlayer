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
    std::string cwd = init::console();
    init::discord(cwd);

    std::string directory = cwd + "\\Songs";
    std::vector<std::string> files = init::song_list(directory);
    if (files.empty())
    {
        discord::close();
        system("color 07");
        init::cursor_visible(true);
        return 1;
    }

    int volume = init::volume(cwd);

    int current_song = 0;
    while (true)
    {
        current_song++;

        int random_index;
        std::string song = song::generate(files, random_index);
        std::string path = "\"" + directory + "\\" + song + "\"";

        song::open(path);
        song::set_volume(volume);
        song::play(path);
        std::string length = song::display_info(song, current_song, random_index, files.size(), cwd);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int wait_time = 100;
        int total_wait_time = 0;
        int bar_width = 0;
        int bar_height = 0;
        bool is_paused = false;
        std::string command;
        while (!song::ended())
        {
            total_wait_time += wait_time;

            if (GetAsyncKeyState(VK_MEDIA_PLAY_PAUSE))
                song::pause_or_play(is_paused, cwd);
            else if (GetAsyncKeyState(VK_MEDIA_NEXT_TRACK))
            {
                if (is_paused) song::pause_or_play(is_paused, cwd);
                break;
            }
            else if (GetAsyncKeyState(VK_MEDIA_STOP))
            {
                song::close(path);
                discord::close();
                song::progress_cleanup(bar_width, bar_height, current_song);
                system("color 07");
                system("cls");
                init::cursor_visible(true);
                return 0;
            }

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
                {
                    if (is_paused) song::pause_or_play(is_paused, cwd);
                    break;
                }
                else if (key == 'q')
                {
                    song::close(path);
                    discord::close();
                    song::progress_cleanup(bar_width, bar_height, current_song);
                    system("color 07");
                    system("cls");
                    init::cursor_visible(true);
                    return 0;
                }
            }

            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
            {
                bar_width = csbi.srWindow.Right - csbi.srWindow.Left - 19;
                bar_height = csbi.srWindow.Bottom - csbi.srWindow.Top - 1;
            }

            int progress = song::display_status_bar(length, volume, bar_width, bar_height, current_song);
            if (total_wait_time == 5000)
            {
                discord::update_progress_status(std::to_string(progress), cwd);
                total_wait_time = 0;
            }

            Sleep(wait_time);
        }
        song::progress_cleanup(bar_width, bar_height, current_song);
        song::close(path);
    }

    return 0;
}
