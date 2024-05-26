# TerminalMusicPlayer

A fast and simple music player for the terminal. 'p' to pause, 'n' to go next, 'u' and 'd' for volume up and down, 'q' to quit.
Alternatively, media controls for pause, skip and stop work too. Also has discord integration, showing information about the song
being played, the current session, the status of the player and song progress.

https://github.com/ConnorSweeneyDev/TerminalMusicPlayer/assets/75945279/b4bbb381-3121-4223-ac8b-b01c84be278a

*Showcase: song history, progress bar, volume indicator, blue signifies unpaused, red signifies paused.*

I made this because it was annoying to have to use a mouse to control my music, however this does have some limitations - it is
windows only and mp3 only, also the only tags the mp3 files should have are `title` and `artist`, the rest are unused - I recommend using
[Mp3tag](https://www.mp3tag.de/en/download.html) to edit the tags en masse.

## Dependencies
- Windows.
- PowerShell (pwsh.exe) &rightarrow; Run `winget install --id Microsoft.WindowsTerminal`.
- MinGW 64-bit &rightarrow; Use my [version](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-17.0.6-11.0.1-ucrt-r5/winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r5.zip) of MinGW or replace the dlls in `binary/` with your own.
- Python &rightarrow; Run `winget install --id Python.Python.3.11`.
- Mutagen &rightarrow; Run `pip install mutagen`.
- A discord app set up on the [developer portal](https://discord.com/developers/applications). Add a new application and enable rich presence
  for it, and make note of the application id.

To run this successfully, put a folder called `user/songs/` in the root of the project, then run `update_song_names.py` using your installed
python version and make sure there are no error messages, this will rename all the files to the correct format. Create `user/client_id.txt` and
put the application id from the portal in it. After that you can build and run the C++ code using the batch files in `system/`.

## Known Issues
- The program will crash if any of the mp3 files have any of the newest IDvX tags.
- The screen can be made bigger (not smaller) when the program first starts, but after the first song any more resizing causes glitchy output.
