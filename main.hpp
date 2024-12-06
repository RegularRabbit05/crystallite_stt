#ifndef CRYSTALLITEMACHINA_ENGINE_MAIN_H
#define CRYSTALLITEMACHINA_ENGINE_MAIN_H
#include "config.h"


#include "include/goinc.h"
namespace CryDec {
    #include "include/CryDec.h"
}

#include <chrono>
#include <cstdint>
#include <iostream>

uint64_t currentTimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

#include <queue>
#include "include/vosk_api.h"

#include "include/steam.h"

#include "engine/_struct.h"
#include "engine/impl/_proto.h"

#endif
