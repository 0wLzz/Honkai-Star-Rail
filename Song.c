#include <windows.h>
#include <MMsystem.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

void sound() {
    PlaySound(TEXT("Song/Song.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
