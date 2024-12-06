#pragma once

#include <cstdint>
#include "_proto.h"
#include "../../config.h"
#include "../../include/vosk_api.h"
#include "../../include/goinc.h"
namespace CryDec {
#include "../../include/CryDec.h"
}

void Vosk_Constructor(Vosk *vosk) {
    if (vosk->_) return;
    vosk->_ = true;
    vosk->hadVoice = false;
    vosk->recognized = std::queue<std::string>();
    vosk->partial = std::queue<std::string>();
    vosk_set_log_level(-1);
    vosk->model = vosk_model_new(CUSTOM_PATH"vosk");
    vosk->recognizer = vosk_recognizer_new(vosk->model, VOICE_RATE);
}

namespace Crystallite::internal {
    int voice_tick_steam(Vosk *vosk) {
        uint32_t sz, compSz, decBufSz = 512*1024, decSz;
        SteamAPI_ISteamUser_GetAvailableVoice(STEAM::steamSelf, &sz, nullptr, 0);
        if (sz == 0) return 2;
        auto* compressedBuffer = (uint8_t*) alloca(sz);
        SteamAPI_ISteamUser_GetVoice(STEAM::steamSelf, true, compressedBuffer, sz, &compSz, false, nullptr, 0, nullptr, 0);
        auto* decompressBuffer = (char*) alloca(decBufSz);
        SteamAPI_ISteamUser_DecompressVoice(STEAM::steamSelf, compressedBuffer, compSz, decompressBuffer, decBufSz, &decSz, VOICE_RATE);
        if (decSz == 0) return 2;
        vosk->hadVoice = true;
        return vosk_recognizer_accept_waveform(vosk->recognizer, decompressBuffer, (int) decSz);
    }

    void voice_tick_vosk(Vosk *vosk) {
        if (vosk->lastVoiceTick+50 > currentTimeMillis()) {
            return;
        }
        vosk->lastVoiceTick = currentTimeMillis();
        int res = voice_tick_steam(vosk);
        if ((res == 2 && vosk->hadVoice) || res == 1) {
            vosk->hadVoice = false;

            std::string jj = vosk_recognizer_final_result(vosk->recognizer);
            vosk_recognizer_reset(vosk->recognizer);
            if (jj.empty()) return;
            auto* in = (char*) alloca(jj.size());
            CryDec::sttJsonToText((GoUint8*) in, jj.size(), goStringFromStr(jj.c_str()));
            if (in[0] == 0) return;
            vosk->mtx2.lock();
            vosk->recognized.emplace(in);
            vosk->mtx2.unlock();
        } else {
            std::string jj = vosk_recognizer_partial_result(vosk->recognizer);
            if (jj.empty()) return;
            auto* in = (char*) alloca(jj.size());
            CryDec::sttJsonToText((GoUint8*) in, jj.size(), goStringFromStr(jj.c_str()));
            if (in[0] == 0) return;
            vosk->mtx3.lock();
            vosk->recognized.emplace(in);
            vosk->mtx3.unlock();
        }
    }

    std::string vosk_partial_pop(Vosk *vosk) {
        if (vosk->partial.empty()) return "";
        std::string res = vosk->partial.front();
        vosk->partial.pop();
        return res;
    }

    std::string vosk_queue_pop(Vosk *vosk) {
        if (vosk->recognized.empty()) return "";
        std::string res = vosk->recognized.front();
        vosk->recognized.pop();
        return res;
    }
}

void Vosk_Tick(Vosk *vosk) {
    vosk->mtx.lock();
    Crystallite::internal::voice_tick_vosk(vosk);
    vosk->mtx.unlock();
}

std::string Vosk_PartialTop(Vosk *vosk) {
    vosk->mtx3.lock();
    std::string r = Crystallite::internal::vosk_partial_pop(vosk);
    vosk->mtx3.unlock();
    return r;
}

std::string Vosk_QueueTop(Vosk *vosk) {
    vosk->mtx2.lock();
    std::string r = Crystallite::internal::vosk_queue_pop(vosk);
    vosk->mtx2.unlock();
    return r;
}

void Vosk_Destructor(Vosk *vosk) {
    if (!vosk->_) return;
    vosk->_ = false;
    vosk_recognizer_free(vosk->recognizer);
    vosk_model_free(vosk->model);
    vosk->recognized = std::queue<std::string>();
}