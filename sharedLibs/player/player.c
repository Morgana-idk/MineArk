#include "../strings/strings.h"
#include "../render/render.h"

#include <raylib.h>
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <string.h>

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

    Vector3 torsoPos = newVector3(0, 1.125f, 0);

    player->position = newVector3(0, 0, 0);
    player->velocity = newVector3(0, 0, 0);
    player->yaw = 0;
    player->pitch = 1;
    player->health = 20;

    NormalMember head = {
        Vector3Add(torsoPos, newVector3(0.0f, 0.625f, 0.0f)),
        Vector3Add(torsoPos, newVector3(0, 0.375f, 0)),
        0,
        newVector3(0.5f, 0.5f, 0.5f)
    };

    NormalMember leftArm = {
        Vector3Add(torsoPos, newVector3(0.375f, 0.0f, 0.0f)),
        Vector3Add(torsoPos, newVector3(0.375f, 0.3125f, 0.0f)),
        0,
        newVector3(0.25f, 0.75f, 0.25f)
    };

    NormalMember rightArm = {
        Vector3Add(torsoPos, newVector3(-0.375f, 0.0f, 0.0f)),
        Vector3Add(torsoPos, newVector3(-0.375f, 0.3125f, 0.0f)),
        0,
        newVector3(0.25f, 0.75f, 0.25f)
    };

    NormalMember leftLeg = {
        Vector3Add(torsoPos, newVector3(0.125f, -0.75f, 0.0f)),
        Vector3Add(torsoPos, newVector3(0.125f, -0.375f, 0.0f)),
        0,
        newVector3(0.25f, 0.75f, 0.25f)
    };

    NormalMember rightLeg = {
        Vector3Add(torsoPos, newVector3(-0.125f, -0.75f, 0.0f)),
        Vector3Add(torsoPos, newVector3(-0.125f, -0.375f, 0.0f)),
        0,
        newVector3(0.25f, 0.75f, 0.25f)
    };

    Torso torso = {
        head,
        leftArm, rightArm,
        leftLeg, rightLeg,

        torsoPos,
        Vector3Add(player->position, newVector3(0, 0.75f, 0)),
        0,
        newVector3(0.5f, 0.75f, 0.26f)
    };

    PlayerModel playerModel = {
        torso
    };

    player->playerModel = playerModel;

    if (player == NULL) return NULL;

    sprintf(player->name, "%s", "GuestN");

    return player;
}

void *stringMontarPlayer(Player player, char *destino, int size, char *params) {
    snprintf(destino, size, "%s|%f,%f,%f|%f,%f,%f|%f,%f,%f|%s", params,
        player.position.x, player.position.y, player.position.z,
        player.velocity.x, player.velocity.y, player.velocity.z,
        player.yaw, player.pitch, player.health,
        player.name
    );

    return NULL;
}