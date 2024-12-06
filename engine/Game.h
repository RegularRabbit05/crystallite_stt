#pragma once

#include <thread>

struct Game {
    bool _ = false;
    Vosk stt;
    std::thread audioThread;
};
