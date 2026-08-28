#include <stdio.h>
#include <enet/enet.h>
#include <raylib.h>
#include <string.h>

char ultimoLog[2048] = "\0";
int connectedClients = 0;

typedef struct {
    char **dados;
    char *buffer;
} fraseSep;

fraseSep separarStrings(char *caractere, char *frase, int range) {
    fraseSep fraseSeparada = {0};

    if (frase == NULL || caractere == NULL) return fraseSeparada;

    fraseSeparada.dados = calloc(range + 1, sizeof(char*));
    
    if (fraseSeparada.dados == NULL) return fraseSeparada;

    fraseSeparada.buffer = strdup(frase);
    if (fraseSeparada.buffer == NULL) {
        free(fraseSeparada.dados);
        fraseSeparada.dados = NULL;
        return fraseSeparada;
    }

    fraseSeparada.dados[0] = strtok(fraseSeparada.buffer, caractere);

    for (int i = 1; i <= range; i++) {
        fraseSeparada.dados[i] = strtok(NULL, caractere);
        if (fraseSeparada.dados[i] == NULL) {
            break;
        }
    }

    return fraseSeparada;
}

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
                    if (server->peers[i].data != NULL && server->peers[i].data != NULL) {
                        printf("|--: %s\n", server->peers[i].data);
                    } else {
                        printf("|--: [INVÁLIDO]\n");
                    }
                }
            }
        }
        printf("\n|ULTIMO LOG: %s|\n", ultimoLog);
        printf("-------------------\n");

        while (enet_host_service(server, &clientResponse, 16) > 0) {
            switch (clientResponse.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    snprintf(ultimoLog, sizeof(ultimoLog), "Novo cliente conectou! Porta: %d", clientResponse.peer->address.port);
                    break;
                case ENET_EVENT_TYPE_RECEIVE: {
                    char *stringResponse = (char*)clientResponse.packet->data;
                    snprintf(ultimoLog, sizeof(ultimoLog), "Recebi: %s", stringResponse);

                    if (strncmp(stringResponse, "SETNAME|", 8) == 0) {
                        fraseSep fraseSeparada = separarStrings("|", stringResponse, 1);

                        clientResponse.peer->data = strdup(fraseSeparada.dados[1]);
                    }

                    enet_packet_destroy(clientResponse.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    snprintf(ultimoLog, sizeof(ultimoLog), "Cliente desconectou!");
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize(); // // Enet Desinicializa!
    printf("Enet desinicializou!\n");

    return 0;
}