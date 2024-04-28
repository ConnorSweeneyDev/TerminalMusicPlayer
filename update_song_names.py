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

        characters = ["<", ">", ":", "/", "\\", "|", "?", "*"]
        for character in characters:
            if character in title:
                title = title.replace(character, "")
            if character in artist:
                artist = artist.replace(character, "")

        unsupported_quotes = ["‘", "’", "“", "”", "\""]
        for quote in unsupported_quotes:
            if quote in title:
                title = title.replace(quote, "'")
            if quote in artist:
                artist = artist.replace(quote, "'")

        if "á" in title:
            title = title.replace("á", "a")
        if "á" in artist:
            artist = artist.replace("á", "a")
        if "é" in title:
            title = title.replace("é", "e")
        if "é" in artist:
            artist = artist.replace("é", "e")
        if "í" in title:
            title = title.replace("í", "i")
        if "í" in artist:
            artist = artist.replace("í", "i")
        if "ó" in title:
            title = title.replace("ó", "o")
        if "ó" in artist:
            artist = artist.replace("ó", "o")
        if "ú" in title:
            title = title.replace("ú", "u")
        if "ú" in artist:
            artist = artist.replace("ú", "u")
        if "ñ" in title:
            title = title.replace("ñ", "n")
        if "ñ" in artist:
            artist = artist.replace("ñ", "n")

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
