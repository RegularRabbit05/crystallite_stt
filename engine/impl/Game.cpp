#pragma once

#include "../_struct.h"
#include "../../include/steam.h"
#include <thread>

namespace Crystallite::internal {
    void audioThread(Game *game) {
        while (game->_) {
            Vosk_Tick(&game->stt);
        }
    }
}

void Game_Constructor(Game* game) {
    if (game->_) return;
    game->_ = true;
    game->stt._ = false;
    Vosk_Constructor(&game->stt);
    SteamAPI_ISteamUser_StartVoiceRecording(STEAM::steamSelf);
    game->audioThread = std::thread(Crystallite::internal::audioThread, game);
}

void Game_Destructor(Game *game) {
    if (!game->_) return;
    game->_ = false;

    if (game->audioThread.joinable()) game->audioThread.join();
    Vosk_Destructor(&game->stt);
    game->stt._ = false;

    SteamAPI_ISteamUser_StopVoiceRecording(STEAM::steamSelf);
}