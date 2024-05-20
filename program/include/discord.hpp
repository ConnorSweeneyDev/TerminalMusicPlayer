#pragma once

#include <string>

namespace tmp
{
    class Discord
    {
        public:
            static void update_presence(std::string song_name);
            static void update_progress(int progress);
            static void update_pause(bool current_song_paused);

            static void init();
            static void cleanup();

        private:
            static void check_for_config();
            static void start_connection();

            static std::string config_path;
            static std::string id_path;

            static std::string presence;
    };
}
