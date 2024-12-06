#pragma once
#include "goinc.h"
#ifdef __cplusplus
extern "C" {
#endif

    extern __declspec(dllexport) void sttJsonToText(GoUint8* decBuf, GoUint32 decBufLen, GoString js);
    extern __declspec(dllexport) void connectToApp();
    extern __declspec(dllexport) void closeConnection();
    extern __declspec(dllexport) void sendStringToApp(GoString str);

#ifdef __cplusplus
}
#endif