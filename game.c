#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include "libs/raygui.h"
#include <string.h>

bool conectado = false;

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
    snprintf(name_weld, sizeof(name_weld), "SETNAME|%s", name);

    ENetPacket *SetNamePacket = enet_packet_create(name_weld, strlen(name_weld) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(conexao, 0, SetNamePacket);
    enet_host_flush(client);

    ENetEvent serverResponse;

    while (1) {
        while (enet_host_service(client, &serverResponse, 16) > 0) {
            if (serverResponse.type == ENET_EVENT_TYPE_RECEIVE) {
                char *stringResponse = (char*)serverResponse.packet->data;

                printf("Recebi: %s", stringResponse);
                enet_packet_destroy(serverResponse.packet);
            }
        }
    }
 
    enet_host_destroy(client);
    enet_deinitialize(); // Enet Desinicializa!
    printf("Enet desinicializou!\n");

    return 0;
}