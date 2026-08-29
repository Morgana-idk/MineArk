#include "player.h"
#include <string.h>
#include "../strings/strings.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void PlayerDesmontarPlayer(Player *destino, char *string) {
    fraseSep stringSeparada = separarStrings("|", string, 4);

    if (stringSeparada.dados != NULL) {
        fraseSep posS = separarStrings(",", stringSeparada.dados[1], 2);
        fraseSep veloS = separarStrings(",", stringSeparada.dados[2], 2);
        fraseSep YPH = separarStrings(",", stringSeparada.dados[3], 2); // yaw, pitch, health

        destino->position = (Vector3){
            atof(posS.dados[0]),
            atof(posS.dados[1]),
            atof(posS.dados[2])
        };

        destino->velocity = (Vector3){
            atof(veloS.dados[0]),
            atof(veloS.dados[1]),
            atof(veloS.dados[2])
        };

        destino->yaw = atof(YPH.dados[0]);
        destino->pitch = atof(YPH.dados[1]);
        destino->health = atof(YPH.dados[2]);
        snprintf(destino->name, sizeof(destino->name), "%s", strdup(stringSeparada.dados[4]));
    }
}

Player *buildPlayer() {
    Player *player = calloc(1, sizeof(Player));

    if (player == NULL) return NULL;

    player->position = (Vector3){0.0f, 0.0f, 0.0f};
    player->velocity = (Vector3){0.0f, 0.0f, 0.0f};
    player->yaw = 0.0f;
    player->pitch = 1.0f;
    player->health = 20.0f;

    sprintf(player->name, "GuestN");

    return player;
}

void *stringMontarPlayer(Player player, char *destino, int size, char *params) {
    snprintf(destino, size, "%s|%f,%f,%f|%f,%f,%f|%f,%f,%f|%s", params,
        player.position.x, player.position.y, player.position.z,
        player.velocity.x, player.velocity.y, player.velocity.z,
        player.yaw, player.pitch, player.health,
        player.name
    );
}