#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include "../sharedLibs/player/player.h"
#include "../sharedLibs/strings/strings.h"
#include "../sharedLibs/render/render.h"
#include "../sharedLibs/worldGeneration/generation.h"
#include "../sharedLibs/packets/packets.h"

char ultimoLog[2048] = "\0";
int connectedClients = 0;
PlayerList playerList = {0};
World serverWorld = {0};

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

    playerList.player = calloc(32, sizeof(Player));
    playerList.size = 0;

    serverWorld.chunks = calloc(50, sizeof(Chunk));
    serverWorld.size = 0;

    Chunk basicChunk = generateChunk(newVector3(0, 0, 0), serverWorld.size);
    serverWorld.chunks[serverWorld.size] = basicChunk;
    serverWorld.size++;

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
                        if (clientResponse.peer->data == NULL && playerList.size < 32) {
                            snprintf(ultimoLog, sizeof(ultimoLog), "Cliente pediu novo Player!");

                            char *token = strtok(strdup(stringResponse), "|");
                            char *name = strtok(NULL, "|");

                            (void)token;

                            Player *newPlayer = buildPlayer();

                            if (newPlayer != NULL) {
                                if (name != NULL) {
                                    snprintf(newPlayer->name, sizeof(newPlayer->name), "%s", name);
                                }
                                clientResponse.peer->data = name;
                                playerList.player[playerList.size] = *newPlayer;
                                playerList.size++;

                                PlayerPacket JOINSUCESS_PPacket = {*newPlayer, "JOINSUCESS|"};

                                ENetPacket *sucessJoin = enet_packet_create(&JOINSUCESS_PPacket, sizeof(PlayerPacket), ENET_PACKET_FLAG_RELIABLE);
                                if (sucessJoin != NULL) {
                                    enet_peer_send(clientResponse.peer, 0, sucessJoin);

                                    for (int i = 0; i < serverWorld.size; i++) {
                                        Chunk c = serverWorld.chunks[i];
                                        ChunkPacket allocChunkPacket = {c, "ALLOC|"};

                                        ENetPacket *allocPacket = enet_packet_create(&allocChunkPacket, sizeof(ChunkPacket), ENET_PACKET_FLAG_RELIABLE);
                                        enet_peer_send(clientResponse.peer, 0, allocPacket);

                                        for (int j = 0; j < serverWorld.chunks[i].blocks_size; j++) {
                                            Block b = serverWorld.chunks[i].blocks[j];
                                            BlockPacket addBlockPacket = {b, "ADD|"};

                                            ENetPacket *addPacket = enet_packet_create(&addBlockPacket, sizeof(BlockPacket), ENET_PACKET_FLAG_RELIABLE);
                                            enet_peer_send(clientResponse.peer, 0, addPacket);
                                        }
                                    }

                                    enet_host_flush(server);
                                }
                            }
                        }
                    }

                    enet_packet_destroy(clientResponse.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    for (size_t i = 0; i < playerList.size; i++) {
                        if (strcmp(playerList.player[i].name, clientResponse.peer->data) == 0) {
                            playerList.player[i] = playerList.player[playerList.size - 1];
                            playerList.player[playerList.size - 1].velocity = newVector3(-84848484, -848484, -8484);
                            playerList.size--;
                            break;
                        }
                    }
                    clientResponse.peer->data = NULL;
                    
                    snprintf(ultimoLog, sizeof(ultimoLog), "Cliente desconectou!");
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
        
        system("clear");
        printf("-----CLIENTES-----\n");
        printf("|QUANTIDADE: %d|\n", playerList.size);

        if (playerList.size <= 0) {
            printf("\nNENHUM CLIENTE CONECTADO.\n\n");
        } else {
            for (size_t i = 0; i < playerList.size; i++) {
                if (Vector3Equals(playerList.player[i].velocity, newVector3(-84848484, -848484, -8484)) > 0) continue;
                printf("|--: %s\n", playerList.player[i].name);    
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