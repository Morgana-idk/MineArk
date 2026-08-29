#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct {
    Vector3 position;
    Vector3 pivot;
    float rotation;
} NormalMember;

typedef struct {
    NormalMember head;

    NormalMember leftArm;
    NormalMember rightArm;
    
    NormalMember leftLeg;
    NormalMember rightLeg;
} Torso;

typedef struct {
    Torso torso;
} PlayerModel;

typedef struct {
    Vector3 position;
    Vector3 velocity;

    float yaw;
    float pitch;

    float health;
    char name[21];
} Player;

void PlayerDesmontarPlayer(Player *destino, char *string);
Player *buildPlayer();
void *stringMontarPlayer(Player player, char *destino, int size, char *params);

#endif