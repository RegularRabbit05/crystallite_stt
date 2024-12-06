#include "main.hpp"

#include "include/steam.h"
#include "engine/_struct.h"
#include "engine/impl/_proto.h"

void wrapper() {
    Game game;

    fprintf(stderr, "Connecting...\n");
    CryDec::connectToApp();
    fprintf(stderr, "Preparing...\n");
    Game_Constructor(&game);
    fprintf(stderr, "Ready...\n");

    while (game._) {
        std::string complete = Vosk_QueueTop(&game.stt);
        std::string discard = Vosk_PartialTop(&game.stt);
        if (complete != "") {
            printf("Complete result: %s\n", complete.c_str());
            CryDec::sendStringToApp({.p = complete.c_str(), .n = (ptrdiff_t) complete.length()});
        }
        if (discard != "") {
            printf("Partial result: %s\n", discard.c_str());
        }
    }

    Game_Destructor(&game);
    CryDec::closeConnection();
}

int main() {
    if (!STEAM::Steam_Init()) {
        fprintf(stderr, "Steam failure\n");
        return 1;
    }
    wrapper();
    STEAM::Steam_Dispose();
    return 0;
}