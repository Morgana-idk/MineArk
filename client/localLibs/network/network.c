#include <enet/enet.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <raylib.h>

#include "../../../sharedLibs/strings/strings.h"
#include "../../../sharedLibs/player/player.h"
#include "../../../sharedLibs/clientState.h"
#include "../../../sharedLibs/worldGeneration/generation.h"
#include "../../../sharedLibs/packets/packets.h"

#define print printf


void *network(void *ClientStateX) {
    ClientState *clientState = (ClientState*)ClientStateX;
    if (enet_initialize() != 0) {print("Não foi possivel inicializar o Enet."); return NULL;}
    // Enet Inicializa!


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

        while (enet_host_service(client, &serverResponse, 16) > 0) {
            if (serverResponse.type == ENET_EVENT_TYPE_RECEIVE) {

                if (serverResponse.packet->dataLength == sizeof(PlayerPacket)) {
                    PlayerPacket *pPacket = (PlayerPacket*)serverResponse.packet->data;
                    if (strncmp(pPacket->packet, "JOINSUCESS|", 11) == 0) {
                        if (&pPacket->player != NULL) {
                            clientState->clientPlayer = pPacket->player;

                            clientState->playerCarregado = true;
                        }
                    }

                }

                if (serverResponse.packet->dataLength == sizeof(ChunkPacket)) {
                    ChunkPacket *chunkPacket = (ChunkPacket*)serverResponse.packet->data;
                    if (strncmp(chunkPacket->packet, "ALLOC|", 6) == 0) {

                        int id = chunkPacket->chunk.id;

                        if (id < 0 || id >= 1024) {
                            printf("ERRO: ID de chunk inválido: %d\n", id);
                            continue;
                        }

                        Chunk *chunk = &clientState->clientWorld.chunks[id];
                        chunk->id = id;
                        chunk->blocks_size = 0;
                        chunk->blocks = calloc(1024, sizeof(Block));

                        if (chunk->blocks == NULL) {
                            printf("ERRO: não foi possível alocar os blocos do chunk %d\n", chunk->id);
                            continue;
                        }

                        clientState->clientWorld.size++;
                    }
                }

                if (serverResponse.packet->dataLength == sizeof(BlockPacket)) {
                    BlockPacket *blockPacket = (BlockPacket*)serverResponse.packet->data;
                    if (strncmp(blockPacket->packet, "ADD|", 4) == 0) {
                        if (clientState->clientWorld.chunks[blockPacket->block.chunkMom].blocks == NULL) {
                            printf("ERRO: chunk %d não possui blocks!\n", blockPacket->block.chunkMom);
                            continue;
                        }

                        clientState->clientWorld.chunks[blockPacket->block.chunkMom].blocks[clientState->clientWorld.chunks[blockPacket->block.chunkMom].blocks_size] = blockPacket->block;
                        clientState->clientWorld.chunks[blockPacket->block.chunkMom].blocks_size++;
                    }
                }

                enet_packet_destroy(serverResponse.packet);
            }
        }
    }
 
    enet_host_destroy(client);
    enet_deinitialize(); // Enet Desinicializa!

    return NULL;
}

void networkStart(ClientState *clientState) {
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, network, clientState);
    pthread_detach(clientThread);
}