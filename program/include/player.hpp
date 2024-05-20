#pragma once

#include <minwindef.h>

namespace tmp
{
    class Player
    {
        friend class App;

        public:
            static bool song_ended();

            static bool song_playing;

        private:
            static void open();
            static void set_volume(int volume);
            static void play();
            static void close();
            static void resume();
            static void pause();
            static DWORD get_progress();
    };
}
