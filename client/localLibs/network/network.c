#include <enet/enet.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <raylib.h>
#include "../../../sharedLibs/strings/strings.h"
#include "../../../sharedLibs/player/player.h"
#include "../../../sharedLibs/clientState.h"

#define print printf


void *network(void *ClientStateX) {
    ClientState *clientState = (ClientState*)ClientStateX;
    if (enet_initialize() != 0) {print("Não foi possivel inicializar o Enet."); return NULL;}
    // Enet Inicializa!

    print("Enet inicializou!\n");

    ENetHost *client = enet_host_create(NULL, 1, 2, 0, 0);

    ENetAddress endereco;
    enet_address_set_host(&endereco, "127.0.0.1"); // IP do servidor
    endereco.port = 12345; // Porta do servidor

    ENetPeer *conexao = NULL;

    if (!clientState->conectado) {
        conexao = enet_host_connect(client, &endereco, 2, 0);
        ENetEvent connectEvent;

        while (enet_host_service(client, &connectEvent, 5000) > 0) {
            if (connectEvent.type == ENET_EVENT_TYPE_CONNECT) {
                print("Conectado ao servidor!\n");
                clientState->conectado = true;
                break;
            }
        }
    }

    if (!clientState->conectado) {print("Não foi possivel se conectar ao servidor."); return NULL;}
    char name[21] = "Modulescript";
    char name_weld[31];
    snprintf(name_weld, sizeof(name_weld), "JOIN|%s", name);

    ENetPacket *JoinPacket = enet_packet_create(name_weld, strlen(name_weld) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(conexao, 0, JoinPacket);
    enet_host_flush(client);

    ENetEvent serverResponse;

    while (1) {
        if (!clientState->conectado) {
            enet_peer_disconnect_now(client->peers, 0);
        }

        if (clientState->playerCarregado) {
            if (client->peers != NULL && client->peers->data != NULL) {
                Player *peerPlayer = (Player*)client->peers->data;
                clientState->clientPlayer = *peerPlayer;
            }
        }

        while (enet_host_service(client, &serverResponse, 16) > 0) {
            if (serverResponse.type == ENET_EVENT_TYPE_RECEIVE) {
                char *stringResponse = (char*)serverResponse.packet->data;

                print("Recebi: %s", stringResponse);

                if (strncmp(stringResponse, "JOINSUCESS|", 11) == 0) {
                    if (client->peers != NULL) {
                        Player *playerDesmontado = calloc(1, sizeof(Player));
                        PlayerDesmontarPlayer(playerDesmontado, stringResponse);

                        clientState->clientPlayer = *playerDesmontado;
                        free(playerDesmontado);

                        clientState->playerCarregado = true;
                    }
                }

                enet_packet_destroy(serverResponse.packet);
            }
        }

        if (&clientState->clientPlayer != NULL && client->peers != NULL) {
            client->peers->data = &clientState->clientPlayer;
        }
    }
 
    enet_host_destroy(client);
    enet_deinitialize(); // Enet Desinicializa!
    print("Enet desinicializou!\n");

    return NULL;
}

void networkStart(ClientState *clientState) {
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, network, clientState);
    pthread_detach(clientThread);
}