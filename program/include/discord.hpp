#pragma once

#include <string>

namespace tmp::discord
{
    void update_presence(std::string song_name);
    void update_progress(int progress);
    void update_pause(bool current_song_paused);

    void init();
    void cleanup();
}
