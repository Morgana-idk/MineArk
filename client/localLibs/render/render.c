#include <raylib.h>
#include <stdio.h>
#include "../../../otherLibs/raygui.h"
#include "../../../sharedLibs/clientState.h"

Vector3 newVector3(float x, float y, float z) {
    return (Vector3){(float)x, (float)y, (float)z};
}

void startRender(ClientState *clientStateX) {
    ClientState *clientState = (ClientState*)clientStateX;

    Camera3D camera = {0};
    camera.position = newVector3(0, 0, 0);
    camera.fovy = 90.0f;
    camera.up = newVector3(0, 1, 0);
    camera.target = newVector3(0, 0, 1);
    camera.projection = CAMERA_FREE;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        BeginMode3D(camera);

        DrawGrid(50, 1);

        EndMode3D();
    }
}



/* if (clientState.playerCarregado) {
            system("clear");
            printf("------VOCÊ-----\n");
            printf("Nome: %s\n", clientState.clientPlayer.name);
            printf("\nPosição: {\n");
            printf("    X: %.2f,\n", clientState.clientPlayer.position.x);
            printf("    Y: %.2f,\n", clientState.clientPlayer.position.y);
            printf("    Z: %.2f,\n", clientState.clientPlayer.position.z);
            printf("}\n");
            printf("\nVida: %.2f\n", clientState.clientPlayer.health);
            printf("Yaw: %.2f | Pitch: %.2f\n", clientState.clientPlayer.yaw, clientState.clientPlayer.pitch);
            printf("-----------------\n");

            sleep(1);
        } */