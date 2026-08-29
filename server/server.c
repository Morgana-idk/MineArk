#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include <string.h>
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"

char ultimoLog[2048] = "\0";
int connectedClients = 0;


int main() {
    if (enet_initialize() != 0) {printf("Não foi possivel inicializar o Enet."); return 1;}
    // Inicializa o Enet!

    ENetAddress endereco;
    enet_address_set_host(&endereco, "127.0.0.1"); // IP do servidor
    endereco.port = 12345; // Porta do servidor

    ENetHost *server = enet_host_create(&endereco, 32, 2, 0, 0);
    if (server == NULL) {
        printf("Não foi possível criar o servidor.\n");
        return 1;
    }
    snprintf(ultimoLog, sizeof(ultimoLog), "Servidor criado com sucesso!");

    ENetEvent clientResponse;
    while (1) {

        while (enet_host_service(server, &clientResponse, 16) > 0) {
            switch (clientResponse.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    snprintf(ultimoLog, sizeof(ultimoLog), "Novo cliente conectou! Porta: %d", clientResponse.peer->address.port);
                    clientResponse.peer->data = NULL;
                    break;
                case ENET_EVENT_TYPE_RECEIVE: {
                    char *stringResponse = (char*)clientResponse.packet->data;
                    snprintf(ultimoLog, sizeof(ultimoLog), "Recebi: %s", stringResponse);

                    if (strncmp(stringResponse, "JOIN|", 5) == 0) {
                        if (clientResponse.peer->data == NULL && connectedClients < 32) {
                            snprintf(ultimoLog, sizeof(ultimoLog), "Cliente pediu novo Player!");

                            char *token = strtok(strdup(stringResponse), "|");
                            char *name = strtok(NULL, "|");

                            (void)token;

                            Player *newPlayer = buildPlayer();

                            if (newPlayer != NULL) {
                                if (name != NULL) {
                                    snprintf(newPlayer->name, sizeof(newPlayer->name), "%s", name);
                                }

                                clientResponse.peer->data = newPlayer;

                                char newPlayerSerial[1024];
                                stringMontarPlayer(*newPlayer, newPlayerSerial, 1024, "JOINSUCESS");

                                if (newPlayerSerial) {
                                    ENetPacket *sucessJoin = enet_packet_create(newPlayerSerial, strlen(newPlayerSerial) + 1, ENET_PACKET_FLAG_RELIABLE);
                                    if (sucessJoin != NULL) {
                                        enet_peer_send(clientResponse.peer, 0, sucessJoin);
                                        enet_host_flush(server);
                                    }
                                }
                            }
                        }
                    }

                    enet_packet_destroy(clientResponse.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    if (clientResponse.peer->data != NULL) {
                        free(clientResponse.peer->data);
                        clientResponse.peer->data = NULL;
                    }

                    snprintf(ultimoLog, sizeof(ultimoLog), "Cliente desconectou!");
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }

        connectedClients = 0;
        for (size_t i = 0; i < server->peerCount; i++) {
            if (server->peers[i].state == ENET_PEER_STATE_CONNECTED) {
                connectedClients++;
            }
        }
        
        system("clear");
        printf("-----CLIENTES-----\n");
        printf("|QUANTIDADE: %d|\n", connectedClients);

        if (connectedClients <= 0) {
            printf("\nNENHUM CLIENTE CONECTADO.\n\n");
        } else {
            for (size_t i = 0; i < server->peerCount; i++) {
                if (server->peers[i].state == ENET_PEER_STATE_CONNECTED) {
                    Player *peerPlayer = (Player*)server->peers[i].data;

                    if (peerPlayer != NULL) {
                        printf("|--: %s\n", peerPlayer->name);
                    } else {
                        printf("|--: [INVÁLIDO]\n");
                    }
                }
            }
        }

        printf("\n|ULTIMO LOG: %s|\n", ultimoLog);
        printf("-------------------\n");
    }

    enet_host_destroy(server);
    enet_deinitialize(); // // Enet Desinicializa!
    printf("Enet desinicializou!\n");

    return 0;
}