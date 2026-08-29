#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include "../otherLibs/raygui.h"
#include <string.h>
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"

bool conectado = false;
bool playerCarregado = false;

Player clientPlayer = {0};

int main() {
    if (enet_initialize() != 0) {printf("Não foi possivel inicializar o Enet."); return 1;}
    // Enet Inicializa!

    printf("Enet inicializou!\n");

    ENetHost *client = enet_host_create(NULL, 1, 2, 0, 0);

    ENetAddress endereco;
    enet_address_set_host(&endereco, "127.0.0.1"); // IP do servidor
    endereco.port = 12345; // Porta do servidor

    ENetPeer *conexao = NULL;

    if (!conectado) {
        conexao = enet_host_connect(client, &endereco, 2, 0);
        ENetEvent connectEvent;

        while (enet_host_service(client, &connectEvent, 5000) > 0) {
            if (connectEvent.type == ENET_EVENT_TYPE_CONNECT) {
                printf("Conectado ao servidor!\n");
                conectado = true;
                break;
            }
        }
    }

    if (!conectado) {printf("Não foi possivel se conectar ao servidor."); return 1;}
    char name[21] = "Modulescript";
    char name_weld[31];
    snprintf(name_weld, sizeof(name_weld), "JOIN|%s", name);

    ENetPacket *JoinPacket = enet_packet_create(name_weld, strlen(name_weld) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(conexao, 0, JoinPacket);
    enet_host_flush(client);

    ENetEvent serverResponse;

    while (1) {
        if (playerCarregado) {
            if (client->peers != NULL && client->peers->data != NULL) {
                Player *peerPlayer = (Player*)client->peers->data;
                clientPlayer = *peerPlayer;
            }
        }

        while (enet_host_service(client, &serverResponse, 16) > 0) {
            if (serverResponse.type == ENET_EVENT_TYPE_RECEIVE) {
                char *stringResponse = (char*)serverResponse.packet->data;

                printf("Recebi: %s", stringResponse);

                if (strncmp(stringResponse, "JOINSUCESS|", 11) == 0) {
                    if (client->peers != NULL) {
                        Player *playerDesmontado = calloc(1, sizeof(Player));
                        PlayerDesmontarPlayer(playerDesmontado, stringResponse);

                        clientPlayer = *playerDesmontado;
                        free(playerDesmontado);

                        playerCarregado = true;
                    }
                }

                enet_packet_destroy(serverResponse.packet);
            }
        }
        if (playerCarregado) {
            system("clear");
            printf("\n------VOCÊ-----\n");
            printf("Nome: %s\n", clientPlayer.name);
            printf("\nPosição: {\n    X: %.2f,\n    Y: %.2f,\n    Z: %.2f\n}\n\n", clientPlayer.position.x, clientPlayer.position.y, clientPlayer.position.z);
            printf("Vida: %.2f\n", clientPlayer.health);
            printf("Yaw: %.2f | Pitch: %.2f\n", clientPlayer.yaw, clientPlayer.pitch);
            printf("-----------------\n");
        }

        if (&clientPlayer != NULL && client->peers != NULL) {
            client->peers->data = &clientPlayer;
        }
    }
 
    enet_host_destroy(client);
    enet_deinitialize(); // Enet Desinicializa!
    printf("Enet desinicializou!\n");

    return 0;
}