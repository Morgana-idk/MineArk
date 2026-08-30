#include <raylib.h>
#include <stdio.h>

#include "../../otherLibs/raygui.h"

#include "../clientState.h"
#include "../player/player.h"
#include "../worldGeneration/generation.h"

#define print printf
#define TAMANHO_ATLAS 512
#define TEXTURAS_ATUAIS 1

Texture2D atlasTextura;
Rectangle atlasUVs[TEXTURAS_ATUAIS];
Model modelos[TEXTURAS_ATUAIS]; 
bool atlasCarregado = false;

Vector3 newVector3(float x, float y, float z) {
    return (Vector3){(float)x, (float)y, (float)z};
}

void carregarRecursos() {
    if (atlasCarregado) return;

    Image atlasImagem = GenImageColor(TAMANHO_ATLAS, TAMANHO_ATLAS, BLANK);

    char *caminhoImagens[TEXTURAS_ATUAIS] = {
        "../../assets/textures/blocks/grass.png"
    };

    int posX = 0, posY = 0;
    int tile = 16;

    for (int i = 0; i < TEXTURAS_ATUAIS; i++) {
        Image img = LoadImage(caminhoImagens[i]);

        if (img.data == NULL) {
            img = GenImageColor(1, 1, WHITE);
        }

        posX = (i % (TAMANHO_ATLAS / tile)) * tile;
        posY = (i / (TAMANHO_ATLAS / tile)) * tile;

        ImageDraw(&atlasImagem, img, (Rectangle){0, 0, img.width, img.height}, (Rectangle){posX, posY, img.width, img.height}, WHITE);

        atlasUVs[i] = (Rectangle){posX, posY, img.width, img.height};
        
        Model basicModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
        basicModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(img);

        modelos[i] = basicModel;

        UnloadImage(img);
    }

    atlasTextura = LoadTextureFromImage(atlasImagem);
    UnloadImage(atlasImagem);

    atlasCarregado = true;
}

void renderizarModelo(PlayerModel modelo) {
    DrawCubeV(modelo.torso.position, modelo.torso.size, YELLOW);
    DrawCubeV(modelo.torso.head.position, modelo.torso.head.size, RED);
    DrawCubeV(modelo.torso.leftArm.position, modelo.torso.leftArm.size, BLUE);
    DrawCubeV(modelo.torso.rightArm.position, modelo.torso.rightArm.size, BLUE);
    DrawCubeV(modelo.torso.leftLeg.position, modelo.torso.leftLeg.size, PINK);
    DrawCubeV(modelo.torso.rightLeg.position, modelo.torso.rightLeg.size, PINK);
}

void renderizarMundo(World world) {
    for (int i = 0; i < world.size; i++) {
        for (int j = 0; j < world.chunks[i].blocks_size; j++) {
            Block b = world.chunks[i].blocks[j];
            DrawModel(modelos[b.type], b.position, 1, WHITE);
        }
    }
}

void startRender(ClientState *clientStateX) {

    InitWindow(480, 450, "MineArk - 0.2.0");
    SetTargetFPS(60);
    carregarRecursos();

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
        renderizarModelo(clientStateX->clientPlayer.playerModel);
        renderizarMundo(clientStateX->clientWorld);

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