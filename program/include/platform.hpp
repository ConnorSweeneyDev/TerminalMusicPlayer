#pragma once

#include <string>

namespace tmp
{
    class Platform
    {
        public:
            static void init();
            static void cleanup();

            static std::string working_directory;

        private:
            static void screen_setup();
            static void screen_shutdown();
            static void cursor_visible(bool is_visible);
            static void input_setup();
            static std::string get_working_directory();
    };
}
