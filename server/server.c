#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include <string.h>

int main() {
    if (enet_initialize() != 0) {printf("Não foi possivel inicializar o Enet."); return 1;}
    // Inicializa o Enet!

    ENetAddress endereco;
    endereco.host = inet_addr("127.0.0.1");
    endereco.port = 12345;

    ENetHost *server = enet_host_create(&endereco, 32, 2, 0, 0);
    if (server == NULL) {
        printf("Não foi possível criar o servidor.\n");
        return 1;
    }
    printf("Servidor criado com sucesso!\n");

    ENetEvent clientResponse;
    while (1) {
        while (enet_host_service(server, &clientResponse, 16) > 0) {
            switch (clientResponse.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("Novo cliente conectou!");
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    char *stringResponse = (char*)clientResponse.packet->data;
                    printf("Recebi: %s", stringResponse);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Cliente desconectou!");
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }

    return 0;
}