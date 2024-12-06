#ifndef LIFESIMGAME_STEAM_H
#define LIFESIMGAME_STEAM_H

#include "steam/steam_api_flat.h"
namespace STEAM {
#include <string.h>
    volatile bool isSteamWorking = false;
    ISteamScreenshots * steamScreenShots;
    ISteamUser * steamSelf;
    ISteamUtils * steamUtils;
    ISteamNetworkingUtils * steamNetworkingUtils;
    ISteamRemoteStorage * steamRemoteStorage;

    bool Steam_Init() {

        if (!SteamAPI_Init()) {
            const char *errorTitle = "Error";
            const char *errorMessage = "Steam failed initialization";
            STEAM::isSteamWorking = false;
        } else STEAM::isSteamWorking = true;
        if (!SteamAPI_IsSteamRunning()) STEAM::isSteamWorking = false;

        if (isSteamWorking) {
            steamScreenShots = SteamScreenshots();
            SteamAPI_ISteamScreenshots_HookScreenshots(steamScreenShots, false);
            steamSelf = SteamAPI_SteamUser();
            steamUtils = SteamAPI_SteamUtils();
            steamNetworkingUtils = SteamAPI_SteamNetworkingUtils_SteamAPI();
            SteamAPI_ISteamNetworkingUtils_InitRelayNetworkAccess(steamNetworkingUtils);
            steamRemoteStorage = SteamAPI_SteamRemoteStorage();
        }

        return isSteamWorking;
    }

    void Steam_Dispose() {
        if (!isSteamWorking) return;
        SteamAPI_Shutdown();
    }
}

#endif //LIFESIMGAME_STEAM_H
