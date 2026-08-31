// Shareds
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"
#include "../sharedLibs/clientState.h"
#include "../sharedLibs/render/render.h"

// Locals
#include "localLibs/network/network.h"

// Externals
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    Player *player = buildPlayer();
    ClientState clientState = {
        false,
        false,
        *player,
        false,
        {0}
    };

    clientState.clientWorld.chunks = calloc(1024, sizeof(Chunk));
    clientState.clientWorld.size = 0;

    if (clientState.clientWorld.chunks == NULL) {
        printf("Não foi possível alocar os chunks do cliente.\n");
        free(player);
        return 1;
    }

    networkStart(&clientState);
    startRender(&clientState);

    free(player);
    return 0;
}