#include <stdio.h>
#include <raylib.h>
#include "../otherLibs/raygui.h"
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"
#include "localLibs/network/network.h"
#include "../sharedLibs/clientState.h"

int main() {
    ClientState clientState = {false, false, {0}};

    networkStart(&clientState);

    return 0;
}