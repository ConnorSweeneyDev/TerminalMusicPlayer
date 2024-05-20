#pragma once

#include <string>

namespace tmp
{
    class Platform
    {
        public:
            void init();
            void cleanup();

            std::string working_directory;

        private:
            void screen_setup();
            void screen_shutdown();
            void cursor_visible(bool is_visible);
            void input_setup();
            std::string get_working_directory();
    };
    extern Platform platform;
}
