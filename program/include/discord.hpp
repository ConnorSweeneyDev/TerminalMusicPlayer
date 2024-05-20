#pragma once

#include <string>

namespace tmp
{
    class Discord
    {
        public:
            void update_presence(std::string song_name);
            void update_progress(int progress);
            void update_pause(bool current_song_paused);

            void init();
            void cleanup();

        private:
            void check_for_config();
            void start_connection();

            std::string config_path;
            std::string id_path;

            std::string presence;
    };
    extern Discord discord;
}
