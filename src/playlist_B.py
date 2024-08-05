# %% Imports
import numpy as np

# %% Variables
last = 0
max_stack = 10
stack = np.zeros(max_stack+1)

# %% Functions
def add_to_stack(song_nr):
    global last
    if last == max_stack:
        print("Playlist is full")
    else:
        last = last + 1
        stack[last] = song_nr

def play_stack():
    global last
    if last > 0:
        print(f"Start playing song nr {stack[1]}")
        for i in range(1, last):
            stack[i] = stack[i+1]
        last = last - 1
    else:
        print("Playlist is empty")

def show_playlist():
    print(stack)

