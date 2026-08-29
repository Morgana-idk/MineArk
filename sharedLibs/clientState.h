#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

typedef struct {
    bool conectado;
    bool playerCarregado;
    Player clientPlayer;
} ClientState;

#endif