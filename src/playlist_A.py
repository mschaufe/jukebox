# %% Imports
import numpy as np

# %% Variables
playlist = np.array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

# %% Functions
def add_song_to_playlist(song_nr):
    for i in range(0, len(playlist)):
        if playlist[i] == 0:
            playlist[i] = song_nr
            break
    if playlist[-1] != 0:
        print("playlist is full")

def remove_played_song():
    for i in range(0, len(playlist)-1):
        playlist[i] = playlist[i+1]
    playlist[-1] = 0

def play():
    if playlist[0] == 0:
        print("playlist is empty")
    else:
        print(f"Start playing song nr {playlist[0]}")
        remove_played_song()

def show_playlist():
    print(playlist)