#include <iostream>
#include <string>
#include <windows.h>

#include "app.hpp"
#include "player.hpp"

namespace tmp::player
{
  bool song_playing = false;

  bool song_ended()
  {
    MCI_STATUS_PARMS status;
    status.dwItem = MCI_STATUS_MODE;
    UINT device_id = mciGetDeviceID("mp3");
    MCIERROR error = mciSendCommand(device_id, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&status);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error getting status: " << error_message << std::endl;
      return true;
    }

    return status.dwReturn == MCI_MODE_STOP;
  }

  void open()
  {
    std::string command = "open " + app.current_song_path + " type mpegvideo alias mp3";
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error opening file: " << app.current_song_path << ": " << error_message
                << std::endl;
      command = "close mp3";
      mciSendString(command.c_str(), NULL, 0, NULL);

      app.cleanup();
      exit(1);
    }
  }

  void set_volume(int volume)
  {
    std::string command = "setaudio mp3 volume to " + std::to_string(volume);
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error setting volume: " << error_message << std::endl;
      command = "close mp3";
      mciSendString(command.c_str(), NULL, 0, NULL);

      app.cleanup();
      exit(1);
    }
  }

  void play()
  {
    std::string command = "play mp3";
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error playing file: " << app.current_song_path << ": " << error_message
                << std::endl;
      command = "close mp3";
      mciSendString(command.c_str(), NULL, 0, NULL);

      app.cleanup();
      exit(1);
    }
  }

  void close()
  {
    std::string command = "close mp3";
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error closing file: " << app.current_song_path << ": " << error_message
                << std::endl;

      app.cleanup();
      exit(1);
    }
  }

  void resume()
  {
    std::string command = "play mp3";
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error resuming: " << error_message << std::endl;
      exit(1);
    }
  }

  void pause()
  {
    std::string command = "pause mp3";
    MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
    if (error != 0)
    {
      char error_message[256];
      mciGetErrorString(error, error_message, sizeof(error_message));
      std::cout << "Error pausing: " << error_message << std::endl;
      exit(1);
    }
  }

  DWORD get_progress()
  {
    MCI_STATUS_PARMS status;
    status.dwItem = MCI_STATUS_POSITION;

    mciSendCommand(mciGetDeviceID("mp3"), MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM,
                   reinterpret_cast<DWORD_PTR>(&status));
    return (DWORD)status.dwReturn;
  }
}
