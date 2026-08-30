#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H
#include "player/player.h"
#include "worldGeneration/generation.h"

typedef struct {
    bool conectado;
    bool playerCarregado;
    Player clientPlayer;
    bool onMenu;

    World clientWorld;
} ClientState;

#endif