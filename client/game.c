// Shareds
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"
#include "../sharedLibs/clientState.h"

// Locals
#include "localLibs/network/network.h"
#include "localLibs/render/render.h"

// Externals
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    ClientState clientState = {false, false, {0}, false};

    networkStart(&clientState);
    startRender(&clientState);

    return 0;
}