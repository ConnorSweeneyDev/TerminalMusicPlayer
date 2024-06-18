#include "app.hpp"
#include "player.hpp"

void app_loop(int argc, char *argv[])
{
  tmp::app.play_song(argc, argv);

  while (tmp::player::song_playing)
  {
    if (tmp::player::song_ended()) tmp::app.close_song();

    tmp::app.read_input();
    tmp::app.display_info();
  }
}

int main(int argc, char *argv[])
{
  tmp::app.init();
  if (argc > 1) tmp::app.verify_arguments(argc, argv);

  while (tmp::app.running) app_loop(argc, argv);

  tmp::app.cleanup();
  return 0;
}
