import os
import math
from mutagen.mp3 import MP3

directory = "Songs"

for filename in os.listdir(directory):
    file_path = os.path.join(directory, filename)

    try:
        audio = MP3(file_path)
        title = audio["TIT2"][0]
        artist = audio["TPE1"][0]
        duration = audio.info.length
        duration = str(math.trunc(duration // 60)) + "___" + str(math.trunc(duration % 60)).zfill(2)

        characters = ["<", ">", ":", "\"", "/", "\\", "|", "?", "*"]
        for character in characters:
            if character in title:
                title = title.replace(character, "")
            if character in artist:
                artist = artist.replace(character, "")

        spaces = ["  ", "   ", "    "]
        for space in spaces:
            if space in title:
                title = title.replace(space, " ")
            if space in artist:
                artist = artist.replace(space, " ")

        new_filename = title + " --- " + artist + " --- " + duration + ".mp3"
        new_file_path = os.path.join(directory, new_filename)
        os.rename(file_path, new_file_path)

        print("Renamed " + filename + " --> " + new_filename)
    except Exception as e:
        print(f"Error renaming {filename}: {e}")
