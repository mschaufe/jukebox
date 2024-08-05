// Online C++ compiler to run C++ program online
#include <iostream>
#include <queue>

using namespace std;

queue<int> playlist;

void add_song_to_playlist(int song_nr){
    if(playlist.size()<10){
        playlist.push(song_nr);
    }else{
        cout<<"Queue is full\n";
    }
}

void play(){
    if(!playlist.empty()){
        cout<<"Playing "<<playlist.front()<<"\n";
        playlist.pop();
    }else{
        cout<<"Playlist is empty!\n";
    }
}

int main() {
    add_song_to_playlist(1);
    add_song_to_playlist(2);
    add_song_to_playlist(3);
    add_song_to_playlist(4);
    add_song_to_playlist(5);
    add_song_to_playlist(6);
    add_song_to_playlist(7);
    add_song_to_playlist(8);
    add_song_to_playlist(9);
    add_song_to_playlist(10);
    add_song_to_playlist(11);
    play();
    play();
    play();
    play();
    play();
    play();
    play();
    play();
    play();
    play();
    play();
    return 0;
}