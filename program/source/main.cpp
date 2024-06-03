#include "app.hpp"
#include "player.hpp"

void app_loop()
{
  tmp::app.play_song();

  while (tmp::player::song_playing)
  {
    if (tmp::player::song_ended()) tmp::app.close_song();

    tmp::app.read_input();
    tmp::app.display_info();
  }
}

int main()
{
  tmp::app.init();

  while (tmp::app.running) app_loop();

  tmp::app.cleanup();
  return 0;
}
