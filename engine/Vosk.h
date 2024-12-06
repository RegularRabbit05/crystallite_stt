#pragma once

#include <mutex>
#include <queue>

struct Vosk {
    bool _ = false;
    std::mutex mtx = std::mutex();
    std::mutex mtx2 = std::mutex();
    std::mutex mtx3 = std::mutex();
    uint64_t lastVoiceTick = 0;
    bool hadVoice = false;
    VoskModel *model = nullptr;
    VoskRecognizer *recognizer = nullptr;
    std::queue<std::string> recognized = std::queue<std::string>();
    std::queue<std::string> partial = std::queue<std::string>();
};

