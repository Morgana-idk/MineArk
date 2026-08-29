#include <raylib.h>
#include <stdio.h>
#include "../../../otherLibs/raygui.h"
#include "../../../sharedLibs/clientState.h"

Vector3 newVector3(float x, float y, float z) {
    return (Vector3){(float)x, (float)y, (float)z};
}

void startRender(ClientState *clientStateX) {
    ClientState *clientState = (ClientState*)clientStateX;

    InitWindow(480, 450, "MineArk - 0.2.0");
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = newVector3(0, 1, 0);
    camera.fovy = 90.0f;
    camera.up = newVector3(0, 1, 0);
    camera.target = newVector3(0, 1, 0.5f);
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh cubeMesh = GenMeshCube(1, 1, 1);
    Model cubeModel = LoadModelFromMesh(cubeMesh);

    DisableCursor();

    SetExitKey(KEY_NULL);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        BeginDrawing(); // DRAWING
        ClearBackground(WHITE);


        BeginMode3D(camera); // 3D

        DrawGrid(50, 1);
        DrawModel(cubeModel, newVector3(.5f, .5f, .5f), 1, BLACK);

        EndMode3D(); // !3D
        
        
        EndDrawing(); // !DRAWING
    }

    UnloadMesh(cubeMesh);
    UnloadModel(cubeModel);

    CloseWindow();
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