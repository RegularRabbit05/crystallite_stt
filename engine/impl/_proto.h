#pragma once

void Game_Constructor(Game* game);
void Game_Destructor(Game *game);

void Vosk_Constructor(Vosk *vosk);
void Vosk_Tick(Vosk *vosk);
std::string Vosk_PartialTop(Vosk *vosk);
std::string Vosk_QueueTop(Vosk *vosk);
void Vosk_Destructor(Vosk *vosk);

#include "Game.cpp"
#include "Vosk.cpp"
